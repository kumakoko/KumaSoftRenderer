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

#include "ksr_vector.h"

namespace KSR
{
    typedef struct VERTEX2DI_TYP
    {
        int x, y; // the vertex
    } VERTEX2DI, *VERTEX2DI_PTR;

    typedef struct VERTEX2DF_TYP
    {
        float x, y; // the vertex
    } VERTEX2DF, *VERTEX2DF_PTR;

    // 包含了两个纹理坐标顶点法线的4D齐次顶点
    typedef struct VERTEX4DTV1_TYP
    {
        union
        {
            float M[12];    // 三种存储方式之一：数组

            struct          // 三种存储方式之二：结构体，且其数据成员为基本数据类型
            {
                float x, y, z, w;     // 位置坐标
                float nx, ny, nz, nw; // 法线
                float u0, v0;         // 纹理坐标
                float i;              // 经过光照计算后的顶点亮度
                int attr;             // 顶点属性
            };

            struct          // 三种存储方式之三：结构体，且其数据成员为高级数据类型
            {
                POINT4D  v;       // 位置坐标
                VECTOR4D n;       // 法线
                POINT2D  t;       // 纹理坐标
            };
        };
    } VERTEX4DTV1, * VERTEX4DTV1_PTR;
}