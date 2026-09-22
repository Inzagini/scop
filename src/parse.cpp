#include "parse.hpp"

namespace {

// Appends an offset (x, y, z) triple to a flat float vector.
template <typename T, typename U>
void pushToVector(T x, T y, T z, T offset, std::vector<U>& vec) {
  vec.push_back(x - offset);
  vec.push_back(y - offset);
  vec.push_back(z - offset);
}

// Strip UTF-8 BOM, leading whitespace, and trailing CR/whitespace.
std::string_view normalise(std::string_view s, bool firstLine) {
  if (firstLine && s.size() >= 3 && static_cast<unsigned char>(s[0]) == 0xEF &&
      static_cast<unsigned char>(s[1]) == 0xBB &&
      static_cast<unsigned char>(s[2]) == 0xBF)
    s.remove_prefix(3);

  while (!s.empty() && (s.front() == ' ' || s.front() == '\t'))
    s.remove_prefix(1);
  while (!s.empty() &&
         (s.back() == '\r' || s.back() == ' ' || s.back() == '\t'))
    s.remove_suffix(1);

  return s;
}

// Split "v 1.0 2.0 3.0" -> {"v", "1.0 2.0 3.0"}.
std::pair<std::string_view, std::string_view>
splitFirst(std::string_view line) {
  const auto sp = line.find_first_of(" \t");
  if (sp == std::string_view::npos)
    return {line, {}};

  std::string_view rest = line.substr(sp + 1);
  while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t'))
    rest.remove_prefix(1);
  return {line.substr(0, sp), rest};
}

// Parses exactly the supplied values, rejecting trailing junk.
template <typename... Ts> bool parseAll(std::string_view s, Ts&... out) {
  std::istringstream ss{std::string{s}};
  if (!((ss >> out) && ...))
    return false;
  std::string extra;
  return !(ss >> extra);
}

// Parses the first values, ignoring trailing junk.
template <typename... Ts> bool parsePrefix(std::string_view s, Ts&... out) {
  std::istringstream ss{std::string{s}};
  return ((ss >> out) && ...);
}

// Parses a full-token decimal integer.
bool parseInt(std::string_view s, int& out) {
  if (s.empty())
    return false;
  auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), out);
  return ec == std::errc{} && p == s.data() + s.size();
}

// Parses a "v/vt" face token into position and UV indices.
bool parseFaceRef(std::string_view tok, int& vOut, int& vtOut) {
  const auto s1 = tok.find('/');
  vtOut = 0;

  if (!parseInt(s1 == std::string_view::npos ? tok : tok.substr(0, s1), vOut))
    return false;

  if (s1 != std::string_view::npos) {
    const auto s2 = tok.find('/', s1 + 1);
    const std::string_view vt = (s2 == std::string_view::npos)
                                    ? tok.substr(s1 + 1)
                                    : tok.substr(s1 + 1, s2 - s1 - 1);
    if (!vt.empty() && !parseInt(vt, vtOut))
      return false;
  }
  return true;
}

