#include "ksr_matrix.h"

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

    int Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod)
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

        return 1;
    }
}