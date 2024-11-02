#pragma once

#include "tiny3d_geometry.h"

struct scanline_t
{
    vertex_t interpolated_point;   // 扫描线对应的梯形左腰边的插值点
    vertex_t interpolated_step;    // 扫描线每执行一步扫描的【插值步】值
    int32_t left_end_point_x;          // 扫描线的左端点X值，即对应于第几列像素
    int32_t y;                         // 扫描线的垂直方向Y值，即对应于第几行像素
    int32_t width;                     // 扫描线的长度
};