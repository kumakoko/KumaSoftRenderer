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

    typedef struct QUAT_TYP
    {
        union
        {
            float M[4]; // array indexed storage w,x,y,z order

            // vector part, real part format
            struct
            {
                float    q0;  // the real part
                VECTOR3D qv;  // the imaginary part xi+yj+zk
            };
            struct
            {
                float w, x, y, z;
            };
        }; // end union

    } QUAT, * QUAT_PTR;

    void QUAT_Add(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum);

    void QUAT_Sub(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff);

    void QUAT_Conjugate(QUAT_PTR q, QUAT_PTR qconj);

    void QUAT_Scale(QUAT_PTR q, float scale, QUAT_PTR qs);

    void QUAT_Scale(QUAT_PTR q, float scale);

    float QUAT_Norm(QUAT_PTR q);

    /**************************************************************************************
    
    @name: KSR::QUAT_Norm2
    @return: float
    @param: QUAT_PTR q
    *************************************************************************************/
    float QUAT_Norm2(QUAT_PTR q);

    /**************************************************************************************
    
    @name: KSR::QUAT_Normalize
    @return: void
    @param: QUAT_PTR q
    @param: QUAT_PTR qn
    *************************************************************************************/
    void QUAT_Normalize(QUAT_PTR q, QUAT_PTR qn);

    /**************************************************************************************
    
    @name: KSR::QUAT_Normalize
    @return: void
    @param: QUAT_PTR q
    *************************************************************************************/
    void QUAT_Normalize(QUAT_PTR q);

    void QUAT_Unit_Inverse(QUAT_PTR q, QUAT_PTR qi);

    void QUAT_Unit_Inverse(QUAT_PTR q);

    void QUAT_Inverse(QUAT_PTR q, QUAT_PTR qi);

    void QUAT_Inverse(QUAT_PTR q);

    void QUAT_Mul(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod);

    void QUAT_Triple_Product(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3, QUAT_PTR qprod);

    void VECTOR3D_Theta_To_QUAT(QUAT_PTR q, VECTOR3D_PTR v, float theta);

    void VECTOR4D_Theta_To_QUAT(QUAT_PTR q, VECTOR4D_PTR v, float theta);

    void EulerZYX_To_QUAT(QUAT_PTR q, float theta_z, float theta_y, float theta_x);

    void QUAT_To_VECTOR3D_Theta(QUAT_PTR q, VECTOR3D_PTR v, float* theta);

    void QUAT_Print(QUAT_PTR q, const char* name = "q");

}