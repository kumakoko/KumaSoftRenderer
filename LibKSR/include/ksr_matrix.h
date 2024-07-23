﻿// 矩阵类，以行优先的方式存储
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
            float M[4][4];
            struct
            {
                float M00, M01, M02, M03;
                float M10, M11, M12, M13;
                float M20, M21, M22, M23;
                float M30, M31, M32, M33;
            };
        };
    } MATRIX4X4, * MATRIX4X4_PTR;

    // P237 
    // 将1x2矩阵，实质上是一个2D点，与一个3x2矩阵相乘，因为1x2的列数与3x2矩阵的行数不同，实质上是不能
    // 直接计算，因此，会将1x2矩阵扩展为1x3矩阵，且新增的那个列的值为1，这样子可以进行计算
    int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma, MATRIX3X2_PTR mb, MATRIX1X2_PTR mprod);

    //1x3矩阵，乘以3x3矩阵。然后将值返回到一个1x3矩阵上
    int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma, MATRIX3X3_PTR mb, MATRIX1X3_PTR mprod);

    int Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod);

    inline int Mat_Init_3X2(MATRIX3X2_PTR ma, float m00, float m01, float m10, float m11, float m20, float m21)
    {

        // this function fills a 3x2 matrix with the sent data in row major form
        ma->M[0][0] = m00; ma->M[0][1] = m01;
        ma->M[1][0] = m10; ma->M[1][1] = m11;
        ma->M[2][0] = m20; ma->M[2][1] = m21;

        // return success
        return 1;
    }

    // 4x4 identity matrix
    extern const MATRIX4X4 IMAT_4X4;

    // 4x3 identity matrix (note this is not correct mathematically)
    // but later we may use 4x3 matrices with the assumption that 
    // the last column is always [0 0 0 1]t
    extern const MATRIX4X3 IMAT_4X3;


    // 3x3 identity matrix
    extern const MATRIX3X3 IMAT_3X3;

    // 2x2 identity matrix
    extern  const MATRIX2X2 IMAT_2X2;

    inline void MAT_IDENTITY_2X2(MATRIX2X2* m)
    {
        std::memcpy(m, &IMAT_2X2, sizeof(MATRIX2X2));
    }

    inline void MAT_IDENTITY_3X3(MATRIX3X3* m)
    {
        std::memcpy(m, &IMAT_3X3, sizeof(MATRIX3X3));
    }

    inline void MAT_IDENTITY_4X4(MATRIX4X4* m)
    {
        std::memcpy(m, &IMAT_4X4, sizeof(MATRIX4X4));
    }

    inline void MAT_IDENTITY_4X3(MATRIX4X3* m)
    {
        std::memcpy(m, &IMAT_4X3, sizeof(MATRIX4X3));
    }
}