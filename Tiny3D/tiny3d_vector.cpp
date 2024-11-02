/*
* https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2008/4d3eabky(v=vs.90)
* _MM_SHUFFLE 宏的注释如下，假定有128位数m1，m2 从高位到低位，各自可以分解为4个浮点数：
* m1 = a b c d   a在高位 d在低位  编号为依次为 3 2 1 0
* m2 = e f g h   e在高位 h在低位  编号为依次为 3 2 1 0
*
* 那么指令 m3 = _mm_shuffle_ps(m1,m2,_MM_SHUFFLE(1,0,3,2))，得到m3为
*
* m3 = g h a b g在高位，b在低位
*
* 上面的操作步骤是：
* 第1步 取得m2的1位，=> 放到m3的3位，即 g
* 第2步 取得m2的0位，=> 放到m3的2位，即 h
* 第3步 取得m1的3位，=> 放到m3的1位，即 a
* 第4步 取得m2的2位，=> 放到m3的2位，即 b
*
*/

#if defined(_MSC_VER)
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#endif

#include <cmath>

#include "tiny3d_math.h"
#include "tiny3d_vector.h"

float T3DVector4Length(const T3DVector4* v)
{
#if defined(_MSC_VER)
    // Load vector into SSE register
    __m128 vec = _mm_load_ps(v->M);

    // Multiply vector by itself (square components)
    __m128 sq = _mm_mul_ps(vec, vec);

    // Add x,y,z components horizontally (ignore w)
    __m128 sum = _mm_add_ps(_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(0, 0, 0, 0)),
        _mm_add_ps(_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(1, 1, 1, 1)),
            _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 2, 2, 2))));

    // Take square root
    __m128 length = _mm_sqrt_ps(sum);

    // Store result
    float result;
    _mm_store_ss(&result, length);

    return result;
#else
    float sq = v->x * v->x + v->y * v->y + v->z * v->z;
    return static_cast<float>(sqrt(sq));
#endif
}

void T3DVector4Add(T3DVector4* r, const T3DVector4* lhs, const T3DVector4* rhs)
{
#if defined(_MSC_VER)
    __m128 v1 = _mm_load_ps(lhs->M);
    __m128 v2 = _mm_load_ps(rhs->M);
    __m128 result = _mm_sub_ps(v1, v2);
    // Set w component to 1.0f
    //result = _mm_blend_ps(result, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), 0x8);
    _mm_store_ps(r->M, result);
    r->w = 1.0f;
#else
    r->x = lhs->x + rhs->x;
    r->y = lhs->y + rhs->y;
    r->z = lhs->z + rhs->z;
    r->w = 1.0f;
#endif
}

void T3DVector4Subtract(T3DVector4* r, const T3DVector4* lhs, const T3DVector4* rhs)
{
#if defined(_MSC_VER)
    __m128 v1 = _mm_load_ps(lhs->M);
    __m128 v2 = _mm_load_ps(rhs->M);
    __m128 result = _mm_sub_ps(v1, v2);
    // Set w component to 1.0f
    //result = _mm_blend_ps(result, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), 0x8);
    _mm_store_ps(r->M, result);
    r->w = 1.0f;
#else
    r->x = lhs->x - rhs->x;
    r->y = lhs->y - rhs->y;
    r->z = lhs->z - rhs->z;
    r->w = 1.0f;
#endif
}

float T3DVector4Dot(const T3DVector4* x, const T3DVector4* y)
{
#if defined(_MSC_VER)
    __m128 a = _mm_load_ps(x->M);  // 用_mm_load_ps加载va和vb到128位SIMD寄存器中,要求va和vb都是16字节对齐。
    __m128 b = _mm_load_ps(y->M);
    __m128 zero_w = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
    a = _mm_mul_ps(a, zero_w);  // Zero out w component of a
    b = _mm_mul_ps(b, zero_w);  // Zero out w component of b
    __m128 result = _mm_mul_ps(a, b); // _mm_mul_ps(a, b)对两个向量进行元素乘法，得到一个包含乘积的向量。

    // Perform horizontal addition to get the dot product
    result = _mm_hadd_ps(result, result);  // _mm_hadd_ps()用于执行水平加法。它将向量中相邻的元素对相加。
    result = _mm_hadd_ps(result, result);  // 第一次调用将相邻元素相加，第二次调用将这些和相加，得到一个点积值。

    /* 代码解释如下：
    //__m128 result = _mm_mul_ps(a, b) : [a1 a2 a3 a4] [b1 b2 b3 b4] 各元素分量两两相乘，得到result = [a1b1 a2b2 a3b3 a4b4]
    // result = _mm_hadd_ps(result, result)：
    // [a1b1 a2b2 a3b3 a4b4] [a1b1 a2b2 a3b3 a4b4] 共四组八个元素依次两两相加：得到result = [a1b1+a2b2 a3b3+a4b4 a1b1+a2b2 a3b3+a4b4]
    // 再来一次 result = _mm_hadd_ps(result, result)：得到：[a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4 a1b1+a2b2+a3b3+a4b4]
    */
    // Extracts the lower 32-bit single-precision floating-point value from a 128-bit SSE register.
    return _mm_cvtss_f32(result);
#else
    return x->x * y->x + x->y * y->y + x->z * y->z;
#endif
}

