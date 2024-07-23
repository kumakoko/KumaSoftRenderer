#pragma once

#include <cstdint>
#include "ksr_vertex.h"

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