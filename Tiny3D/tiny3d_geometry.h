#pragma once

#include "tiny3d_vector.h"

//=====================================================================
// 几何计算：顶点、扫描线、边缘、矩形、步长计算
//=====================================================================

struct T3DColor
{
    float r, g, b;
};

struct T3DTextureCoord
{
    float u, v;
};

struct T3DVertex
{
    T3DVector4 pos;
    T3DTextureCoord tc;
    T3DColor color;
    float rhw;
};

struct edge_t
{
    T3DVertex interpolated_point;  // 给了插值比例值后，根据端点12和插值比例值所算出来的插值位置点
    T3DVertex v1; // 边的端点1
    T3DVertex v2; // 边的端点2
};


void T3DVertexRHWInit(T3DVertex* v);

void T3DVertexInterpolate(T3DVertex* y, const T3DVertex* x1, const T3DVertex* x2, float t);

void T3DVertexDivision(T3DVertex* step, const T3DVertex* x1, const T3DVertex* x2, float w);

void T3DVertexAdd(T3DVertex* y, const T3DVertex* x);