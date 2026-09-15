#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "MathUtils.hpp"
#include "class/Mesh.hpp"

namespace {

// push to obj vertex
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

// Read exactly the values supplied; fails on missing or trailing junk.
template <typename... Ts> bool parseAll(std::string_view s, Ts&... out) {
  std::istringstream ss{std::string{s}};
  if (!((ss >> out) && ...)) // all must succeed
    return false;
  std::string extra;
  return !(ss >> extra); // nothing must be left
}

// "v", "v/vt", "v//vn", "v/vt/vn" -> 1-based position index, 0 on error.
unsigned faceVertexIndex(std::string_view tok) {
  const auto slash = tok.find('/');
  const std::string_view num =
      (slash == std::string_view::npos) ? tok : tok.substr(0, slash);

  unsigned idx = 0;
  auto [p, ec] = std::from_chars(num.data(), num.data() + num.size(), idx);
  return (ec == std::errc{} && p == num.data() + num.size()) ? idx : 0;
}

// Fan-triangulate any n-gon of 1-based indices into a flat output vector.
void triangulate(const std::vector<unsigned>& face, unsigned offset,
                 std::vector<unsigned>& out) {
  for (std::size_t i = 1; i + 1 < face.size(); ++i) {
    out.push_back(face[0] - offset);
    out.push_back(face[i] - offset);
    out.push_back(face[i + 1] - offset);
  }
}

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
} // namespace

static bool validLine(const std::string& line, const size_t expectedSize,
                      bool indicies = false) {
  std::stringstream ss(line);
  std::string valid;
  std::vector<std::string> vec;
  vec.reserve(expectedSize);

  while (ss >> valid)
    vec.push_back(valid);

  if (vec.size() == expectedSize)
    return true;

  if (!indicies && vec.size() == 4)
    std::cerr << "Invalid line: " << line << std::endl;

  return false;
}

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

  int lineNum{1};

  while (std::getline(file, raw)) {
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
    } else if (prefix == "illum") { /* ignored */
    } else
      std::cerr << "CANNOT RECOGNIZE: " << prefix << "on line: " << lineNum
                << "." << std::endl;
    lineNum += 1;
  }

  if (mtlCount > 1)
    std::cerr << "ONLY 1 MATERIAL IS SUPPORTED" << std::endl;

  mat = tmp;
  return true;
}
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
  constexpr unsigned kIndexOffset = 1;

  std::string raw;
  int lineNum{1};

  while (std::getline(file, raw)) {
    std::string_view line = normalise(raw, lineNum == 1);
    if (line.empty() || line.front() == '#')
      continue;

    auto [prefix, rest] = splitFirst(line);

    if (prefix == "v") {
      float x, y, z;
      if (!parseAll(rest, x, y, z))
        return false;
      pushToVector(x, y, z, kVertexOffset, obj.vertices);
    } else if (prefix == "f") {
      std::vector<unsigned> face;
      std::istringstream ss{std::string{rest}};
      std::string tok;
      while (ss >> tok) {
        const unsigned idx = faceVertexIndex(tok);
        if (idx == 0)
          return false;
        face.push_back(idx);
      }
      if (face.size() < 3)
        return false;
      triangulate(face, kIndexOffset, obj.indices);
    } else if (prefix == "mtllib") {
      std::string fileName;
      if (!parseAll(rest, fileName))
        break;

      const auto dir = path.parent_path();
      if (!parseMaterial(dir / fileName, obj.material))
        std::cerr << "MATERIAL parse failed\n Skipping... USING DEFAULT"
                  << std::endl;

    } else if (prefix == "o") {
      continue;
    } else if (prefix == "usemtl") {
      std::cerr << "USEMTL NOT SUPPORTED\n continuing..." << std::endl;
    } else if (prefix == "s") {
      std::cerr << "SMOOTHING NOT SUPPORTED\n continuing..." << std::endl;
    } else {
      std::cerr << "CANNOT RECOGNIZE: " << raw << "on line: " << lineNum << " "
                << std::endl;
      return false;
    }
    lineNum += 1;
  }
  recenter(obj.vertices);

  return true;
}
