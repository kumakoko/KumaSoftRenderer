#pragma once

#include "tiny3d_transform.h"

void Transform::Update()
{
    T3DMatrix4X4 m;
    T3DMatrixMultiply(&m, &world_matrix_, &view_matrix_);
    T3DMatrixMultiply(&wvp_matrix_, &m, &projection_matrix_);
}

// 初始化，设置屏幕长宽
void Transform::Init(int width, int height, float near_clip, float far_clip)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    T3DMatrixIdentity(&this->world_matrix_);
    T3DMatrixIdentity(&this->view_matrix_);
    T3DMatrixPerspective(&this->projection_matrix_, 3.1415926f * 0.5f, aspect, near_clip, far_clip);
    this->screen_width_ = static_cast<float>(width);
    this->screen_height_ = static_cast<float>(height);
    Update();
}

// 将矢量 x 进行 project 
void Transform::Apply(T3DVector4* y, const T3DVector4* x) const
{
    T3DMatrixApply(y, x, &wvp_matrix_);
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
uint32_t Transform::CheckCVV(const T3DVector4* v) const
{
    float w = v->w;
    uint32_t check = 0;

    if (v->z < 0.0f)
        check |= 0x00000001; // 00000000 00000000 00000000 00000001;
    if (v->z > w)
        check |= 0x00000002; // 00000000 00000000 00000000 00000010;
    if (v->x < -w)
        check |= 0x00000004; // 00000000 00000000 00000000 00000100;
    if (v->x > w)
        check |= 0x00000008; // 00000000 00000000 00000000 00001000;
    if (v->y < -w)
        check |= 0x00000010; // 00000000 00000000 00000000 00010000;
    if (v->y > w)
        check |= 0x00000020; // 00000000 00000000 00000000 00100000;

    return check;
}

// 归一化，得到屏幕坐标
void Transform::Homogenize(T3DVector4* homogenized_vertex, const T3DVector4* vertex) const
{
    float rhw = 1.0f / vertex->w;
    homogenized_vertex->x = (vertex->x * rhw + 1.0f) * this->screen_width_ * 0.5f;
    homogenized_vertex->y = (1.0f - vertex->y * rhw) * this->screen_height_ * 0.5f;
    homogenized_vertex->z = vertex->z * rhw;
    homogenized_vertex->w = 1.0f;
}