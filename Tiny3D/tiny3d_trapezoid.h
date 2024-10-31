#pragma once

#include <array>
#include "tiny3d_geometry.h"

struct trapezoid_t
{
    float top;
    float bottom;
    edge_t left;   // 左腰边
    edge_t right;  // 右腰边

    // 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
    static int trapezoid_init_triangle(std::array<trapezoid_t, 2>& trap, const vertex_t* p1, const vertex_t* p2, const vertex_t* p3);

    // 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
    void trapezoid_edge_interp(float y);

    // 根据左右两边的端点，初始化计算出扫描线的起点和步长
    void trapezoid_init_scan_line(scanline_t* scanline, int y);
};
