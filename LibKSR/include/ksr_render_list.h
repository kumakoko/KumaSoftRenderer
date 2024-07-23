#pragma once

#include "ksr_vector.h"
#include "ksr_polygon.h"

namespace KSR
{

    // render list defines ver 2.0
    extern constexpr int RENDERLIST4DV2_MAX_POLYS = 32768;//          32768


    // object to hold the render list version 2.0, this way we can have more
// than one render list at a time
    typedef struct RENDERLIST4DV2_TYP
    {
        int state; // state of renderlist ???
        int attr;  // attributes of renderlist ???

        // the render list is an array of pointers each pointing to 
        // a self contained "renderable" polygon face POLYF4DV2
        POLYF4DV2_PTR poly_ptrs[RENDERLIST4DV2_MAX_POLYS];

        // additionally to cut down on allocatation, de-allocation
        // of polygons each frame, here's where the actual polygon
        // faces will be stored
        POLYF4DV2 poly_data[RENDERLIST4DV2_MAX_POLYS];

        int num_polys; // number of polys in render list

    } RENDERLIST4DV2, * RENDERLIST4DV2_PTR;
}
