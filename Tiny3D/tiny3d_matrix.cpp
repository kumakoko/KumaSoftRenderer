#if defined(_MSC_VER)
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#endif

#include <cmath>

#include "tiny3d_matrix.h"

// c = a + b
void T3dMatrixAdd(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b)
{
#if defined(_MSC_VER)
    __m128* c_rows = (__m128*)c->m;
    const __m128* a_rows = (const __m128*)a->m;
    const __m128* b_rows = (const __m128*)b->m;

    // Add each row using SSE
    c_rows[0] = _mm_add_ps(a_rows[0], b_rows[0]);
    c_rows[1] = _mm_add_ps(a_rows[1], b_rows[1]);
    c_rows[2] = _mm_add_ps(a_rows[2], b_rows[2]);
    c_rows[3] = _mm_add_ps(a_rows[3], b_rows[3]);
#else
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            c->m[i][j] = a->m[i][j] + b->m[i][j];
    }
#endif
}

// c = a - b
void T3DMatrixSubtract(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b)
{
#if defined(_MSC_VER)
    __m128* c_rows = (__m128*)c->m;
    const __m128* a_rows = (const __m128*)a->m;
    const __m128* b_rows = (const __m128*)b->m;
    c_rows[0] = _mm_sub_ps(a_rows[0], b_rows[0]);
    c_rows[1] = _mm_sub_ps(a_rows[1], b_rows[1]);
    c_rows[2] = _mm_sub_ps(a_rows[2], b_rows[2]);
    c_rows[3] = _mm_sub_ps(a_rows[3], b_rows[3]);
#else
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            c->m[i][j] = a->m[i][j] - b->m[i][j];
    }
#endif
}

// c = a * b
void T3DMatrixMultiply(T3DMatrix4X4* c, const T3DMatrix4X4* a, const T3DMatrix4X4* b)
{
#if defined(_MSC_VER)
    T3DMatrix4X4 z;

    for (int i = 0; i < 4; i++)
    {
        // Load the i-th column of matrix b
        __m128 b_col = _mm_set_ps(b->m[3][i], b->m[2][i], b->m[1][i], b->m[0][i]);

        // For each row of matrix a
        for (int j = 0; j < 4; j++)
        {
            // Load the j-th row of matrix a
            __m128 a_row = _mm_load_ps(&a->m[j][0]);

            // Multiply and add using SSE
            __m128 result = _mm_mul_ps(a_row, b_col);

            // Horizontal add to get final result
            result = _mm_hadd_ps(result, result);
            result = _mm_hadd_ps(result, result);

            // Store the result
            _mm_store_ss(&z.m[j][i], result);
        }
    }

    *c = z;
#else
    T3DMatrix4X4 z;
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
                (a->m[j][1] * b->m[1][i]) +
                (a->m[j][2] * b->m[2][i]) +
                (a->m[j][3] * b->m[3][i]);
        }
    }

    c[0] = z;
#endif
}

// c = a * f
void T3DMatrixScale(T3DMatrix4X4* c, const T3DMatrix4X4* a, float f)
{
#if defined(_MSC_VER)
    __m128 scale = _mm_set1_ps(f);

    for (int i = 0; i < 4; i++)
    {
        __m128 row = _mm_load_ps(a->m[i]);
        __m128 result = _mm_mul_ps(row, scale);
        _mm_store_ps(c->m[i], result);
    }
#else
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            c->m[i][j] = a->m[i][j] * f;
    }
#endif
}

// y = x * m
void T3DMatrixApply(T3DVector4* rv, const T3DVector4* v, const T3DMatrix4X4* m)
{
#if defined(_MSC_VER)
    __m128 vec = _mm_set_ps(v->w, v->z, v->y, v->x);

    __m128 row0 = _mm_load_ps(m->m[0]);
    __m128 row1 = _mm_load_ps(m->m[1]);
    __m128 row2 = _mm_load_ps(m->m[2]);
    __m128 row3 = _mm_load_ps(m->m[3]);

    __m128 result;
    result = _mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(0, 0, 0, 0)), row0);
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(1, 1, 1, 1)), row1));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(2, 2, 2, 2)), row2));
    result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3, 3, 3, 3)), row3));

    _mm_store_ps(&rv->x, result);
#else
    float X = v->x, Y = v->y, Z = v->z, W = v->w;
    rv->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
    rv->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
    rv->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
    rv->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
#endif
}

void T3DMatrixIdentity(T3DMatrix4X4* m)
{
#if defined(_MSC_VER)
    __m128 zero = _mm_setzero_ps();
    __m128 one = _mm_set_ps(1.0f, 0.0f, 0.0f, 1.0f);

    _mm_store_ps(m->m[0], _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f));
    _mm_store_ps(m->m[1], _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f));
    _mm_store_ps(m->m[2], _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f));
    _mm_store_ps(m->m[3], _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f));
#else
    m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
    m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
    m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
    m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
    m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
#endif
}

void T3DMatrixSetZero(T3DMatrix4X4* m)
{
#if defined(_MSC_VER)
    __m128 zero = _mm_setzero_ps();
    _mm_store_ps(m->m[0], zero);
    _mm_store_ps(m->m[1], zero);
    _mm_store_ps(m->m[2], zero);
    _mm_store_ps(m->m[3], zero);
#else
    m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
    m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
    m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
    m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
#endif
}

