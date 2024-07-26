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
        ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
        ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
        ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
        ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
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
    }

    void Mat_Mul_4X4(MATRIX4X4_PTR ma,MATRIX4X4_PTR mb,MATRIX4X4_PTR mprod)
    {
        // this function multiplies two 4x4 matrices together and 
        // and stores the result in mprod
        // note later we will take advantage of the fact that we know
        // that w=1 always, and that the last column of a 4x4 is
        // always 0

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                // compute dot product from row of ma 
                // and column of mb

                float sum = 0; // used to hold result

                for (int index = 0; index < 4; index++)
                {
                    // add in next product pair
                    sum += (ma->M[row][index] * mb->M[index][col]);
                } // end for index

           // insert resulting row,col element
                mprod->M[row][col] = sum;

            } // end for col

        } // end for row

    } // end Mat_Mul_4X4

    void Build_XYZ_Rotation_MATRIX4X4(float theta_x, // euler angles
        float theta_y,
        float theta_z,
        MATRIX4X4_PTR mrot) // output 
    {
        // this helper function takes a set if euler angles and computes
        // a rotation matrix from them, usefull for object and camera
        // work, also  we will do a little testing in the function to determine
        // the rotations that need to be performed, since there's no
        // reason to perform extra matrix multiplies if the angles are
        // zero!

        /*这个辅助函数采用一组欧拉角并从中计算出一个旋转矩阵，这对于物体和相机的工作很有用，
            我们还将在函数中做一些测试来确定需要执行的旋转，因为如果角度为零，就没有理由执行额外的矩阵乘法！*/

        MATRIX4X4 mx, my, mz, mtmp;       // working matrices
        float sin_theta = 0;
        float cos_theta = 0;   // used to initialize matrices
        int rot_seq = 0;                  // 1 for x, 2 for y, 4 for z

        // step 0: fill in with identity matrix
        MAT_IDENTITY_4X4(mrot);

        // step 1: based on zero and non-zero rotation angles, determine
        // rotation sequence
        if (fabs(theta_x) > EPSILON_E5) // x
            rot_seq = rot_seq | 1;

        if (fabs(theta_y) > EPSILON_E5) // y
            rot_seq = rot_seq | 2;

        if (fabs(theta_z) > EPSILON_E5) // z
            rot_seq = rot_seq | 4;

        // now case on sequence
        switch (rot_seq)
        {
        case 0: // no rotation
        {
            // what a waste!
            return;
        } break;

        case 1: // x rotation
        {
            // compute the sine and cosine of the angle
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            // set the matrix up 
            Mat_Init_4X4(&mx, 1, 0, 0, 0,
                0, cos_theta, sin_theta, 0,
                0, -sin_theta, cos_theta, 0,
                0, 0, 0, 1);

            // that's it, copy to output matrix
            MAT_COPY_4X4(&mx, mrot);
            return;

        } break;

        case 2: // y rotation
        {
            // compute the sine and cosine of the angle
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            // set the matrix up 
            Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
                0, 1, 0, 0,
                sin_theta, 0, cos_theta, 0,
                0, 0, 0, 1);


            // that's it, copy to output matrix
            MAT_COPY_4X4(&my, mrot);
            return;

        } break;

        case 3: // xy rotation
        {
            // compute the sine and cosine of the angle for x
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            // set the matrix up 
            Mat_Init_4X4(&mx, 1, 0, 0, 0,
                0, cos_theta, sin_theta, 0,
                0, -sin_theta, cos_theta, 0,
                0, 0, 0, 1);

            // compute the sine and cosine of the angle for y
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            // set the matrix up 
            Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
                0, 1, 0, 0,
                sin_theta, 0, cos_theta, 0,
                0, 0, 0, 1);

            // concatenate matrices 
            Mat_Mul_4X4(&mx, &my, mrot);
            return;

        } break;

        case 4: // z rotation
        {
            // compute the sine and cosine of the angle
            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            // set the matrix up 
            Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
                -sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);


            // that's it, copy to output matrix
            MAT_COPY_4X4(&mz, mrot);
            return;

        } break;

        case 5: // xz rotation
        {
            // compute the sine and cosine of the angle x
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            // set the matrix up 
            Mat_Init_4X4(&mx, 1, 0, 0, 0,
                0, cos_theta, sin_theta, 0,
                0, -sin_theta, cos_theta, 0,
                0, 0, 0, 1);

            // compute the sine and cosine of the angle z
            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            // set the matrix up 
            Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
                -sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);

            // concatenate matrices 
            Mat_Mul_4X4(&mx, &mz, mrot);
            return;

        } break;

        case 6: // yz rotation
        {
            // compute the sine and cosine of the angle y
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            // set the matrix up 
            Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
                0, 1, 0, 0,
                sin_theta, 0, cos_theta, 0,
                0, 0, 0, 1);

            // compute the sine and cosine of the angle z
            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            // set the matrix up 
            Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
                -sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);

            // concatenate matrices 
            Mat_Mul_4X4(&my, &mz, mrot);
            return;

        } break;

        case 7: // xyz rotation
        {
            // compute the sine and cosine of the angle x
            cos_theta = cosf(theta_x);
            sin_theta = sinf(theta_x);

            // set the matrix up 
            Mat_Init_4X4(&mx, 1, 0, 0, 0,
                0, cos_theta, sin_theta, 0,
                0, -sin_theta, cos_theta, 0,
                0, 0, 0, 1);

            // compute the sine and cosine of the angle y
            cos_theta = cosf(theta_y);
            sin_theta = sinf(theta_y);

            // set the matrix up 
            Mat_Init_4X4(&my, cos_theta, 0, -sin_theta, 0,
                0, 1, 0, 0,
                sin_theta, 0, cos_theta, 0,
                0, 0, 0, 1);

            // compute the sine and cosine of the angle z
            cos_theta = cosf(theta_z);
            sin_theta = sinf(theta_z);

            // set the matrix up 
            Mat_Init_4X4(&mz, cos_theta, sin_theta, 0, 0,
                -sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);

            // concatenate matrices, watch order!
            Mat_Mul_4X4(&mx, &my, &mtmp);
            Mat_Mul_4X4(&mtmp, &mz, mrot);

        } break;

        default: break;

        } // end switch

    } // end Build_XYZ_Rotation_MATRIX4X4                                    
}