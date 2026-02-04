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
#include "MathUtils.hpp"

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


static bool parseMaterial(const std::string &fileName, Material &mat)
{
    std::ifstream file(fileName);
    if (!file)
    {
        std::cerr << "UNABLE TO OPEN MTL FILE: " << fileName << '\n';
        return false;
    }

    std::cout << "OPENED: " << fileName << '\n';

    Material tmpMat;
    std::string line;
    int mtlCount{};
    bool valid = true;

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
                return false;
            
            ss >> r;
            tmpMat.shininess = r;
        }
        else if (prefix == "Ka")
        {
            if(!validLine(line, 4))
                return false;

            ss >> r >> g >> b;
            tmpMat.ambient = Vec3(r, g, b);
        }
        else if (prefix == "Kd")
        {
            if(!validLine(line, 4))
                return false;
            ss >> r >> g >> b;
            tmpMat.diffuse = Vec3(r, g, b);
        }
        else if (prefix == "Ks")
        {
            if(!validLine(line, 4))
                return false;
            ss >> r >> g >> b;
            tmpMat.specular = Vec3(r, g, b);
        }
        else if (prefix == "d")
        {
            if(!validLine(line, 2))
                return false;
            ss >> r;
            tmpMat.opacity = r;
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

    mat = tmpMat;

    return true;
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

    Material mat; //defaut setting
    mat.ambient = Vec3(0.0f);
    mat.diffuse = Vec3(0.8f);
    mat.specular = Vec3(1.0f);
    mat.opacity = 1.0f;
    mat.shininess = 32.0f;

    obj.material = mat;

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
            const float offset{0.0f};

            ss >> x >> y >> z;
            pushToVector(x, y, z, offset, obj.vertices);
        }
        else if (prefix == "f")
        {
            unsigned int w{}, x{}, y{}, z{};
            unsigned int offset{1};
            bool parsingIndicies = true;

            if(validLine(line, 4, parsingIndicies))
            {
                ss >> x >> y >> z;
                pushToVector(x, y, z, offset, obj.indices);
            }
            else if(validLine(line, 5, parsingIndicies))
            {
                ss >> x >> y >> z >> w;
                pushToVector(x, y, z, offset, obj.indices);
                pushToVector(x, z, w, offset, obj.indices);
            }
            else
                return false;
        }
        else if (prefix == "mtllib")
        {
            if(!validLine(line, 2))
                break;

            std::string fileName;
            ss >> fileName;

            std::string dirPath = path.parent_path();
            if (!parseMaterial(dirPath + "/" + fileName, obj.material))
                std::cerr << "MATERIAL parse failed \n Skipping... USING DEFAULT\n";
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