// 平移变换
void T3DMatrixMakeTranslation(T3DMatrix4X4* m, float x, float y, float z)
{
    T3DMatrixIdentity(m);
#if defined(_MSC_VER)
    __m128 trans = _mm_set_ps(1.0f, z, y, x);
    _mm_store_ps(m->m[3], trans);
#else
    m->m[3][0] = x;
    m->m[3][1] = y;
    m->m[3][2] = z;
#endif
}

// 缩放变换
void T3DMatrixMakeScaling(T3DMatrix4X4* m, float x, float y, float z)
{
#if defined(_MSC_VER)
    T3DMatrixSetZero(m);
    m->m[0][0] = x;
    m->m[1][1] = y;
    m->m[2][2] = z;
    m->m[3][3] = 1.0f;
#else
    T3DMatrixIdentity(m);
    m->m[0][0] = x;
    m->m[1][1] = y;
    m->m[2][2] = z;
#endif
}

// 旋转矩阵
void T3DMatrixMakeRotation(T3DMatrix4X4* m, float x, float y, float z, float theta)
{
    float qsin = sinf(theta * 0.5f);
    float qcos = cosf(theta * 0.5f);
    T3DVector4 vec = { x, y, z, 1.0f };
    float w = qcos;
    T3DVector4Normalize(&vec);
    x = vec.x * qsin;
    y = vec.y * qsin;
    z = vec.z * qsin;

#if defined(_MSC_VER)
    __m128 row0 = _mm_set_ps(0.0f, 2 * x * z - 2 * w * y, 2 * x * y + 2 * w * z, 1 - 2 * y * y - 2 * z * z);
    __m128 row1 = _mm_set_ps(0.0f, 2 * y * z + 2 * w * x, 1 - 2 * x * x - 2 * z * z, 2 * x * y - 2 * w * z);
    __m128 row2 = _mm_set_ps(0.0f, 1 - 2 * x * x - 2 * y * y, 2 * y * z - 2 * w * x, 2 * x * z + 2 * w * y);
    __m128 row3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

    _mm_store_ps(m->m[0], row0);
    _mm_store_ps(m->m[1], row1);
    _mm_store_ps(m->m[2], row2);
    _mm_store_ps(m->m[3], row3);
#else
    m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
    m->m[1][0] = 2 * x * y - 2 * w * z;
    m->m[2][0] = 2 * x * z + 2 * w * y;
    m->m[0][1] = 2 * x * y + 2 * w * z;
    m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
    m->m[2][1] = 2 * y * z - 2 * w * x;
    m->m[0][2] = 2 * x * z - 2 * w * y;
    m->m[1][2] = 2 * y * z + 2 * w * x;
    m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
    m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
    m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
    m->m[3][3] = 1.0f;
#endif
}

// 设置摄像机
void T3DMatrixMakeLookat(T3DMatrix4X4* m, const T3DVector4* eye, const T3DVector4* at, const T3DVector4* up)
{
    T3DVector4 xaxis, yaxis, zaxis;

    T3DVector4Subtract(&zaxis, at, eye);
    T3DVector4Normalize(&zaxis);
    T3DVector4Cross(&xaxis, up, &zaxis);
    T3DVector4Normalize(&xaxis);
    T3DVector4Cross(&yaxis, &zaxis, &xaxis);

    m->m[0][0] = xaxis.x;
    m->m[1][0] = xaxis.y;
    m->m[2][0] = xaxis.z;
    m->m[3][0] = -T3DVector4Dot(&xaxis, eye);

    m->m[0][1] = yaxis.x;
    m->m[1][1] = yaxis.y;
    m->m[2][1] = yaxis.z;
    m->m[3][1] = -T3DVector4Dot(&yaxis, eye);

    m->m[0][2] = zaxis.x;
    m->m[1][2] = zaxis.y;
    m->m[2][2] = zaxis.z;
    m->m[3][2] = -T3DVector4Dot(&zaxis, eye);

    m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
    m->m[3][3] = 1.0f;
}

// D3DXMatrixPerspectiveFovLH
void T3DMatrixPerspective(T3DMatrix4X4* m, float fovy, float aspect, float zn, float zf)
{
    float fax = 1.0f / (float)tanf(fovy * 0.5f);
    T3DMatrixSetZero(m);
#if defined(_MSC_VER)
    __m128 row0 = _mm_set_ps(0.0f, 0.0f, 0.0f, fax / aspect);
    __m128 row1 = _mm_set_ps(0.0f, 0.0f, fax, 0.0f);
    __m128 row2 = _mm_set_ps(1.0f, zf / (zf - zn), 0.0f, 0.0f);
    __m128 row3 = _mm_set_ps(0.0f, -zn * zf / (zf - zn), 0.0f, 0.0f);
    _mm_store_ps(m->m[0], row0);
    _mm_store_ps(m->m[1], row1);
    _mm_store_ps(m->m[2], row2);
    _mm_store_ps(m->m[3], row3);
#else
    m->m[0][0] = static_cast<float>(fax / aspect);
    m->m[1][1] = static_cast<float>(fax);
    m->m[2][2] = zf / (zf - zn);
    m->m[3][2] = -zn * zf / (zf - zn);
    m->m[2][3] = 1;
#endif
}