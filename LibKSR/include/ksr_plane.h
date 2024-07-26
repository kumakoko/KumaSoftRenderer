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
