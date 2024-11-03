#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "tiny3d_geometry.h"


class OBJLoader 
{
private:
    std::vector<T3DVertex> vertices;
    std::vector<T3DVector4> positions;
    std::vector<T3DTextureCoord> texcoords;
    std::vector<T3DColor> colors;

public:
    bool LoadFromFile(const std::string& filename);

    const std::vector<T3DVertex>& GetVertices() const;
};