// Ear-clips a polygon (convex, concave or non-planar) into triangles.
void triangulate(const std::vector<unsigned>& face,
                 const std::vector<float>& vertices,
                 std::vector<unsigned>& out) {
  const std::size_t n = face.size();
  if (n < 3)
    return;

  auto P = [&](unsigned idx) {
    return Vec3(vertices[3 * idx], vertices[3 * idx + 1],
                vertices[3 * idx + 2]);
  };

  if (n == 3) {
    out.push_back(face[0]);
    out.push_back(face[1]);
    out.push_back(face[2]);
    return;
  }

  Vec3 normal(0.0f);
  for (std::size_t i = 0; i < n; ++i) {
    const Vec3 a = P(face[i]);
    const Vec3 b = P(face[(i + 1) % n]);
    normal.x += (a.y - b.y) * (a.z + b.z);
    normal.y += (a.z - b.z) * (a.x + b.x);
    normal.z += (a.x - b.x) * (a.y + b.y);
  }

  const float ax = std::fabs(normal.x), ay = std::fabs(normal.y),
              az = std::fabs(normal.z);
  const int axis = (ay >= ax && ay >= az) ? 1 : (az >= ax ? 2 : 0);

  auto ux = [&](const Vec3& p) { return axis == 0 ? p.y : p.x; };
  auto uy = [&](const Vec3& p) { return axis == 2 ? p.y : p.z; };
  auto cross2 = [&](const Vec3& a, const Vec3& b, const Vec3& c) {
    return (ux(b) - ux(a)) * (uy(c) - uy(a)) -
           (uy(b) - uy(a)) * (ux(c) - ux(a));
  };

  std::vector<unsigned> poly(face);
  float area = 0.0f;
  for (std::size_t i = 0; i < n; ++i) {
    const Vec3 a = P(poly[i]);
    const Vec3 b = P(poly[(i + 1) % n]);
    area += ux(a) * uy(b) - ux(b) * uy(a);
  }
  const float sign = area >= 0.0f ? 1.0f : -1.0f;

  std::size_t guard = 0;
  while (poly.size() > 3 && guard++ <= n * n) {
    std::size_t best = poly.size();
    float bestDiag = 0.0f;
    for (std::size_t i = 0; i < poly.size(); ++i) {
      const std::size_t prev = (i + poly.size() - 1) % poly.size();
      const std::size_t next = (i + 1) % poly.size();
      const Vec3 a = P(poly[prev]), b = P(poly[i]), c = P(poly[next]);

      if (sign * cross2(a, b, c) <= 0.0f)
        continue;

      bool ear = true;
      for (std::size_t j = 0; j < poly.size() && ear; ++j) {
        if (j == prev || j == i || j == next)
          continue;
        const Vec3 p = P(poly[j]);
        if (sign * cross2(a, b, p) >= 0.0f && sign * cross2(b, c, p) >= 0.0f &&
            sign * cross2(c, a, p) >= 0.0f)
          ear = false;
      }
      if (!ear)
        continue;

      const float diag = MathUtils::length(a - c);
      if (best == poly.size() || diag < bestDiag) {
        best = i;
        bestDiag = diag;
      }
    }

    if (best == poly.size())
      break;

    const std::size_t prev = (best + poly.size() - 1) % poly.size();
    const std::size_t next = (best + 1) % poly.size();
    out.push_back(poly[prev]);
    out.push_back(poly[best]);
    out.push_back(poly[next]);
    poly.erase(poly.begin() + best);
  }

  for (std::size_t i = 1; i + 1 < poly.size(); ++i) {
    out.push_back(poly[0]);
    out.push_back(poly[i]);
    out.push_back(poly[i + 1]);
  }
}

// Recentres vertices on their bounding-box centre; optionally normalises size.
void recenter(std::vector<float>& verts, bool normaliseSize = false) {
  if (verts.empty())
    return;

  float minX = verts[0], minY = verts[1], minZ = verts[2];
  float maxX = minX, maxY = minY, maxZ = minZ;

  for (std::size_t i = 0; i + 2 < verts.size(); i += 3) {
    minX = std::min(minX, verts[i]);
    maxX = std::max(maxX, verts[i]);
    minY = std::min(minY, verts[i + 1]);
    maxY = std::max(maxY, verts[i + 1]);
    minZ = std::min(minZ, verts[i + 2]);
    maxZ = std::max(maxZ, verts[i + 2]);
  }

  const float cx = 0.5f * (minX + maxX);
  const float cy = 0.5f * (minY + maxY);
  const float cz = 0.5f * (minZ + maxZ);

  float scale = 1.0f;
  if (normaliseSize) {
    const float ex = maxX - minX, ey = maxY - minY, ez = maxZ - minZ;
    const float longest = std::max({ex, ey, ez});
    if (longest > 0.0f)
      scale = 1.0f / longest;
  }

  for (std::size_t i = 0; i + 2 < verts.size(); i += 3) {
    verts[i] = (verts[i] - cx) * scale;
    verts[i + 1] = (verts[i + 1] - cy) * scale;
    verts[i + 2] = (verts[i + 2] - cz) * scale;
  }
}

// Resolves a 1-based (or negative) OBJ index; returns -1 if out of range.
int resolveIndex(int idx, std::size_t count) {
  if (idx > 0)
    return (static_cast<std::size_t>(idx) <= count) ? idx - 1 : -1;
  if (idx < 0)
    return (static_cast<std::size_t>(-idx) <= count)
               ? static_cast<int>(count) + idx
               : -1;
  return -1;
}