void T3DVector4Cross(T3DVector4* r, const T3DVector4* lhs, const T3DVector4* rhs)
{
#if defined(_MSC_VER)
    __m128 a = _mm_load_ps(lhs->M);
    __m128 b = _mm_load_ps(rhs->M);

    __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 a_zxy = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 b_zxy = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));

    __m128 mul1 = _mm_mul_ps(a_yzx, b_zxy);
    __m128 mul2 = _mm_mul_ps(a_zxy, b_yzx);
    __m128 result = _mm_sub_ps(mul1, mul2);

    // Set w to 1.0f
   // result = _mm_blend_ps(result, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), 0x8);

    _mm_store_ps(r->M, result);
    r->w = 1.0f;
#else
    float m1, m2, m3;
    m1 = a->y * b->z - a->z * b->y;
    m2 = a->z * b->x - a->x * b->z;
    m3 = a->x * b->y - a->y * b->x;
    r->x = m1;
    r->y = m2;
    r->z = m3;
    r->w = 1.0f;
#endif
    return;
}

void T3DVector4Interpolate(T3DVector4* z, const T3DVector4* x1, const T3DVector4* x2, float t)
{
#if defined(_MSC_VER)
    // Load vectors into SSE registers
    __m128 v1 = _mm_load_ps(x1->M);
    __m128 v2 = _mm_load_ps(x2->M);

    // Calculate (x2 - x1)
    __m128 diff = _mm_sub_ps(v2, v1);

    // Broadcast t to all elements
    __m128 vt = _mm_set1_ps(t);

    // Multiply diff by t
    __m128 scaled = _mm_mul_ps(diff, vt);

    // Add x1 to get final result
    __m128 result = _mm_add_ps(v1, scaled);

    // Set w component to 1.0f
  //  result = _mm_move_ss(result, _mm_set_ss(1.0f));

    // Store result
    _mm_store_ps(z->M, result);
    z->w = 1.0f;
#else

    z->x = LinearInterpolate(x1->x, x2->x, t);
    z->y = LinearInterpolate(x1->y, x2->y, t);
    z->z = LinearInterpolate(x1->z, x2->z, t);
    z->w = 1.0f;
#endif
}

void T3DVector4Normalize(T3DVector4* v)
{
#if _MSC_VER
    // Load vector
    __m128 vec = _mm_load_ps(v->M);

    // Calculate length
    __m128 sq = _mm_mul_ps(vec, vec);
    __m128 sum = _mm_add_ps(_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(0, 0, 0, 0)),
        _mm_add_ps(_mm_shuffle_ps(sq, sq, _MM_SHUFFLE(1, 1, 1, 1)),
            _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 2, 2, 2))));
    __m128 length = _mm_sqrt_ps(sum);

    // Check if length is not zero
    __m128 zero = _mm_setzero_ps();
    __m128 mask = _mm_cmpneq_ps(length, zero);

    // Calculate inverse length
    __m128 inv_length = _mm_div_ps(_mm_set1_ps(1.0f), length);

    // Multiply vector by inverse length
    __m128 normalized = _mm_mul_ps(vec, inv_length);

    // Select between normalized and original based on mask
    normalized = _mm_and_ps(normalized, mask);
    vec = _mm_andnot_ps(mask, vec);
    normalized = _mm_or_ps(normalized, vec);

    // Store result
    _mm_store_ps(v->M, normalized);
#else
    float length = T3DVector4Length(v);

    if (length != 0.0f)
    {
        float inv = 1.0f / length;
        v->x *= inv;
        v->y *= inv;
        v->z *= inv;
    }
#endif
}