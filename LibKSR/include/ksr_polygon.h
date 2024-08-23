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

#include <cstdint>
#include "ksr_vertex.h"
#include "ksr_bitmap.h"

namespace KSR
{
    // 2D多边形
    typedef struct POLYGON2D_TYP
    {
        int state;          // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int num_verts;      // 本多边形有多少个顶点
        int x0, y0;         // 本多边形中心点的位置
        int xv, yv;         // 本多边形的初始速度
        std::uint32_t color;// 本多边形的颜色
        VERTEX2DF* vlist;   // 指向本多边形的顶点的列表的首指针
    } POLYGON2D, * POLYGON2D_PTR;

    // 第1个版本的3D多边形结构体
    // 表示一个多边形，但需要引用外部顶点列表
    typedef struct POLY4DV1_TYP
    {
        int state;          // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int attr;           // 本多边形的物理属性，比如是否是双目的，是否反射光，是否透明的，等等，可参见POLY4DV1_ATTR_2SIDED等
        std::uint32_t color;// 本多边形的颜色
        POINT4D_PTR vlist;  // 指向本多边形的顶点的列表的首指针
        int vert[3];        // 存储着构成本多边形三个顶点的顶点索引值的数组
    }POLY4DV1, *POLY4DV1_PTR;

    // 第2个版本的3D多边形结构体
    typedef struct POLY4DV2_TYP
    {
        int state;                  // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int attr;                   // 本多边形的物理属性，比如是否是双目的，是否反射光，是否透明的，等等，可参见POLY4DV1_ATTR_2SIDED等
        std::uint32_t color;        // 本多边形的颜色
        int lit_color[3];           // 用来存储经过光照处理后的颜色，用flat shading的话，颜色存在数组第一个元素中，用Gouraud着色的话，顶点颜色存在三个元素中
        BITMAP_IMAGE_PTR texture;   // 指向纹理信息的指针，用于纹理映色
        int mati;                   // 材质索引，-1表示没有材质
        VERTEX4DTV1_PTR vlist;      // 顶点列表
        POINT2D_PTR     tlist;      // 纹理坐标列表
        int vert[3];                // 指向顶点列表的索引
        int text[3];                // 指向纹理坐标列表的索引
        float nlength;              // 法线长度
    } POLY4DV2, *POLY4DV2_PTR;

    // 自包含（顶点）的多边形结构体，可以用于render list
    // 表示一个多边形，内部存储了顶顶啊数据，是主多边形列表的主要组成部分
    typedef struct POLYF4DV1_TYP
    {
        int state;              // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int attr;               // 本多边形的物理属性，比如是否是双目的，是否反射光，是否透明的，等等，可参见POLY4DV1_ATTR_2SIDED等
        std::uint32_t color;    // 本多边形的颜色
        POINT4D vlist[3];       // 三个元素的数组，每一个元素对应于一个三角形的一个顶点坐标
        POINT4D tvlist[3];      // 三个元素的数组，每一个元素对应于一个三角形的一个经过变换坐标系的顶点坐标
        POLYF4DV1_TYP* next;    // 指向列表中下一个多边形的指针
        POLYF4DV1_TYP* prev;    // 指向列表中前一个多边形的指针
    } POLYF4DV1, *POLYF4DV1_PTR;

    // 自包含（顶点）的第2版的多边形结构体，可以用于render list
    typedef struct POLYF4DV2_TYP
    {
        int state;                // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int attr;                 // 本多边形的物理属性，比如是否是双目的，是否反射光，是否透明的，等等，可参见POLY4DV1_ATTR_2SIDED等
        std::uint32_t color;      // 本多边形的颜色
        int lit_color[3];         // 用来存储经过光照处理后的颜色，用flat shading的话，颜色存在数组第一个元素中，用Gouraud着色的话，顶点颜色存在三个元素中
        BITMAP_IMAGE_PTR texture; // 指向纹理信息的指针，用于纹理映色
        int mati;                 // 材质索引，-1表示没有材质
        float    nlength;         // 法线长度
        VECTOR4D normal;          // 多边形的法线
        float    avg_z;           // 平均Z值，用来排序
        VERTEX4DTV1 vlist[3];     // 三个元素的数组，每一个元素对应于一个三角形的一个顶点坐标
        VERTEX4DTV1 tvlist[3];    // 三个元素的数组，每一个元素对应于一个三角形的一个经过变换坐标系的顶点坐标
        POLYF4DV2_TYP* next;      // 指向列表中下一个多边形的指针
        POLYF4DV2_TYP* prev;      // 指向列表中前一个多边形的指针
    } POLYF4DV2, *POLYF4DV2_PTR;

    /**************************************************************************************
    
    @name: KSR::Draw_Filled_Polygon2D
    @return: void
    @param: POLYGON2D_PTR poly
    @param: std::uint8_t * vbuffer
    @param: int mempitch
    *************************************************************************************/
    void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);

    /**************************************************************************************
    
    @name: KSR::Draw_Filled_Polygon2D16
    @return: void
    @param: POLYGON2D_PTR poly
    @param: std::uint8_t * vbuffer
    @param: int mempitch
    *************************************************************************************/
    void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch);
}