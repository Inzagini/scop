#include <fstream>
#include <istream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "class/Mesh.hpp"

template<typename T, typename U>
void pushToVector(T x, T y, T z, T offset,std::vector<U> &vec)
{
    vec.push_back(x - offset);
    vec.push_back(y - offset);
    vec.push_back(z - offset);
}

static bool validLine(const std::string &line, const size_t expectedSize, bool indicies = false)
{
    std::stringstream ss(line);
    std::string valid;
    std::vector<std::string> vec;
    vec.reserve(expectedSize);

    while (ss >> valid)
    {
        vec.push_back(valid);
    }

    if (vec.size() == expectedSize)
        return true;

    
    if (!indicies && vec.size() == 4)
        std::cerr << "Invalid line: " << line << '\n';     

    return false;
}


static Material parseMaterial(const std::string &fileName)
{
    
    Material mat;
    mat.ambient = glm::vec3(1.0f);
    mat.diffuse = glm::vec3(1.0f);
    mat.specular = glm::vec3(1.0f);
    mat.opacity = 1.0f;
    mat.shininess = 0.0f;

    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "UNABLE TO OPEN MTL FILE: " << fileName << '\n';
        return mat;
    }

    std::cout << "OPENED: " << fileName << '\n';

    std::string line;
    int mtlCount{};

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        float r{}, g{}, b{}; //r is reused for float
        

        if (prefix == "Ns")
        {
            if(!validLine(line, 2))
                break;
            
            ss >> r;
            mat.shininess = r;
        }
        else if (prefix == "Ka")
        {
            if(!validLine(line, 4))
                break;
            ss >> r >> g >> b;
            mat.ambient = glm::vec3(r, g, b);
        }
        else if (prefix == "Kd")
        {
            if(!validLine(line, 4))
                break;
            ss >> r >> g >> b;
            mat.diffuse = glm::vec3(r, g, b);
        }
        else if (prefix == "Ks")
        {
            if(!validLine(line, 4))
                break;
            ss >> r >> g >> b;
            mat.specular = glm::vec3(r, g, b);
        }
        else if (prefix == "d")
        {
            if(!validLine(line, 2))
                break;
            ss >> r;
            mat.opacity = r;
        }
        else if (prefix == "newmtl")
        {
            mtlCount += 1;
        }
        else if (prefix == "illum")
        {
            continue;
        }
        else
        {
            std::cerr << "CANNOT RECOGNIZE: " << prefix << '\n';
        }
    }
    
    if (mtlCount > 1)
        std::cerr << "ONLY 1 MATERIAL IS SUPPORTED\n";

    return mat;
}

bool parseObj(const char *filePath, ObjProp &obj)
{ 
    std::filesystem::path path(filePath);

    if (path.extension() != ".obj")
    {
        std::cerr << "File is not type .obj\n";
        return false;
    }

    std::ifstream file(filePath);
    if (!file)
    {
        std::cerr << "UNABLE TO OPEN OBJECT FILE\n";
        return false;
    }

    std::cout << "OPENED: " << filePath << '\n';

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string prefix;

        ss >> prefix;

        if (prefix == "v")
        {
            if(!validLine(line, 4, true))
                return false;
            float x{}, y{}, z{};

            ss >> x >> y >> z;
            pushToVector(x, y, z, 0.0f, obj.vertices);
        }
        else if (prefix == "f")
        {
            unsigned int w{}, x{}, y{}, z{};
            bool parsingIndicies = true;

            if(validLine(line, 4, parsingIndicies))
            {
                ss >> x >> y >> z;
                pushToVector(x, y, z, 1u, obj.indices);
            }
            else if(validLine(line, 5, parsingIndicies))
            {
                ss >> x >> y >> z >> w;
                pushToVector(x, y, z, 1u, obj.indices);
                pushToVector(x, y, w, 1u, obj.indices);
            }
            else
                return false;
        }
        else if (prefix == "mtllib")
        {
            if(!validLine(line, 2))
            {
                Material mat;
                mat.ambient = glm::vec3(1.0f);
                mat.diffuse = glm::vec3(1.0f);
                mat.specular = glm::vec3(1.0f);
                mat.opacity = 1.0f;
                mat.shininess = 0.0f;

                obj.material = mat;
                break;
            }
            std::string fileName;
            ss >> fileName;

            std::string dirPath = path.parent_path();
            obj.material = parseMaterial(dirPath + "/" + fileName);
        }
        else if (prefix == "o")
        {
            //in future make it to handle multiple obj (voxel project)
            continue;
        }
        else if (prefix == "usemtl") //not supported 
        {
            std::cerr << "USEMTL NOT SUPPORTED\n continuing...\n";
        }
        else if (prefix == "s") 
        {
            std::cerr << "SMOOTHING NOT SUPPORTED\n continuing...\n";
        }
        else
        {
            std::cerr << "CANNOT RECOGNIZE: " << line << '\n';
            return false;
        }
    }
    return true;
}

