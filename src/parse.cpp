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
    while (!std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        
        std::string prefix;
        ss >> prefix;

        float r{}, g{}, b{}; //r is reused for float

        if (prefix == "Ns")
        {
            ss >> r;
            mat.shininess = r;
        }
        else if (prefix == "Ka")
        {
            ss >> r >> g >> b;
            mat.ambient = glm::vec3(r, g, b);
        }
        else if (prefix == "Kd")
        {
            ss >> r >> g >> b;
            mat.diffuse = glm::vec3(r, g, b);
        }
        else if (prefix == "Ks")
        {
            ss >> r >> g >> b;
            mat.specular = glm::vec3(r, g, b);
        }
        else if (prefix == "d")
        {
            ss >> r;
            mat.opacity = r;
        }
    }
    
    return mat;
}

bool parseObj(const char *filePath, Objprop &obj)
{
    std::vector<float> verticies;
    verticies.reserve(1000);
    
    std::filesystem::path path(filePath);

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
            float x{}, y{}, z{};

            ss >> x >> y >> z;
            obj.vertices.push_back(x);
            obj.vertices.push_back(y);
            obj.vertices.push_back(z);
        }
        else if (prefix == "f")
        {
            std::string vertexStr;
            // vertexStr could be "16" or "16/1/1"
            while (ss >> vertexStr)
            {
                std::stringstream vs(vertexStr);
                unsigned int vertexIndex;
                
                vs >> vertexIndex;
                obj.indices.push_back(vertexIndex - 1);
            }
        }
        else if (prefix == "mtllib")
        {
            std::string fileName;
            ss >> fileName;

            std::string dirPath = path.parent_path();
            obj.material = parseMaterial(dirPath + "/" + fileName);
        }
    }
    return true;
}