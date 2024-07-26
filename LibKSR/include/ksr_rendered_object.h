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
#include "ksr_vertex.h"
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

    // an object ver 2.0 based on a vertex list and list of polygons //////////////////////////
// this new object has a lot more flexibility and it supports "framed" animation
// that is this object can hold hundreds of frames of an animated mesh as long as
// the mesh has the same polygons and geometry, but with changing vertex positions
// similar to the Quake II .md2 format
    typedef struct OBJECT4DV2_TYP
    {
        int   id;           // numeric id of this object
        char  name[64];     // ASCII name of object just for kicks
        int   state;        // state of object
        int   attr;         // attributes of object
        int   mati;         // material index overide (-1) - no material (new)
        float* avg_radius;  // [OBJECT4DV2_MAX_FRAMES];   // average radius of object used for collision detection
        float* max_radius;  // [OBJECT4DV2_MAX_FRAMES];   // maximum radius of object

        POINT4D world_pos;  // position of object in world

        VECTOR4D dir;       // rotation angles of object in local
        // cords or unit direction vector user defined???

        VECTOR4D ux, uy, uz;  // local axes to track full orientation
        // this is updated automatically during
        // rotation calls

        int num_vertices;   // number of vertices per frame of this object
        int num_frames;     // number of frames
        int total_vertices; // total vertices, redudant, but it saves a multiply in a lot of places
        int curr_frame;     // current animation frame (0) if single frame

        VERTEX4DTV1_PTR vlist_local; // [OBJECT4DV1_MAX_VERTICES]; // array of local vertices
        VERTEX4DTV1_PTR vlist_trans; // [OBJECT4DV1_MAX_VERTICES]; // array of transformed vertices

        // these are needed to track the "head" of the vertex list for mult-frame objects
        VERTEX4DTV1_PTR head_vlist_local;
        VERTEX4DTV1_PTR head_vlist_trans;

        // texture coordinates list (new)
        POINT2D_PTR tlist;       // 3*num polys at max

        BITMAP_IMAGE_PTR texture; // pointer to the texture information for simple texture mapping (new)

        int num_polys;           // number of polygons in object mesh
        POLY4DV2_PTR plist;      // ptr to polygons (new)

        int   ivar1, ivar2;      // auxiliary vars
        float fvar1, fvar2;      // auxiliary vars

        // METHODS //////////////////////////////////////////////////

        // setting the frame is so important that it should be a member function
        // calling functions without doing this can wreak havok!
        int Set_Frame(int frame);

    } OBJECT4DV2, * OBJECT4DV2_PTR;
}
