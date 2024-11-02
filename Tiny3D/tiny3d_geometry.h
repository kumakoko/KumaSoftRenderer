#pragma once

#include "tiny3d_vector.h"

//=====================================================================
// 几何计算：顶点、扫描线、边缘、矩形、步长计算
//=====================================================================

struct color_t
{
    float r, g, b;
};

struct texcoord_t
{
    float u, v;
};

struct vertex_t
{
    T3DVector4 pos;
    texcoord_t tc;
    color_t color;
    float rhw;
};

struct edge_t
{
    vertex_t interpolated_point;  // 给了插值比例值后，根据端点12和插值比例值所算出来的插值位置点
    vertex_t v1; // 边的端点1
    vertex_t v2; // 边的端点2
};


void vertex_rhw_init(vertex_t* v);

void vertex_interp(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float t);

void vertex_division(vertex_t* step, const vertex_t* x1, const vertex_t* x2, float w);

void vertex_add(vertex_t* y, const vertex_t* x);