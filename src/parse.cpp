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

template <typename T, typename U>
void pushToVector(T x, T y, T z, T offset, std::vector<U>& vec) {
  vec.push_back(x - offset);
  vec.push_back(y - offset);
  vec.push_back(z - offset);
}

namespace {

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
    std::cerr << "Invalid line: " << line << '\n';

  return false;
}

static bool parseMaterial(const std::filesystem::path& fileName,
                          Material& mat) {
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "UNABLE TO OPEN MTL FILE: " << fileName << '\n';
    return false;
  }
  std::cout << "OPENED: " << fileName << '\n';

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

  while (std::getline(file, raw)) {
    std::string_view line = raw;
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
      std::cerr << "CANNOT RECOGNIZE: " << prefix << '\n';
  }

  if (mtlCount > 1)
    std::cerr << "ONLY 1 MATERIAL IS SUPPORTED\n";

  mat = tmp;
  return true;
}
bool parseObj(const char* filePath, ObjProp& obj) {
  std::filesystem::path path(filePath);

  if (path.extension() != ".obj") {
    std::cerr << "File is not type .obj\n";
    return false;
  }

  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "UNABLE TO OPEN OBJECT FILE\n";
    return false;
  }
  std::cout << "OPENED: " << filePath << '\n';

  constexpr float kVertexOffset = 0.0f;
  constexpr unsigned kIndexOffset = 1;

  std::string raw;
  while (std::getline(file, raw)) {
    std::string_view line = raw;
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
        std::cerr << "MATERIAL parse failed\n Skipping... USING DEFAULT\n";
    } else if (prefix == "o") {
      continue;
    } else if (prefix == "usemtl") {
      std::cerr << "USEMTL NOT SUPPORTED\n continuing...\n";
    } else if (prefix == "s") {
      std::cerr << "SMOOTHING NOT SUPPORTED\n continuing...\n";
    } else {
      std::cerr << "CANNOT RECOGNIZE: " << raw << '\n';
      return false;
    }
  }
  return true;
}
