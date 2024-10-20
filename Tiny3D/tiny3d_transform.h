﻿#pragma once

#include "tiny3d_matrix.h"

struct transform_t
{
    matrix_t world;         // 世界坐标变换
    matrix_t view;          // 摄影机坐标变换
    matrix_t projection;    // 投影变换
    matrix_t transform;     // transform = world * view * projection
    float w, h;             // 屏幕大小
};

// 矩阵更新，计算 transform = world * view * projection
void transform_update(transform_t* ts);

// 初始化，设置屏幕长宽
void transform_init(transform_t* ts, int width, int height);

// 将矢量 x 进行 project 
void transform_apply(const transform_t* ts, vector_t* y, const vector_t* x);

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int transform_check_cvv(const vector_t* v);

// 归一化，得到屏幕坐标
void transform_homogenize(const transform_t* ts, vector_t* y, const vector_t* x);