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
/*!
 * \file ksr_transform.h
 * \date 2024/07/26 9:33
 *
 * \author www.xionggf.com
 * Contact: 
 *
 * \brief 
 *
 * TODO: 定义了一系列向量矩阵变换计算的函数
 *
 * \note
*/
#pragma once
#include "ksr_vector.h"
#include "ksr_matrix.h"

namespace KSR
{
    // 顶点坐标变换的控制标志
    enum TransformControlFlag
    {
        TRANSFORM_LOCAL_ONLY = 0,   // 对多边形的【局部/世界坐标系下的顶点】进行变换，即只对
                                    //【多边形结构体中的成员变量vlist】中的顶点数据进行变换，
                                    // 且变换之后还是存回到成员变量vlist中
        TRANSFORM_TRANS_ONLY = 1,   // 对多边形的【经过变换后的顶点】进行变换，即只对
                                    //【多边形结构体中的成员变量tvlist】中的顶点数据进行变换，
                                    // 且变换之后还是存回到成员变量tvlist中
        TRANSFORM_LOCAL_TO_TRANS = 2// 对【多边形结构体中的成员变量vlist】中的顶点数据进行变换，
                                    // 且变换之后存到多边形结构体中的成员变量tvlist】中
    };

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR3D_4X4
    @return: void
    @param: VECTOR3D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR3D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR va, MATRIX4X4_PTR mb, VECTOR3D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR3D_4X3
    @return: void
    @param: VECTOR3D_PTR va
    @param: MATRIX4X3_PTR mb
    @param: VECTOR3D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR3D_4X3(VECTOR3D_PTR va, MATRIX4X3_PTR mb, VECTOR3D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR4D_4X4
    @return: void
    @param: VECTOR4D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR4D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR4D_4X3
    @return: void
    @param: VECTOR4D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR4D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR4D_4X3(VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod);
}
