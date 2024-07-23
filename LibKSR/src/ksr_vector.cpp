#include <cmath>
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
}