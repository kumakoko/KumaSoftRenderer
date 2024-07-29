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
#include "ksr_polygon.h"

namespace KSR
{
    constexpr int OBJECT4DV1_MAX_VERTICES = 1024;  // 64
    constexpr int OBJECT4DV1_MAX_POLYS = 1024; // 128

    // an object based on a vertex list and list of polygons
    typedef struct OBJECT4DV1_TYP
    {
        int  id;           // numeric id of this object
        char name[64];     // ASCII name of object just for kicks
        int  state;        // state of object
        int  attr;         // attributes of object
        float avg_radius;  // average radius of object used for collision detection
        float max_radius;  // maximum radius of object

        POINT4D world_pos;  // position of object in world

        VECTOR4D dir;       // rotation angles of object in local
        // cords or unit direction vector user defined???

        VECTOR4D ux, uy, uz;  // local axes to track full orientation
        // this is updated automatically during
        // rotation calls

        int num_vertices;   // number of vertices of this object

        POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES]; // array of local vertices
        POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES]; // array of transformed vertices

        int num_polys;        // number of polygons in object mesh
        POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];  // array of polygons
    } OBJECT4DV1, * OBJECT4DV1_PTR;

    /**************************************************************************************

    @name: KSR::Compute_OBJECT4DV1_Radius
    @return: float
    @param: OBJECT4DV1_PTR obj
    *************************************************************************************/
    float Compute_OBJECT4DV1_Radius(OBJECT4DV1_PTR obj);

    /**************************************************************************************

     * @name: Load_OBJECT4DV1_PLG
     * @return: bool
     * @param: OBJECT4DV1_PTR obj
     * @param: const char * filename
     * @param: VECTOR4D_PTR scale initial scaling factors
     * @param: VECTOR4D_PTR pos   initial position
     * @param: VECTOR4D_PTR rot
     *************************************************************************************/
    bool Load_OBJECT4DV1_PLG(OBJECT4DV1_PTR obj, const char* filename, VECTOR4D_PTR scale, VECTOR4D_PTR pos, VECTOR4D_PTR rot);

}