// Returns the deduplicated output vertex for a position/UV pair, appending it.
unsigned getVertex(int vIdx, int vtIdx, const std::vector<float>& rawPos,
                   const std::vector<float>& rawUV, ObjProp& obj,
                   std::unordered_map<std::uint64_t, unsigned>& vertexMap) {
  const int nv = static_cast<int>(rawPos.size() / 3);
  const int nvt = static_cast<int>(rawUV.size() / 2);

  const int rv = resolveIndex(vIdx, static_cast<std::size_t>(nv));
  if (rv < 0)
    return 0;
  const int rvt =
      (vtIdx == 0) ? -1 : resolveIndex(vtIdx, static_cast<std::size_t>(nvt));

  const std::uint64_t key =
      (static_cast<std::uint64_t>(rv) << 32) |
      (rvt < 0 ? 0xFFFFFFFFu : static_cast<std::uint32_t>(rvt));
  auto it = vertexMap.find(key);
  if (it != vertexMap.end())
    return it->second;

  const unsigned newIdx = static_cast<unsigned>(obj.vertices.size() / 3);

  const std::size_t pi = 3 * static_cast<std::size_t>(rv);
  obj.vertices.push_back(rawPos[pi]);
  obj.vertices.push_back(rawPos[pi + 1]);
  obj.vertices.push_back(rawPos[pi + 2]);

  if (rvt >= 0 && 2 * static_cast<std::size_t>(rvt) + 1 < rawUV.size()) {
    obj.texCoords.push_back(rawUV[2 * static_cast<std::size_t>(rvt)]);
    obj.texCoords.push_back(rawUV[2 * static_cast<std::size_t>(rvt) + 1]);
  } else {
    obj.texCoords.push_back(0.0f);
    obj.texCoords.push_back(0.0f);
  }

  vertexMap[key] = newIdx;
  return newIdx;
}
} // namespace

// Parses an .mtl file into a Material (only the first material is kept).
static bool parseMaterial(const std::filesystem::path& fileName,
                          Material& mat) {
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "UNABLE TO OPEN MTL FILE: " << fileName << std::endl;
    return false;
  }
  std::cout << "OPENED MTL FILE: " << fileName << '\n';

  Material tmp;
  int mtlCount = 0;
  std::string raw;

  auto readRgb = [&](const std::string_view rest, Vec3& dst) {
    float r, g, b;
    if (!parseAll(rest, r, g, b))
      return false;
    dst = Vec3(r, g, b);
    return true;
  };

  int lineNum{};
  while (std::getline(file, raw)) {

    lineNum += 1;
    std::string_view line = normalise(raw, lineNum == 1);
    if (line.empty() || line.front() == '#')
      continue;

    auto [prefix, rest] = splitFirst(line);
    if (prefix == "Ns") {
      if (!parseAll(rest, tmp.shininess))
        return false;
    } else if (prefix == "Ka") {
      if (!readRgb(rest, tmp.ambient))
        return false;
    } else if (prefix == "Kd") {
      if (!readRgb(rest, tmp.diffuse))
        return false;
    } else if (prefix == "Ks") {
      if (!readRgb(rest, tmp.specular))
        return false;
    } else if (prefix == "d") {
      if (!parseAll(rest, tmp.opacity))
        return false;
    } else if (prefix == "newmtl") {
      ++mtlCount;
    } else if (prefix == "illum" || prefix == "Ni" || prefix == "Tr" ||
               prefix == "Tf" || prefix == "Ke" || prefix == "map_Ka" ||
               prefix == "map_Ks" || prefix == "map_Ns" || prefix == "map_d" ||
               prefix == "map_bump" || prefix == "bump") {
      continue;
    } else if (prefix == "map_Kd") {

      std::cerr << "[Mtl] got map_Kd, rest = '" << rest << "'\n";
      const auto sp = rest.find_last_of(" \t");
      const std::string_view file =
          (sp == std::string_view::npos) ? rest : rest.substr(sp + 1);
      if (file.empty())
        return false;
      tmp.diffuseMap = (fileName.parent_path() / file).string();
      std::cerr << "[Mtl] diffuseMap -> '" << tmp.diffuseMap << "'\n";
    } else
      std::cerr << "Line: " << lineNum << " CANNOT RECOGNIZE: " << prefix
                << std::endl;
  }

  if (mtlCount > 1)
    std::cerr << "ONLY 1 MATERIAL IS SUPPORTED" << std::endl;

  mat = tmp;
  return true;
}

