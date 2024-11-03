#include "tiny3d_obj_loader.h"


bool OBJLoader::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {  // Vertex position
            T3DVector4 pos;
            iss >> pos.x >> pos.y >> pos.z;
            pos.w = 1.0f;
            positions.push_back(pos);
        }
        else if (type == "vt")
        {  // Texture coordinate
            T3DTextureCoord tc;
            iss >> tc.u >> tc.v;
            texcoords.push_back(tc);
        }
        else if (type == "f")
        {  
            // Face
            for (int i = 0; i < 3; i++)
            {  // Assuming triangulated faces
                std::string vertex;
                iss >> vertex;

                // Parse vertex indices (format: position/texcoord)
                size_t pos = vertex.find('/');
                int v_idx = std::stoi(vertex.substr(0, pos)) - 1;
                int vt_idx = std::stoi(vertex.substr(pos + 1)) - 1;

                // Create vertex
                T3DVertex vert;
                vert.pos = positions[v_idx];
                vert.tc = texcoords[vt_idx];
                vert.rhw = 1.0f;

                // Set default color (white)
                vert.color = { 1.0f, 1.0f, 1.0f };

                vertices.push_back(vert);
            }
        }
    }

    file.close();
    return true;
}

const std::vector<T3DVertex>& OBJLoader::GetVertices() const
{
    return vertices;
}


//// Example usage:
//void LoadOBJExample() {
//    OBJLoader loader;
//    if (loader.LoadFromFile("model.obj")) {
//        const std::vector<vertex_t>& vertices = loader.GetVertices();
//        std::cout << "Loaded " << vertices.size() << " vertices" << std::endl;
//        
//        // Now you can use these vertices for rendering
//        for (const auto& vertex : vertices) {
//            // Process each vertex...
//            std::cout << "Position: " << vertex.pos.x << ", " << vertex.pos.y << ", " << vertex.pos.z << std::endl;
//            std::cout << "TexCoord: " << vertex.tc.u << ", " << vertex.tc.v << std::endl;
//        }
//    }
//}