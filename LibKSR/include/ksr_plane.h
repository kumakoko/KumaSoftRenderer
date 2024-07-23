#pragma once

#include "ksr_vector.h"

namespace KSR
{
    // 3D plane ///////////////////////////////////////////////////
    typedef struct PLANE3D_TYP
    {
        POINT3D p0; // point on the plane
        VECTOR3D n; // normal to the plane (not necessarily a unit vector)
    } PLANE3D, * PLANE3D_PTR;

    inline void POINT3D_COPY(POINT3D_PTR vdst, POINT3D_PTR vsrc)
    {
        vdst->x = vsrc->x; 
        vdst->y = vsrc->y; 
        vdst->z = vsrc->z;
    }

    void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0, VECTOR3D_PTR normal, int normalize = 0);
}