// Parses an .obj file into ObjProp (vertices, UVs, indices, material).
bool parseObj(const char* filePath, ObjProp& obj) {
  std::filesystem::path path(filePath);

  if (path.extension() != ".obj") {
    std::cerr << "File is not type .obj" << std::endl;
    return false;
  }

  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "UNABLE TO OPEN OBJECT FILE" << std::endl;
    return false;
  }
  std::cout << "OPENED OBJ File: " << filePath << '\n';

  constexpr float kVertexOffset = 0.0f;

  std::vector<float> rawPos;
  std::vector<float> rawUV;

  std::unordered_map<std::uint64_t, unsigned> vertexMap;

  std::string raw;
  int lineNum{};

  while (std::getline(file, raw)) {

    lineNum += 1;
    std::string_view line = normalise(raw, lineNum == 1);
    if (line.empty() || line.front() == '#')
      continue;

    auto [prefix, rest] = splitFirst(line);

    if (prefix == "v") {
      float x, y, z;
      if (!parsePrefix(rest, x, y, z))
        return false;
      rawPos.push_back(x);
      rawPos.push_back(y);
      rawPos.push_back(z);

    } else if (prefix == "vn") {
      continue;

    } else if (prefix == "vt") {
      float u, v;
      if (!parsePrefix(rest, u, v))
        return false;
      rawUV.push_back(u);
      rawUV.push_back(v);

    } else if (prefix == "g") {
      continue;

    } else if (prefix == "f") {
      std::vector<unsigned> face;
      std::istringstream ss{std::string{rest}};
      std::string tok;
      while (ss >> tok) {
        int v, vt;
        if (!parseFaceRef(tok, v, vt))
          return false;
        face.push_back(getVertex(v, vt, rawPos, rawUV, obj, vertexMap));
      }
      if (face.size() < 3)
        return false;
      triangulate(face, obj.vertices, obj.indices);

    } else if (prefix == "mtllib") {
      std::string fileName;
      if (!parseAll(rest, fileName))
        break;

      const auto dir = path.parent_path();
      if (!parseMaterial(dir / fileName, obj.material))
        std::cerr << "MATERIAL parse failed\n Skipping... USING DEFAULT"
                  << std::endl;
      std::cerr << "[Obj] mtllib file: " << fileName << '\n';
    } else if (prefix == "o") {
      continue;

    } else if (prefix == "usemtl") {
      continue;

    } else if (prefix == "s") {
      std::cerr << "SMOOTHING NOT SUPPORTED\n continuing..." << std::endl;

    } else {
      std::cerr << "Line: " << lineNum << " CANNOT RECOGNIZE: " << raw
                << std::endl;
      return false;
    }
  }

  recenter(obj.vertices);

  obj.normals.assign(obj.vertices.size(), 0.0f);
  for (std::size_t i = 0; i + 2 < obj.indices.size(); i += 3) {
    const unsigned a = obj.indices[i], b = obj.indices[i + 1],
                   c = obj.indices[i + 2];
    auto P = [&](unsigned v) {
      return Vec3(obj.vertices[3 * v], obj.vertices[3 * v + 1],
                  obj.vertices[3 * v + 2]);
    };
    const Vec3 n = MathUtils::cross(P(b) - P(a), P(c) - P(a));
    for (unsigned v : {a, b, c}) {
      obj.normals[3 * v] += n.x;
      obj.normals[3 * v + 1] += n.y;
      obj.normals[3 * v + 2] += n.z;
    }
  }
  for (std::size_t v = 0; v + 2 < obj.normals.size(); v += 3) {
    const Vec3 n = MathUtils::normalize(
        Vec3(obj.normals[v], obj.normals[v + 1], obj.normals[v + 2]));
    obj.normals[v] = n.x;
    obj.normals[v + 1] = n.y;
    obj.normals[v + 2] = n.z;
  }

  return true;
}
