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

#include "ksr_transform.h"

namespace KSR
{
    void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR  va,
        MATRIX4X4_PTR mb,
        VECTOR3D_PTR  vprod)
    {
        // this function multiplies a VECTOR3D against a 
        // 4x4 matrix - ma*mb and stores the result in mprod
        // the function assumes that the vector refers to a 
        // 4D homogenous vector, thus the function assumes that
        // w=1 to carry out the multiply, also the function
        // does not carry out the last column multiply since
        // we are assuming w=1, there is no point
        int col, row;
        for ( col = 0; col < 3; col++)
        {
            // compute dot product from row of ma 
            // and column of mb
            float sum = 0; // used to hold result

            for ( row = 0; row < 3; row++)
            {
                // add in next product pair
                sum += (va->M[row] * mb->M[row][col]);
            } // end for index

       // add in last element in column or w*mb[3][col]
            sum += mb->M[row][col];

            // insert resulting col element
            vprod->M[col] = sum;

        } // end for col

    } // end Mat_Mul_VECTOR3D_4X4

    ///////////////////////////////////////////////////////////////

    void Mat_Mul_VECTOR3D_4X3(VECTOR3D_PTR  va,
        MATRIX4X3_PTR mb,
        VECTOR3D_PTR  vprod)
    {
        // this function multiplies a VECTOR3D against a 
        // 4x3 matrix - ma*mb and stores the result in mprod
        // the function assumes that the vector refers to a 
        // 4D homogenous vector, thus the function assumes that
        // w=1 to carry out the multiply, also the function
        // does not carry out the last column multiply since
        // we are assuming w=1, there is no point
        int col, row;
        for (col = 0; col < 3; col++)
        {
            // compute dot product from row of ma 
            // and column of mb
            float sum = 0; // used to hold result

            for (row = 0; row < 3; row++)
            {
                // add in next product pair
                sum += (va->M[row] * mb->M[row][col]);
            } // end for index

       // add in last element in column or w*mb[3][col]
            sum += mb->M[row][col];

            // insert resulting col element
            vprod->M[col] = sum;

        } // end for col

    } // end Mat_Mul_VECTOR3D_4X3

    ////////////////////////////////////////////////////////////////////

    void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR  va,
        MATRIX4X4_PTR mb,
        VECTOR4D_PTR  vprod)
    {
        // this function multiplies a VECTOR4D against a 
        // 4x4 matrix - ma*mb and stores the result in mprod
        // the function makes no assumptions

        for (int col = 0; col < 4; col++)
        {
            // compute dot product from row of ma 
            // and column of mb
            float sum = 0; // used to hold result

            for (int row = 0; row < 4; row++)
            {
                // add in next product pair
                sum += (va->M[row] * mb->M[row][col]);
            } // end for index

       // insert resulting col element
            vprod->M[col] = sum;

        } // end for col

    } // end Mat_Mul_VECTOR4D_4X4

    ////////////////////////////////////////////////////////////////////

    void Mat_Mul_VECTOR4D_4X3(VECTOR4D_PTR  va,
        MATRIX4X4_PTR mb,
        VECTOR4D_PTR  vprod)
    {
        // this function multiplies a VECTOR4D against a 
        // 4x3 matrix - ma*mb and stores the result in mprod
        // the function assumes that the last column of
        // mb is [0 0 0 1]t , thus w just gets replicated
        // from the vector [x y z w]

        for (int col = 0; col < 3; col++)
        {
            // compute dot product from row of ma 
            // and column of mb
            float sum = 0; // used to hold result

            for (int row = 0; row < 4; row++)
            {
                // add in next product pair
                sum += (va->M[row] * mb->M[row][col]);
            } // end for index

       // insert resulting col element
            vprod->M[col] = sum;

        } // end for col

     // copy back w element
        vprod->M[3] = va->M[3];

    } // end Mat_Mul_VECTOR4D_4X3
}