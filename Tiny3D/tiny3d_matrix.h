#pragma once

#include "tiny3d_vector.h"

struct T3DMatrix4X4
{
    union
    {
#if defined(_MSC_VER)
        __declspec(align(16)) float m[4][4];
#else
        float m[4][4];
#endif
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
    };
};

// c = a + b
void T3dMatrixAdd(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b);

// c = a - b
void T3DMatrixSubtract(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b);

// c = a * b
void T3DMatrixMultiply(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b);

// c = a * f
void T3DMatrixScale(T3DMatrix4X4* c, const T3DMatrix4X4* a, float f);

// y = x * m
void T3DMatrixApply(T3DVector4* rv, const T3DVector4* v, const T3DMatrix4X4* m);

void T3DMatrixIdentity(T3DMatrix4X4* m);

void T3DMatrixSetZero(T3DMatrix4X4* m);

// 平移变换
void T3DMatrixMakeTranslation(T3DMatrix4X4* m, float x, float y, float z);

// 缩放变换
void T3DMatrixMakeScaling(T3DMatrix4X4* m, float x, float y, float z);

// 旋转矩阵
void T3DMatrixMakeRotation(T3DMatrix4X4* m, float x, float y, float z, float theta);

// 设置摄像机
void T3DMatrixMakeLookat(T3DMatrix4X4* m, const T3DVector4* eye, const T3DVector4* at, const T3DVector4* up);

// D3DXMatrixPerspectiveFovLH
void T3DMatrixPerspective(T3DMatrix4X4* m, float fovy, float aspect, float zn, float zf);