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