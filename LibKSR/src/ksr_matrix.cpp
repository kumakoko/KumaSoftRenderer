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

#include <cmath>
#if defined(_MSC_VER)
#include <xmmintrin.h>  // SSE header
#include <pmmintrin.h>
#endif
#include "ksr_matrix.h"
#include "ksr_constants.h"

namespace KSR
{
    // 4x4 identity matrix
    const MATRIX4X4 IMAT_4X4 =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    // 4x3 identity matrix (note this is not correct mathematically)
    // but later we may use 4x3 matrices with the assumption that 
    // the last column is always [0 0 0 1]t
    const MATRIX4X3 IMAT_4X3 =
    {
        1,0,0,
        0,1,0,
        0,0,1,
        0,0,0
    };

    // 3x3 identity matrix
    const MATRIX3X3 IMAT_3X3 =
    {
        1,0,0,
        0,1,0,
        0,0,1
    };

    // 2x2 identity matrix
    const MATRIX2X2 IMAT_2X2 =
    {
        1,0,
        0,1
    };

    void Mat_Init_3X2(MATRIX3X2_PTR ma,
        float m00, float m01,
        float m10, float m11,
        float m20, float m21)
    {
        ma->M[0][0] = m00; ma->M[0][1] = m01;
        ma->M[1][0] = m10; ma->M[1][1] = m11;
        ma->M[2][0] = m20; ma->M[2][1] = m21;
    }

    void Mat_Init_4X4(MATRIX4X4_PTR ma,
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33)
    {
#if defined(_MSC_VER)
        __m128 row0 = _mm_set_ps(m03, m02, m01, m00);
        __m128 row1 = _mm_set_ps(m13, m12, m11, m10);
        __m128 row2 = _mm_set_ps(m23, m22, m21, m20);
        __m128 row3 = _mm_set_ps(m33, m32, m31, m30);

        _mm_store_ps(ma->M[0], row0);
        _mm_store_ps(ma->M[1], row1);
        _mm_store_ps(ma->M[2], row2);
        _mm_store_ps(ma->M[3], row3);
#else
        ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
        ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
        ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
        ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
#endif
    }

    /*
    经过扩展后的ma: [x,y,1]

    mb: |m00,m01]
        |m10,m11|
        |m20,m23|

    调用此函数执行后的值为
    mprod = [x*00+y*m10+1*m20,x*00+y*m10+1*m20]
    */
    int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma, MATRIX3X2_PTR mb, MATRIX1X2_PTR mprod)
    {
#if defined(_MSC_VER)
        // Load the 1x2 matrix into an SSE register
        __m128 vecA = _mm_set_ps(0.0f, 0.0f, ma->M01, ma->M00);

        for (int col = 0; col < 2; col++)
        {
            // Load the corresponding column of matrix B
            __m128 colB = _mm_set_ps(mb->M[2][col], 0.0f, mb->M[1][col], mb->M[0][col]);

            // Perform dot product and add the result
            __m128 result = _mm_mul_ps(vecA, colB);

            // Horizontal add to sum the products
            result = _mm_hadd_ps(result, result);
            result = _mm_hadd_ps(result, result);

            // Store the result in the product matrix
            mprod->M[col] = _mm_cvtss_f32(result);
        }
#else
        // 假设1x2矩阵的第三个元素为1：
        // 用循环来计算性能较慢，后续可以用并行计算以及循环展开的方式去优化它
        for (int col = 0; col < 2; col++)
        {
            // 计算ma中的行，与mb中的列之间的点积

            float sum = 0;

            for (int index = 0; index < 2; index++)
            {
                sum += (ma->M[index] * mb->M[index][col]);
            }

            // 最后一项，让mb的第三行元素的第col列元素
            sum += mb->M[2][col] * 1.0f;
            mprod->M[col] = sum;
        }
#endif
        return 1;
    }

    int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma, MATRIX3X3_PTR mb, MATRIX1X3_PTR mprod)
    {
        // ma的行，点积mb的列，得出的值，作为mprod的一列
        for (int col = 0; col < 3; col++)
        {
            float sum = 0;

            for (int index = 0; index < 3; index++)
            {
                sum += (ma->M[index] * mb->M[index][col]);
            }

            mprod->M[col] = sum;
        }

        return 1;
    }

    void Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod)
    {
#if defined(_MSC_VER)
        // Load rows of matrix A
        __m128 rowA0 = _mm_loadu_ps(&ma->M[0][0]); // Load M00, M01, M02, and one extra
        __m128 rowA1 = _mm_loadu_ps(&ma->M[1][0]); // Load M10, M11, M12, and one extra
        __m128 rowA2 = _mm_loadu_ps(&ma->M[2][0]); // Load M20, M21, M22, and one extra

        for (int col = 0; col < 3; col++)
        {
            // Broadcast elements of matrix B for column operations
            __m128 colB = _mm_set_ps(0.0f, mb->M[2][col], mb->M[1][col], mb->M[0][col]);

            // Multiply and sum rows and columns
            __m128 sum0 = _mm_mul_ps(rowA0, colB);
            __m128 sum1 = _mm_mul_ps(rowA1, colB);
            __m128 sum2 = _mm_mul_ps(rowA2, colB);

            // Horizontal add to sum up the products
            sum0 = _mm_hadd_ps(sum0, sum0);
            sum0 = _mm_hadd_ps(sum0, sum0);

            sum1 = _mm_hadd_ps(sum1, sum1);
            sum1 = _mm_hadd_ps(sum1, sum1);

            sum2 = _mm_hadd_ps(sum2, sum2);
            sum2 = _mm_hadd_ps(sum2, sum2);

            // Store the results in the product matrix
            mprod->M[0][col] = _mm_cvtss_f32(sum0);
            mprod->M[1][col] = _mm_cvtss_f32(sum1);
            mprod->M[2][col] = _mm_cvtss_f32(sum2);
        }
#else
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                float sum = 0;

                for (int index = 0; index < 3; index++)
                {
                    sum += (ma->M[row][index] * mb->M[index][col]);
                }

                mprod->M[row][col] = sum;
            }
        }
