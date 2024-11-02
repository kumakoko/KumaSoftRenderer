#pragma once

#include "tiny3d_vector.h"

struct matrix_t
{
    union
    {
#if defined(_MSC_VER)
        __declspec(align(16)) float m[4][4];
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
};

// c = a + b
void matrix_add(matrix_t* c, const matrix_t* a, const matrix_t* b);

// c = a - b
void matrix_sub(matrix_t* c, const matrix_t* a, const matrix_t* b);

// c = a * b
void matrix_mul(matrix_t* c, const matrix_t* a, const matrix_t* b);

// c = a * f
void matrix_scale(matrix_t* c, const matrix_t* a, float f);

// y = x * m
void matrix_apply(T3DVector4* y, const T3DVector4* x, const matrix_t* m);

void matrix_set_identity(matrix_t* m);

void matrix_set_zero(matrix_t* m);

// 平移变换
void matrix_set_translate(matrix_t* m, float x, float y, float z);

// 缩放变换
void matrix_set_scale(matrix_t* m, float x, float y, float z);

// 旋转矩阵
void matrix_set_rotate(matrix_t* m, float x, float y, float z, float theta);

// 设置摄像机
void matrix_set_lookat(matrix_t* m, const T3DVector4* eye, const T3DVector4* at, const T3DVector4* up);

// D3DXMatrixPerspectiveFovLH
void matrix_set_perspective(matrix_t* m, float fovy, float aspect, float zn, float zf);