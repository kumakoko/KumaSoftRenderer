#pragma once

#include "ksr_vector.h"

namespace KSR
{
    // a 2D vertex
    typedef struct VERTEX2DI_TYP
    {
        int x, y; // the vertex
    } VERTEX2DI, * VERTEX2DI_PTR;

    // a 2D vertex
    typedef struct VERTEX2DF_TYP
    {
        float x, y; // the vertex
    } VERTEX2DF, * VERTEX2DF_PTR;


    // 4D homogeneous vertex with 2 texture coordinates, and vertex normal ////////////////
// normal can be interpreted as vector or point
    typedef struct VERTEX4DTV1_TYP
    {
        union
        {
            float M[12];            // array indexed storage

            // explicit names
            struct
            {
                float x, y, z, w;     // point
                float nx, ny, nz, nw; // normal (vector or point)
                float u0, v0;       // texture coordinates 

                float i;           // final vertex intensity after lighting
                int   attr;        // attributes/ extra texture coordinates
            };                 // end struct

            // high level types
            struct
            {
                POINT4D  v;       // the vertex
                VECTOR4D n;       // the normal
                POINT2D  t;       // texture coordinates
            };

        }; // end union

    } VERTEX4DTV1, * VERTEX4DTV1_PTR;


}