#pragma once

#include <cstdint>
#include "ksr_vertex.h"
#include "ksr_bitmap.h"

namespace KSR
{
    typedef struct POLYGON2D_TYP
    {
        int state;        // state of polygon
        int num_verts;    // number of vertices
        int x0, y0;        // position of center of polygon  
        int xv, yv;        // initial velocity
        std::uint32_t color;      // could be index or PALETTENTRY
        VERTEX2DF* vlist; // pointer to vertex list

    } POLYGON2D, * POLYGON2D_PTR;


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

    } POLYF4DV2, * POLYF4DV2_PTR;

    /******************************************************************************************************************
     * Desc:
     * Method:    Draw_Filled_Polygon2D
     * Returns:   void
     * Parameter: POLYGON2D_PTR poly
     * Parameter: std::uint8_t * vbuffer
     * Parameter: int mempitch
     ****************************************************************************************************************/
    void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);
    void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);
}