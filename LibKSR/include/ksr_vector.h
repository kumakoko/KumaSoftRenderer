#pragma once

namespace KSR
{

    // 2D vector, point without the w ////////////////////////
    typedef struct VECTOR2D_TYP
    {
        union
        {
            float M[2]; // array indexed storage

            // explicit names
            struct
            {
                float x, y;
            }; // end struct

        }; // end union

    } VECTOR2D, POINT2D, * VECTOR2D_PTR, * POINT2D_PTR;

    // 3D vector, point without the w ////////////////////////
    typedef struct VECTOR3D_TYP
    {
        union
        {
            float M[3]; // array indexed storage

            // explicit names
            struct
            {
                float x, y, z;
            }; // end struct

        }; // end union

    } VECTOR3D, POINT3D, * VECTOR3D_PTR, * POINT3D_PTR;

    // 4D homogenous vector, point with w ////////////////////
    typedef struct VECTOR4D_TYP
    {
        union
        {
            float M[4]; // array indexed storage

            // explicit names
            struct
            {
                float x, y, z, w;
            }; // end struct
        }; // end union

    } VECTOR4D, POINT4D, * VECTOR4D_PTR, * POINT4D_PTR;

    inline void VECTOR2D_ZERO(VECTOR2D_PTR v)
    {
        (v)->x = (v)->y = 0.0;
    }

    inline void VECTOR3D_ZERO(VECTOR3D_PTR v)
    {
        (v)->x = (v)->y = (v)->z = 0.0;
    }

    inline void VECTOR4D_ZERO(VECTOR4D_PTR v)
    {
        (v)->x = (v)->y = (v)->z = 0.0; (v)->w = 1.0;
    }

    // macros to initialize vectors with explicit components
    inline void VECTOR2D_INITXY(VECTOR2D_PTR v, float x, float y)
    {
        (v)->x = (x); (v)->y = (y);
    }

    inline void VECTOR3D_INITXYZ(VECTOR3D_PTR v, float x, float y, float z)
    {
        (v)->x = (x); (v)->y = (y); (v)->z = (z);
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
        (vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;
    }

    /**************************************************************************************

    @name: KSR::VECTOR3D_COPY
    @return: void
    @param: VECTOR3D_PTR vdst
    @param: VECTOR3D_PTR vsrc
    *************************************************************************************/
    inline void VECTOR3D_COPY(VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc)
    {
        (vdst)->x = (vsrc)->x; (vdst)->y = (vsrc)->y;  (vdst)->z = (vsrc)->z;
    }


    /**************************************************************************************

    @name: KSR::VECTOR3D_Length
    @return: float
    @param: VECTOR3D_PTR va
    *************************************************************************************/
    float VECTOR3D_Length(VECTOR3D_PTR va);

    void VECTOR3D_Normalize(VECTOR3D_PTR va);

    void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn);

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

    void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result);

    void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum);

    VECTOR4D VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    void VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff);

    VECTOR4D VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va);

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled);

    float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    void VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vn);

    VECTOR4D VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb);

    float VECTOR4D_Length(VECTOR4D_PTR va);

    float VECTOR4D_Length_Fast(VECTOR4D_PTR va);

    void VECTOR4D_Normalize(VECTOR4D_PTR va);

    void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn);
}