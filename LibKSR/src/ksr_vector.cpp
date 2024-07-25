#include <cmath>
#include <utility>
#include "ksr_vector.h"
#include "ksr_constants.h"

namespace KSR
{
    float VECTOR3D_Length(VECTOR3D_PTR va)
    {
        return sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
    }

    void VECTOR3D_Normalize(VECTOR3D_PTR va)
    {
        // normalizes the sent vector in placew

        // compute length
        float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

        // test for zero length vector
        // if found return zero vector
        if (length < EPSILON_E5)
            return;

        float length_inv = 1 / length;

        // compute normalized version of vector
        va->x *= length_inv;
        va->y *= length_inv;
        va->z *= length_inv;

    }

    void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn)
    {
        // normalizes the sent vector and returns the result in vn

        VECTOR3D_ZERO(vn);

        // compute length
        float length = VECTOR3D_Length(va);

        // test for zero length vector
        // if found return zero vector
        if (length < EPSILON_E5)
            return;

        float length_inv = 1.0f / length;

        // compute normalized version of vector
        vn->x = va->x * length_inv;
        vn->y = va->y * length_inv;
        vn->z = va->z * length_inv;

    } // end VECTOR3D_Normalize


    float Fast_Distance_3D(float fx, float fy, float fz)
    {
        // this function computes the distance from the origin to x,y,z


        int x, y, z; // used for algorithm

        // make sure values are all positive
        x = static_cast<int>(fabs(fx) * 1024);
        y = static_cast<int>(fabs(fy) * 1024);
        z = static_cast<int>(fabs(fz) * 1024);

        // sort values
        if (y < x) std::swap(x, y);

        if (z < y) std::swap(y, z);

        if (y < x) std::swap(x, y);

                    int dist = (z + 11 * (y >> 5) + (x >> 2));

        // compute distance with 8% error
        return((float)(dist >> 10));

    } // end Fast_Distance_3D

    // ---------------------- vector 4d ---------------------- //

// these are the 4D version of the vector functions, they
// assume that the vectors are 3D with a w, so w is left
// out of all the operations

    void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result)
    {
        // build a 4d vector
        result->x = term->x - init->x;
        result->y = term->y - init->y;
        result->z = term->z - init->z;
        result->w = 1;

    } // end VECTOR4D_Build

    ////////////////////////////////////////////////////////////////

    void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum)
    {
        // this function adds va+vb and return it in vsum
        vsum->x = va->x + vb->x;
        vsum->y = va->y + vb->y;
        vsum->z = va->z + vb->z;
        vsum->w = 1;

    } // end VECTOR4D_Add

    ////////////////////////////////////////////////////////////

    VECTOR4D VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
        // this function adds va+vb and returns the result on 
        // the stack
        VECTOR4D vsum;

        vsum.x = va->x + vb->x;
        vsum.y = va->y + vb->y;
        vsum.z = va->z + vb->z;
        vsum.w = 1;

        // return result
        return(vsum);

    } // end VECTOR4D_Add

    ////////////////////////////////////////////////////////////

    void VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff)
    {
        // this function subtracts va-vb and return it in vdiff
        // the stack
        vdiff->x = va->x - vb->x;
        vdiff->y = va->y - vb->y;
        vdiff->z = va->z - vb->z;
        vdiff->w = 1;

    } // end VECTOR4D_Sub

    ////////////////////////////////////////////////////////////

    VECTOR4D VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
        // this function subtracts va-vb and returns the result on 
        // the stack
        VECTOR4D vdiff;

        vdiff.x = va->x - vb->x;
        vdiff.y = va->y - vb->y;
        vdiff.z = va->z - vb->z;
        vdiff.w = 1;

        // return result
        return(vdiff);

    } // end VECTOR4D_Sub

    ////////////////////////////////////////////////////////////

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va)
    {
        // this function scales a vector by the constant k,
        // in place , note w is left unchanged

        // multiply each component by scaling factor
        va->x *= k;
        va->y *= k;
        va->z *= k;
        va->w = 1;

    } // end VECTOR4D_Scale

    /////////////////////////////////////////////////////////////

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled)
    {
        // this function scales a vector by the constant k,
        // leaves the original unchanged, and returns the result
        // in vres as well as on the stack

        // multiply each component by scaling factor
        vscaled->x = k * va->x;
        vscaled->y = k * va->y;
        vscaled->z = k * va->z;
        vscaled->w = 1;

    } // end VECTOR4D_Scale

    //////////////////////////////////////////////////////////////

    float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
        // computes the dot product between va and vb
        return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
    } // end VECTOR4D_DOT

    /////////////////////////////////////////////////////////////

    void VECTOR4D_Cross(VECTOR4D_PTR va,
        VECTOR4D_PTR vb,
        VECTOR4D_PTR vn)
    {
        // this function computes the cross product between va and vb
        // and returns the vector that is perpendicular to each in vn

        vn->x = ((va->y * vb->z) - (va->z * vb->y));
        vn->y = -((va->x * vb->z) - (va->z * vb->x));
        vn->z = ((va->x * vb->y) - (va->y * vb->x));
        vn->w = 1;

    } // end VECTOR4D_Cross

    /////////////////////////////////////////////////////////////

    VECTOR4D VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
        // this function computes the cross product between va and vb
        // and returns the vector that is perpendicular to each

        VECTOR4D vn;

        vn.x = ((va->y * vb->z) - (va->z * vb->y));
        vn.y = -((va->x * vb->z) - (va->z * vb->x));
        vn.z = ((va->x * vb->y) - (va->y * vb->x));
        vn.w = 1;

        // return result
        return(vn);

    } // end VECTOR4D_Cross

    //////////////////////////////////////////////////////////////

    float VECTOR4D_Length(VECTOR4D_PTR va)
    {
        // computes the magnitude of a vector, slow

        return(sqrtf(va->x * va->x + va->y * va->y + va->z * va->z));

    } // end VECTOR4D_Length

    ///////////////////////////////////////////////////////////////

    float VECTOR4D_Length_Fast(VECTOR4D_PTR va)
    {
        // computes the magnitude of a vector using an approximation
        // very fast
        return(Fast_Distance_3D(va->x, va->y, va->z));

    } // end VECTOR4D_Length_Fast

    ///////////////////////////////////////////////////////////////

    void VECTOR4D_Normalize(VECTOR4D_PTR va)
    {
        // normalizes the sent vector and returns the result

        // compute length
        float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

        // test for zero length vector
        // if found return zero vector
        if (length < EPSILON_E5)
            return;

        float length_inv = 1.0f / length;

        // compute normalized version of vector
        va->x *= length_inv;
        va->y *= length_inv;
        va->z *= length_inv;
        va->w = 1;

    } // end VECTOR4D_Normalize

    ///////////////////////////////////////////////////////////////

    void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn)
    {
        // normalizes the sent vector and returns the result in vn

        VECTOR4D_ZERO(vn);

        // compute length
        float length = sqrt(va->x * va->x + va->y * va->y + va->z * va->z);

        // test for zero length vector
        // if found return zero vector
        if (length < EPSILON_E5)
            return;

        float length_inv = 1.0f / length;

        // compute normalized version of vector
        vn->x = va->x * length_inv;
        vn->y = va->y * length_inv;
        vn->z = va->z * length_inv;
        vn->w = 1;

    } // end VECTOR4D_Normalize
}