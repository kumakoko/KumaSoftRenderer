/*********************************************************************************************
MIT License

Copyright (c) 2024 kumakoko www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*********************************************************************************************/

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