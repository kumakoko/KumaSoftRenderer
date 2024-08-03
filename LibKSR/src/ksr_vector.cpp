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

#if defined(_MSC_VER)
#include <xmmintrin.h>
#include <pmmintrin.h>
#endif
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

    ///

    void VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum)
    {
        // this function adds va+vb and return it in vsum
        vsum->x = va->x + vb->x;
        vsum->y = va->y + vb->y;
        vsum->z = va->z + vb->z;
        vsum->w = 1;

    } // end VECTOR4D_Add



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



    void VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff)
    {
        // this function subtracts va-vb and return it in vdiff
        // the stack
        vdiff->x = va->x - vb->x;
        vdiff->y = va->y - vb->y;
        vdiff->z = va->z - vb->z;
        vdiff->w = 1;

    } // end VECTOR4D_Sub



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

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va)
    {
        va->x *= k;
        va->y *= k;
        va->z *= k;
        va->w = 1.0f;
    }

    void VECTOR4D_Scale(float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled)
    {
#if defined(_MSC_VER)
        __m128 vecA = _mm_loadu_ps(va->M);// Load the vector into an SSE register
        __m128 scale = _mm_set1_ps(k); // Load the scalar k into an SSE register, replicated across all elements
        __m128 scaledVec = _mm_mul_ps(vecA, scale);// Perform the scaling by multiplying the vector with the scalar
        scaledVec = _mm_move_ss(scaledVec, _mm_set_ss(1.0f));// Set the w component to 1.0f manually
        _mm_storeu_ps(vscaled->M, scaledVec);  // Store the result back to the output vector
#else
        // this function scales a vector by the constant k,
        // leaves the original unchanged, and returns the result
        // in vres as well as on the stack

        // multiply each component by scaling factor
        vscaled->x = k * va->x;
        vscaled->y = k * va->y;
        vscaled->z = k * va->z;
        vscaled->w = 1;
#endif
    } // end VECTOR4D_Scale

    float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
#if defined(_MSC_VER)
        __m128 a = _mm_load_ps(va->M);  // 用_mm_load_ps加载va和vb到128位SIMD寄存器中,要求va和vb都是16字节对齐。
        __m128 b = _mm_load_ps(vb->M);
        __m128 zero_w = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
        a = _mm_mul_ps(a, zero_w);  // Zero out w component of a
        b = _mm_mul_ps(b, zero_w);  // Zero out w component of b
        __m128 result = _mm_mul_ps(a, b); // _mm_mul_ps(a, b)对两个向量进行元素乘法，得到一个包含乘积的向量。

        // Perform horizontal addition to get the dot product
        result = _mm_hadd_ps(result, result);  // _mm_hadd_ps()用于执行水平加法。它将向量中相邻的元素对相加。
        result = _mm_hadd_ps(result, result);  // 第一次调用将相邻元素相加，第二次调用将这些和相加，得到一个点积值。

        
        //__m128 result = _mm_mul_ps(a, b) : [a1 a2 a3 a4] [b1 b2 b3 b4] 各元素分量两两相乘，得到result = [a1b1 a2b2 a3b3 a4b4]
        // result = _mm_hadd_ps(result, result)： [a1b1 a2b2 a3b3 a4b4] [a1b1 a2b2 a3b3 a4b4] 共四组八个元素依次两两相加：得到result = [a1b1+a2b2 a3b3+a4b4 a1b1+a2b2 a3b3+a4b4]
        // 再来一次 result = _mm_hadd_ps(result, result)：得到：[a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4]
        
        // Extracts the lower 32-bit single-precision floating-point value from a 128-bit SSE register.
        return _mm_cvtss_f32(result);
#else
        return va->x * vb->x + va->y * vb->y + va->z * vb->z;
#endif
    }

    void VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vn)
    {
        // this function computes the cross product between va and vb
        // and returns the vector that is perpendicular to each in vn
        vn->x = ((va->y * vb->z) - (va->z * vb->y));
        vn->y = -((va->x * vb->z) - (va->z * vb->x));
        vn->z = ((va->x * vb->y) - (va->y * vb->x));
        vn->w = 1;
    }

    VECTOR4D VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb)
    {
        // this function computes the cross product between va and vb
        // and returns the vector that is perpendicular to each

        VECTOR4D vn;

        vn.x = va->y * vb->z - va->z * vb->y;
        vn.y = -(va->x * vb->z) - va->z * vb->x;
        vn.z = va->x * vb->y - va->y * vb->x;
        vn.w = 1.0f;

        return(vn);
    } 

    float VECTOR4D_Length(VECTOR4D_PTR va)
    {
        return sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
    }

    //

    float VECTOR4D_Length_Fast(VECTOR4D_PTR va)
    {
        // computes the magnitude of a vector using an approximation
        // very fast
        return(Fast_Distance_3D(va->x, va->y, va->z));

    } // end VECTOR4D_Length_Fast

    //

    void VECTOR4D_Normalize(VECTOR4D_PTR va)
    {

        float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

        if (length < EPSILON_E5)
            return;

        float length_inv = 1.0f / length;


        va->x *= length_inv;
        va->y *= length_inv;
        va->z *= length_inv;
        va->w = 1.0f;

    } // end VECTOR4D_Normalize

    //

    void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn)
    {
        // normalizes the sent vector and returns the result in vn

        VECTOR4D_ZERO(vn);

#if defined(_MSC_VER)
        // Load the vector into an SSE register
        __m128 vecA = _mm_loadu_ps(va->M);

        // Calculate the dot product of the vector with itself
        __m128 dotProduct = _mm_mul_ps(vecA, vecA);
        dotProduct = _mm_hadd_ps(dotProduct, dotProduct);
        dotProduct = _mm_hadd_ps(dotProduct, dotProduct);

        // Compute the square root of the dot product to get the length
        __m128 length = _mm_sqrt_ss(dotProduct);

        // Check if the length is less than EPSILON_E5
        float len = _mm_cvtss_f32(length);
        if (len < EPSILON_E5) {
            return;  // Zero length vector, do nothing
        }

        // Calculate the inverse of the length
        __m128 lengthInv = _mm_rcp_ss(length);

        // Broadcast the inverse length across all elements
        lengthInv = _mm_shuffle_ps(lengthInv, lengthInv, _MM_SHUFFLE(0, 0, 0, 0));

        // Normalize the vector
        __m128 normalizedVec = _mm_mul_ps(vecA, lengthInv);

        // Set the w component to 1.0f
        normalizedVec = _mm_move_ss(normalizedVec, _mm_set_ss(1.0f));

        // Store the normalized vector back to vn
        _mm_storeu_ps(vn->M, normalizedVec);
#else

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
#endif
    } // end VECTOR4D_Normalize
}