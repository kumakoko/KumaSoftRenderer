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
// 矩阵类，以行优先的方式存储
#pragma once

#include <cstdlib>
#include <cstring>

namespace KSR
{
    // 1x2 matrix
    typedef struct MATRIX1X2_TYP
    {
        union
        {
            float M[2];
            struct
            {
                float M00, M01;
            };
        };
    } MATRIX1X2, * MATRIX1X2_PTR;

    // 1x3 matrix
    typedef struct MATRIX1X3_TYP
    {
        union
        {
            float M[3];

            struct
            {
                float M00, M01, M02;

            };
        };
    } MATRIX1X3, * MATRIX1X3_PTR;

    // 1x4 matrix
    typedef struct MATRIX1X4_TYP
    {
        union
        {
            float M[4];
            struct
            {
                float M00, M01, M02, M03;
            };
        };
    } MATRIX1X4, * MATRIX1X4_PTR;

    // 2x2 matrix
    typedef struct MATRIX2X2_TYP
    {
        union
        {
            float M[2][2];
            struct
            {
                float M00, M01;
                float M10, M11;
            };
        };
    } MATRIX2X2, * MATRIX2X2_PTR;

    // 3x2 matrix 
    typedef struct MATRIX3X2_TYP
    {
        union
        {
            float M[3][2];
            struct
            {
                float M00, M01;
                float M10, M11;
                float M20, M21;
            };
        };
    } MATRIX3X2, * MATRIX3X2_PTR;

    typedef struct MATRIX3X3_TYP
    {
        union
        {
            float M[3][3];
            struct
            {
                float M00, M01, M02;
                float M10, M11, M12;
                float M20, M21, M22;
            };
        };
    } MATRIX3X3, * MATRIX3X3_PTR;

    // 4x3 matrix
    typedef struct MATRIX4X3_TYP
    {
        union
        {
            float M[4][3];
            struct
            {
                float M00, M01, M02;
                float M10, M11, M12;
                float M20, M21, M22;
                float M30, M31, M32;
            };
        };
    } MATRIX4X3, * MATRIX4X3_PTR;


    //4x4 matrix
    typedef struct MATRIX4X4_TYP
    {
        union
        {
#if defined _MSC_VER
            __declspec(align(16)) float M[4][4];
#else
            float M[4][4];
#endif
            struct
            {
                float M00, M01, M02, M03;
                float M10, M11, M12, M13;
                float M20, M21, M22, M23;
                float M30, M31, M32, M33;
            };
        };
    } MATRIX4X4, * MATRIX4X4_PTR;

