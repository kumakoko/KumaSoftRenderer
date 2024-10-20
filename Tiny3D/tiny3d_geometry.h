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
    point_t pos;
    texcoord_t tc;
    color_t color;
    float rhw;
};

struct edge_t
{
    vertex_t v, v1, v2;
};

struct trapezoid_t
{
    float top, bottom; edge_t left, right;
};

struct scanline_t
{
    vertex_t v, step;
    int x, y, w;
};

void vertex_rhw_init(vertex_t* v);

void vertex_interp(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float t);

void vertex_division(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float w);

void vertex_add(vertex_t* y, const vertex_t* x);

// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
int trapezoid_init_triangle(trapezoid_t* trap, const vertex_t* p1, const vertex_t* p2, const vertex_t* p3);

// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
void trapezoid_edge_interp(trapezoid_t* trap, float y);

// 根据左右两边的端点，初始化计算出扫描线的起点和步长
void trapezoid_init_scan_line(const trapezoid_t* trap, scanline_t* scanline, int y);