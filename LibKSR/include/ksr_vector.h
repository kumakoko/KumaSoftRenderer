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

namespace KSR
{
    typedef struct VECTOR2D_TYP
    {
        union
        {
#if defined(_MSC_VER)
            __declspec(align(16)) float M[2];
#else
            float M[2];
#endif
            struct
            {
                float x, y;
            };
        };
    } VECTOR2D, POINT2D, * VECTOR2D_PTR, * POINT2D_PTR;

    typedef struct VECTOR3D_TYP
    {
        union
        {
#if defined(_MSC_VER)
            __declspec(align(16)) float M[3];
#else
            float M[3];
#endif
            struct
            {
                float x, y, z;
            };
        };
    } VECTOR3D, POINT3D, *VECTOR3D_PTR, *POINT3D_PTR;

    typedef struct VECTOR4D_TYP
    {
        union
        {
#if defined(_MSC_VER)
            __declspec(align(16)) float M[4];
#else
            float M[4];
#endif
            struct
            {
                float x, y, z, w;
            };
        };
    }VECTOR4D, POINT4D, *VECTOR4D_PTR, *POINT4D_PTR;

    inline void VECTOR2D_ZERO(VECTOR2D_PTR v)
    {
        v->x = v->y = 0.0f;
    }

    inline void VECTOR3D_ZERO(VECTOR3D_PTR v)
    {
        v->x = v->y = v->z = 0.0f;
    }

    inline void VECTOR4D_ZERO(VECTOR4D_PTR v)
    {
        v->x = v->y = v->z = 0.0; v->w = 1.0;
    }

    // macros to initialize vectors with explicit components
    inline void VECTOR2D_INITXY(VECTOR2D_PTR v, float x, float y)
    {
        v->x = x; v->y = y;
    }

    inline void VECTOR3D_INITXYZ(VECTOR3D_PTR v, float x, float y, float z)
    {
        v->x = x; v->y = y; v->z = z;
    }

    /**************************************************************************************

    @name: KSR::VECTOR4D_INITXYZ
    @return: void
    @param: VECTOR4D_PTR v
    @param: float x
    @param: float y
    @param: float z
    *************************************************************************************/
    inline void VECTOR4D_INITXYZ(VECTOR4D_PTR v, float x, float y, float z)
    {
        v->x = x; v->y = y; v->z = z; v->w = 1.0;
    }

    /**************************************************************************************

    @name: KSR::VECTOR2D_COPY
    @return: void
    @param: VECTOR2D_PTR vdst
    @param: VECTOR2D_PTR vsrc
    *************************************************************************************/
    inline void VECTOR2D_COPY(VECTOR2D_PTR vdst, VECTOR2D_PTR vsrc)
    {
       vdst->x =vsrc->x;vdst->y =vsrc->y;
    }

    /**************************************************************************************

    @name: KSR::VECTOR3D_COPY
    @return: void
    @param: VECTOR3D_PTR vdst
    @param: VECTOR3D_PTR vsrc
    *************************************************************************************/
    inline void VECTOR3D_COPY(VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc)
    {
       vdst->x =vsrc->x;vdst->y =vsrc->y; vdst->z =vsrc->z;
    }

    /**************************************************************************************

    @name: KSR::VECTOR3D_Length
    @return: float
    @param: VECTOR3D_PTR va
    *************************************************************************************/
    float VECTOR3D_Length(VECTOR3D_PTR va);

    /**************************************************************************************

    @name: KSR::VECTOR3D_Normalize
    @return: void
    @param: VECTOR3D_PTR va
    *************************************************************************************/
    void VECTOR3D_Normalize(VECTOR3D_PTR va);

    /**************************************************************************************

    @name: KSR::VECTOR3D_Normalize
    @return: void
    @param: VECTOR3D_PTR va
    @param: VECTOR3D_PTR vn
    *************************************************************************************/
    void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn);

    /**************************************************************************************

    @name: KSR::Fast_Distance_3D
    @return: float
    @param: float fx
    @param: float fy
    @param: float fz
    *************************************************************************************/
    float Fast_Distance_3D(float fx, float fy, float fz);

    // ---------------------- vector 4d ---------------------- //

    /**************************************************************************************

    @name: KSR::VECTOR4D_COPY
    @return: void
    @param: VECTOR4D_PTR vdst
    @param: VECTOR4D_PTR vsrc
    *************************************************************************************/
    inline void VECTOR4D_COPY(VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc)
    {
        vdst->x = vsrc->x; vdst->y = vsrc->y; vdst->z = vsrc->z; vdst->w = vsrc->w;
    }

    /**************************************************************************************
     
     * @name: VECTOR4D_Build
     * @return: void
     * @param: VECTOR4D_PTR init
     * @param: VECTOR4D_PTR term
     * @param: VECTOR4D_PTR result
     *************************************************************************************/
     void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result);

    /**************************************************************************************
     
     * @name: VECTOR4D_Add
     * @return: void
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vb
     * @param: VECTOR4D_PTR vsum
     *************************************************************************************/
     void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Add
    @return: KSR::VECTOR4D
    @param: VECTOR4D_PTR va
    @param: VECTOR4D_PTR vb
    *************************************************************************************/
    VECTOR4D VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    /**************************************************************************************
     
     * @name: VECTOR4D_Sub
     * @return: void
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vb
     * @param: VECTOR4D_PTR vdiff
     *************************************************************************************/
     void VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff);

    /**************************************************************************************
     
     * @name: VECTOR4D_Sub
     * @return: KSR::VECTOR4D
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vb
     *************************************************************************************/
     VECTOR4D VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    /**************************************************************************************
     
     * @name: VECTOR4D_Scale
     * @return: void
     * @param: float k
     * @param: VECTOR4D_PTR va
     *************************************************************************************/
     void VECTOR4D_Scale(float k, VECTOR4D_PTR va);

    /**************************************************************************************
     
     * @name: VECTOR4D_Scale
     * @return: void
     * @param: float k
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vscaled
     *************************************************************************************/
     void VECTOR4D_Scale(float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled);

    /**************************************************************************************
     
     * @name: VECTOR4D_Dot
     * @return: float
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vb
     *************************************************************************************/
     float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    /**************************************************************************************
     
     * @name: VECTOR4D_Cross
     * @return: void
     * @param: VECTOR4D_PTR va
     * @param: VECTOR4D_PTR vb
     * @param: VECTOR4D_PTR vn
     *************************************************************************************/
     void VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vn);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Cross
    @return: KSR::VECTOR4D
    @param: VECTOR4D_PTR va
    @param: VECTOR4D_PTR vb
    *************************************************************************************/
    VECTOR4D VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Length
    @return: float
    @param: VECTOR4D_PTR va
    *************************************************************************************/
    float VECTOR4D_Length(VECTOR4D_PTR va);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Length_Fast
    @return: float
    @param: VECTOR4D_PTR va
    *************************************************************************************/
    float VECTOR4D_Length_Fast(VECTOR4D_PTR va);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Normalize
    @return: void
    @param: VECTOR4D_PTR va
    *************************************************************************************/
    void VECTOR4D_Normalize(VECTOR4D_PTR va);

    /**************************************************************************************

    @name: KSR::VECTOR4D_Normalize
    @return: void
    @param: VECTOR4D_PTR va
    @param: VECTOR4D_PTR vn
    *************************************************************************************/
    void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn);
}