#endif
    }

    void Mat_Mul_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod)
    {
#if defined(_MSC_VER)
        __m128 row1, row2, row3, row4;
        __m128 col;

        for (int row = 0; row < 4; row++)
        {
            row1 = _mm_load_ps(&ma->M[row][0]);  // 加载第一个矩阵的行

            for (int col_idx = 0; col_idx < 4; col_idx++)
            {
                col = _mm_set_ps(mb->M[3][col_idx], mb->M[2][col_idx], mb->M[1][col_idx], mb->M[0][col_idx]);
                row2 = _mm_mul_ps(row1, col);  // 逐元素相乘
                row3 = _mm_hadd_ps(row2, row2); // 水平加法
                row4 = _mm_hadd_ps(row3, row3); // 再次水平加法
                _mm_store_ss(&mprod->M[row][col_idx], row4); // 存储结果
            }
        }
#else
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                float sum = 0;

                for (int index = 0; index < 4; index++)
                {
                    sum += (ma->M[row][index] * mb->M[index][col]);
                }

                mprod->M[row][col] = sum;
            }
        }
#endif
    }

    void Build_XYZ_Rotation_MATRIX4X4(float theta_x, float theta_y, float theta_z, MATRIX4X4_PTR mrot)
    {
        /*这个辅助函数采用一组欧拉角并从中计算出一个旋转矩阵，这对于物体和相机的工作很有用，
            我们还将在函数中做一些测试来确定需要执行的旋转，因为如果角度为零，就没有理由执行额外的矩阵乘法！*/

        MATRIX4X4 mx, my, mz, mtmp;
        float sin_theta = 0;
        float cos_theta = 0;
        int rot_seq = 0; // 1表示绕x轴旋转，2表示绕y轴旋转，4表示绕z轴旋转 for x, 2 for y, 4 for z

        // step 0: fill in with identity matrix
        MAT_IDENTITY_4X4(mrot);

        // 分别看看绕xyz轴旋转的欧拉角度为多少，因为累积计算的原因，当意图为0的时候，可能会有小小的偏差
        // 如果需要和epslion比较，而不是直接和0比较
        if (fabs(theta_x) > EPSILON_E5) // x
            rot_seq = rot_seq | 1;

        if (fabs(theta_y) > EPSILON_E5) // y
            rot_seq = rot_seq | 2;

        if (fabs(theta_z) > EPSILON_E5) // z
            rot_seq = rot_seq | 4;

        switch (rot_seq)
        {
        case 0: // 无旋转
            return;
        case 1: //仅绕x轴旋转
        {
            // 根据旋转角算出正余弦，然后构建旋转矩阵
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            Mat_Init_4X4(&mx,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos_theta, sin_theta, 0.0f,
                0.0f, -sin_theta, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            MAT_COPY_4X4(&mx, mrot);
        }
        return;
        case 2: //仅绕x轴旋转
        {
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            Mat_Init_4X4(&my,
                cos_theta, 0.0f, -sin_theta, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin_theta, 0.0f, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            MAT_COPY_4X4(&my, mrot);
        }
        return;
        case 3: // 先绕x轴然后绕y轴旋转
        {
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            Mat_Init_4X4(&mx,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos_theta, sin_theta, 0.0f,
                0.0f, -sin_theta, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            Mat_Init_4X4(&my,
                cos_theta, 0.0f, -sin_theta, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin_theta, 0.0f, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            Mat_Mul_4X4(&mx, &my, mrot);

        }
        return;
        case 4: //仅绕z轴旋转
        {
            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            Mat_Init_4X4(&mz,
                cos_theta, sin_theta, 0.0f, 0.0f,
                -sin_theta, cos_theta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            MAT_COPY_4X4(&mz, mrot);
        }
        return;
        case 5:  // 先绕x轴然后绕z轴旋转
        {
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            Mat_Init_4X4(&mx,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos_theta, sin_theta, 0.0f,
                0.0f, -sin_theta, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            Mat_Init_4X4(&mz,
                cos_theta, sin_theta, 0.0f, 0.0f,
                -sin_theta, cos_theta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            Mat_Mul_4X4(&mx, &mz, mrot);
        }
        return;
        case 6:// 先绕y轴然后绕z轴旋转
        {
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            Mat_Init_4X4(&my,
                cos_theta, 0, -sin_theta, 0,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin_theta, 0, cos_theta, 0,
                0.0f, 0.0f, 0.0f, 1.0f);

            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            Mat_Init_4X4(&mz,
                cos_theta, sin_theta, 0.0f, 0.0f,
                -sin_theta, cos_theta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            Mat_Mul_4X4(&my, &mz, mrot);
        }
        return;
        case 7: // 先绕x轴然后绕y轴最后绕z轴旋转
        {
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            Mat_Init_4X4(&mx,
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos_theta, sin_theta, 0.0f,
                0.0f, -sin_theta, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            Mat_Init_4X4(&my,
                cos_theta, 0.0f, -sin_theta, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin_theta, 0.0f, cos_theta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            Mat_Init_4X4(&mz,
                cos_theta, sin_theta, 0.0f, 0.0f,
                -sin_theta, cos_theta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);

            Mat_Mul_4X4(&mx, &my, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz, mrot);
        }
        return;
        default:
            break;
        }
    }                             
}