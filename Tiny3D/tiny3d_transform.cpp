#pragma once

#include "tiny3d_transform.h"

void transform_t::Update()
{
    matrix_t m;
    matrix_mul(&m, &world_matrix_, &view_matrix_);
    matrix_mul(&wvp_matrix_, &m, &projection_matrix_);
}

// 初始化，设置屏幕长宽
void transform_t::Init(int width, int height, float near_clip, float far_clip)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    matrix_set_identity(&this->world_matrix_);
    matrix_set_identity(&this->view_matrix_);
    matrix_set_perspective(&this->projection_matrix_, 3.1415926f * 0.5f, aspect, near_clip, far_clip);
    this->screen_width_ = static_cast<float>(width);
    this->screen_height_ = static_cast<float>(height);
    Update();
}

// 将矢量 x 进行 project 
void transform_t::Apply(vector_t* y, const vector_t* x) const
{
    matrix_apply(y, x, &wvp_matrix_);
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
uint32_t transform_t::CheckCVV(const vector_t* v) const
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
void transform_t::Homogenize(vector_t* homogenized_vertex, const vector_t* vertex) const
{
    float rhw = 1.0f / vertex->w;
    homogenized_vertex->x = (vertex->x * rhw + 1.0f) * this->screen_width_ * 0.5f;
    homogenized_vertex->y = (1.0f - vertex->y * rhw) * this->screen_height_ * 0.5f;
    homogenized_vertex->z = vertex->z * rhw;
    homogenized_vertex->w = 1.0f;
}