    /**************************************************************************************
    P237:
    将1x2矩阵，实质上是一个2D点，与一个3x2矩阵相乘，因为1x2的列数与3x2矩阵的行数不同，实质上是
    不能直接计算，因此，会将1x2矩阵扩展为1x3矩阵，且新增的那个列的值为1，这样子可以进行计算
    @name: KSR::Mat_Mul_1X2_3X2
    @return: int
    @param: MATRIX1X2_PTR ma
    @param: MATRIX3X2_PTR mb
    @param: MATRIX1X2_PTR mprod
    *************************************************************************************/
    int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma, MATRIX3X2_PTR mb, MATRIX1X2_PTR mprod);

    //1x3矩阵，乘以3x3矩阵。然后将值返回到一个1x3矩阵上
    int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma, MATRIX3X3_PTR mb, MATRIX1X3_PTR mprod);

    void Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod);

    /**************************************************************************************
    4x4矩阵ma乘以4x4矩阵mb，将将结果存储在mprod中
    @name: KSR::Mat_Mul_4X4
    @return: void
    @param: MATRIX4X4_PTR ma
    @param: MATRIX4X4_PTR mb
    @param: MATRIX4X4_PTR mprod
    *************************************************************************************/
    void Mat_Mul_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod);

    /**************************************************************************************

    @name: KSR::Mat_Init_3X2
    @return: void
    @param: MATRIX3X2_PTR ma
    @param: float m00
    @param: float m01
    @param: float m10
    @param: float m11
    @param: float m20
    @param: float m21
    *************************************************************************************/
    void Mat_Init_3X2(MATRIX3X2_PTR ma, float m00, float m01, float m10, float m11, float m20, float m21);

    /**************************************************************************************

    @name: KSR::Mat_Init_4X4
    @return: void
    @param: MATRIX4X4_PTR ma
    @param: float m00
    @param: float m01
    @param: float m02
    @param: float m03
    @param: float m10
    @param: float m11
    @param: float m12
    @param: float m13
    @param: float m20
    @param: float m21
    @param: float m22
    @param: float m23
    @param: float m30
    @param: float m31
    @param: float m32
    @param: float m33
    *************************************************************************************/
    void Mat_Init_4X4(MATRIX4X4_PTR ma,
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33);

    // 4x4 identity matrix
    extern const MATRIX4X4 IMAT_4X4;

    // 4x3 identity matrix (note this is not correct mathematically)
    // but later we may use 4x3 matrices with the assumption that 
    // the last column is always [0 0 0 1]t
    extern const MATRIX4X3 IMAT_4X3;


    // 3x3 identity matrix
    extern const MATRIX3X3 IMAT_3X3;

    // 2x2 identity matrix
    extern const MATRIX2X2 IMAT_2X2;

    /**************************************************************************************
    
    @name: KSR::MAT_IDENTITY_2X2
    @return: void
    @param: MATRIX2X2 * m
    *************************************************************************************/
    inline void MAT_IDENTITY_2X2(MATRIX2X2* m)
    {
        std::memcpy(m, &IMAT_2X2, sizeof(MATRIX2X2));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_IDENTITY_3X3
    @return: void
    @param: MATRIX3X3 * m
    *************************************************************************************/
    inline void MAT_IDENTITY_3X3(MATRIX3X3* m)
    {
        std::memcpy(m, &IMAT_3X3, sizeof(MATRIX3X3));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_IDENTITY_4X4
    @return: void
    @param: MATRIX4X4 * m
    *************************************************************************************/
    inline void MAT_IDENTITY_4X4(MATRIX4X4* m)
    {
        std::memcpy(m, &IMAT_4X4, sizeof(MATRIX4X4));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_IDENTITY_4X3
    @return: void
    @param: MATRIX4X3 * m
    *************************************************************************************/
    inline void MAT_IDENTITY_4X3(MATRIX4X3* m)
    {
        std::memcpy(m, &IMAT_4X3, sizeof(MATRIX4X3));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_COPY_2X2
    @return: void
    @param: MATRIX2X2 * src_mat
    @param: MATRIX2X2 * dest_mat
    *************************************************************************************/
    inline void MAT_COPY_2X2(MATRIX2X2* src_mat, MATRIX2X2* dest_mat)
    {
        std::memcpy(dest_mat, src_mat, sizeof(MATRIX2X2));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_COPY_3X3
    @return: void
    @param: MATRIX3X3 * src_mat
    @param: MATRIX3X3 * dest_mat
    *************************************************************************************/
    inline void MAT_COPY_3X3(MATRIX3X3* src_mat, MATRIX3X3* dest_mat)
    {
        std::memcpy(dest_mat, src_mat, sizeof(MATRIX3X3));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_COPY_4X4
    @return: void
    @param: MATRIX4X4 * src_mat
    @param: MATRIX4X4 * dest_mat
    *************************************************************************************/
    inline void MAT_COPY_4X4(MATRIX4X4* src_mat, MATRIX4X4* dest_mat)
    {
        std::memcpy(dest_mat, src_mat, sizeof(MATRIX4X4));
    }

    /**************************************************************************************
    
    @name: KSR::MAT_COPY_4X3
    @return: void
    @param: MATRIX4X3 * src_mat
    @param: MATRIX4X3 * dest_mat
    *************************************************************************************/
    inline void MAT_COPY_4X3(MATRIX4X3* src_mat, MATRIX4X3* dest_mat)
    {
        std::memcpy(dest_mat, src_mat, sizeof(MATRIX4X3));
    }

    /**************************************************************************************
    
    @name: KSR::Build_XYZ_Rotation_MATRIX4X4
    @return: void
    @param: float theta_x
    @param: float theta_y
    @param: float theta_z
    @param: MATRIX4X4_PTR mrot
    *************************************************************************************/
    void Build_XYZ_Rotation_MATRIX4X4(float theta_x, float theta_y, float theta_z, MATRIX4X4_PTR mrot);
}
