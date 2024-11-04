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

#pragma once

#include <cstdint>
#include "tiny3d_matrix.h"

class Transform
{
private:
    T3DMatrix4X4 world_matrix_;         // 世界坐标变换
    T3DMatrix4X4 view_matrix_;          // 摄影机坐标变换
    T3DMatrix4X4 projection_matrix_;    // 投影变换
    T3DMatrix4X4 wvp_matrix_;           // WVP矩阵，即 world * view * projection
    float screen_width_;            // 屏幕宽
    float screen_height_;           // 屏幕高
public:
    /**************************************************************************************
     矩阵更新，计算 transform = world * view * projection
    @name: transform_t::Update
    @return: void
    *************************************************************************************/
    void Update();

    /**************************************************************************************
    初始化，设置屏幕长宽
    @name: Transform::Init
    @return: void
    @param: int width
    @param: int height
    *************************************************************************************/
    void Init(int width, int height, float near_clip, float far_clip);

    /**************************************************************************************
    将矢量 x 进行 project
    @name: Transform::Apply
    @return: void
    @param: T3DVector4 * y
    @param: const T3DVector4 * x
    *************************************************************************************/
    void Apply(T3DVector4* y, const T3DVector4* x) const;

    /**************************************************************************************
    检查齐次坐标同 cvv 的边界用于视锥裁剪
    @name: Transform::CheckCVV
    @return: uint32_t
    @param: const T3DVector4 * v
    *************************************************************************************/
    uint32_t CheckCVV(const T3DVector4* v) const;

    /**************************************************************************************
    归一化，得到屏幕坐标
    @name: Transform::Homogenize
    @return: void
    @param: T3DVector4 * homogenized_vertex
    @param: const T3DVector4 * vertex
    *************************************************************************************/
    void Homogenize(T3DVector4* homogenized_vertex, const T3DVector4* vertex) const;

    inline void SetViewMatrix(const T3DVector4* eye, const T3DVector4* at, const T3DVector4* up)
    {
        T3DMatrixMakeLookat(&view_matrix_, eye, at, up);
    }

    inline void SetWorldMatrix(const T3DMatrix4X4& m)
    {
        world_matrix_ = m;
    }
};