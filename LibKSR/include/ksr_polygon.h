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

#include <cstdint>
#include "ksr_vertex.h"
#include "ksr_bitmap.h"

namespace KSR
{
    // states of polygons and faces
   /* extern const std::uint16_t POLY4DV1_STATE_ACTIVE;
    extern const std::uint16_t POLY4DV1_STATE_CLIPPED;
    extern const std::uint16_t POLY4DV1_STATE_BACKFACE;*/

    typedef struct POLYGON2D_TYP
    {
        int state;        // state of polygon
        int num_verts;    // number of vertices
        int x0, y0;        // position of center of polygon  
        int xv, yv;        // initial velocity
        std::uint32_t color;      // could be index or PALETTENTRY
        VERTEX2DF* vlist; // pointer to vertex list

    } POLYGON2D, * POLYGON2D_PTR;


    // a polygon based on an external vertex list
    typedef struct POLY4DV1_TYP
    {
        int state;    // state information
        int attr;     // physical attributes of polygon
        int color;    // color of polygon
        POINT4D_PTR vlist; // the vertex list itself
        int vert[3];       // the indices into the vertex list

    }POLY4DV1, *POLY4DV1_PTR;

    // a polygon ver 2.0 based on an external vertex list  //////////////////////////////////
    typedef struct POLY4DV2_TYP
    {
        int state;           // state information
        int attr;            // physical attributes of polygon
        int color;           // color of polygon
        int lit_color[3];    // holds colors after lighting, 0 for flat shading
        // 0,1,2 for vertex colors after vertex lighting

        BITMAP_IMAGE_PTR texture; // pointer to the texture information for simple texture mapping

        int mati;              // material index (-1) no material (new)

        VERTEX4DTV1_PTR vlist; // the vertex list itself 
        POINT2D_PTR     tlist; // the texture list itself (new)
        int vert[3];           // the indices into the vertex list
        int text[3];           // the indices into the texture coordinate list (new)
        float nlength;         // length of normal (new)

    } POLY4DV2, * POLY4DV2_PTR;

    // a self contained polygon used for the render list
    typedef struct POLYF4DV1_TYP
    {
        int state;              // state information
        int attr;               // physical attributes of polygon
        int color;              // 本多边形的颜色
        POINT4D vlist[3];       // 三个元素的数组，每一个元素对应于一个三角形的一个顶点坐标
        POINT4D tvlist[3];      // 三个元素的数组，每一个元素对应于一个三角形的一个经过变换坐标系的顶点坐标
        POLYF4DV1_TYP* next;    // 存储局部顶点坐标的数组 pointer to next polygon in list??
        POLYF4DV1_TYP* prev;    // pointer to previous polygon in list??
    } POLYF4DV1, *POLYF4DV1_PTR;

    // a self contained polygon used for the render list version 2 /////////////////////////
    typedef struct POLYF4DV2_TYP
    {
        int      state;           // state information
        int      attr;            // physical attributes of polygon
        int      color;           // color of polygon
        int      lit_color[3];    // holds colors after lighting, 0 for flat shading
        // 0,1,2 for vertex colors after vertex lighting
        BITMAP_IMAGE_PTR texture; // pointer to the texture information for simple texture mapping

        int      mati;    // material index (-1) for no material  (new)

        float    nlength; // length of the polygon normal if not normalized (new)
        VECTOR4D normal;  // the general polygon normal (new)

        float    avg_z;   // average z of vertices, used for simple sorting (new)

        VERTEX4DTV1 vlist[3];  // the vertices of this triangle 
        VERTEX4DTV1 tvlist[3]; // the vertices after transformation if needed 

        POLYF4DV2_TYP* next;   // pointer to next polygon in list??
        POLYF4DV2_TYP* prev;   // pointer to previous polygon in list??

    } POLYF4DV2, *POLYF4DV2_PTR;

    /**************************************************************************************
    
    @name: KSR::Draw_Filled_Polygon2D
    @return: void
    @param: POLYGON2D_PTR poly
    @param: std::uint8_t * vbuffer
    @param: int mempitch
    *************************************************************************************/
    void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);

    /**************************************************************************************
    
    @name: KSR::Draw_Filled_Polygon2D16
    @return: void
    @param: POLYGON2D_PTR poly
    @param: std::uint8_t * vbuffer
    @param: int mempitch
    *************************************************************************************/
    void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);
}