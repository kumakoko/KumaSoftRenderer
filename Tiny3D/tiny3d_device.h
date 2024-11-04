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

#include "tiny3d_transform.h"
#include "tiny3d_geometry.h"
#include "tiny3d_trapezoid.h"

//=====================================================================
// 渲染设备
//=====================================================================

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色


struct Device 
{
public:
    Transform transform_;     // 坐标变换器
    uint32_t window_width_;     // 窗口宽度
    uint32_t window_height_;    // 窗口高度
    uint32_t* frame_buffer_;    // 像素缓存：framebuffer[y] 代表第 y行
    float* z_buffer_;           // 深度缓存：zbuffer[y] 为第 y行指针
    uint32_t* texture_;         // 纹理：同样是每行索引
    uint32_t texture_width_;    // 纹理宽度
    uint32_t texture_height_;   // 纹理高度
    float max_u_;               // 纹理最大宽度：tex_width - 1
    float max_v_;               // 纹理最大高度：tex_height - 1
    int render_state_;          // 渲染状态
    uint32_t background_color_; // 背景颜色
    uint32_t foreground_color_; // 线框颜色

public:
    inline void SetFrameBufer(uint8_t* buffer)
    {
        frame_buffer_ = reinterpret_cast<uint32_t*>(buffer);
    }

    /**************************************************************************************
    设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
    @name: Device::Initialize
    @return: void
    @param: int width
    @param: int height
    *************************************************************************************/
    void Initialize(int width, int height);

    /**************************************************************************************
    删除设备
    @name: Device::Destroy
    @return: void
    *************************************************************************************/
    void Destroy();

    /**************************************************************************************
    清空 Frame Buffer 和 Z Buffer
    @name: Device::Clear
    @return: void
    @param: int mode
    *************************************************************************************/
    void ResetZBuffer();

    /**************************************************************************************
    画点
    @name: Device::WritePixel
    @return: void
    @param: uint32_t x
    @param: uint32_t y
    @param: uint32_t color
    *************************************************************************************/
    void WritePixel(uint32_t x, uint32_t y, uint32_t color);

    /**************************************************************************************
    采用Bresenham算法绘制线段
    @name: Device::DrawLine
    @return: void
    @param: uint32_t x1
    @param: uint32_t y1
    @param: uint32_t x2
    @param: uint32_t y2
    @param: uint32_t c
    *************************************************************************************/
    void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t c);

    /**************************************************************************************
    根据坐标读取纹理
    @name: Device::GetTexel
    @return: uint32_t
    @param: float u
    @param: float v
    *************************************************************************************/
    uint32_t GetTexel(float u, float v);

    /**************************************************************************************
    绘制扫描线
    @name: Device::DrawScanline
    @return: void
    @param: scanline_t * scanline
    *************************************************************************************/
    void DrawScanline(scanline_t* scanline);

    /**************************************************************************************
    渲染梯形
    @name: Device::RenderTrapezoid
    @return: void
    @param: Trapezoid * trap
    *************************************************************************************/
    void RenderTrapezoid(Trapezoid* trap);

    /**************************************************************************************
    根据 render_state 绘制原始三角形
    @name: Device::DrawPrimitive
    @return: void
    @param: const vertex_t * v1
    @param: const vertex_t * v2
    @param: const vertex_t * v3
    *************************************************************************************/
    void DrawPrimitive(const T3DVertex* v1, const T3DVertex* v2, const T3DVertex* v3);

    /**************************************************************************************
    
    @name: Device::ResetCamera
    @return: void
    @param: float x
    @param: float y
    @param: float z
    *************************************************************************************/
    void ResetCamera(float x, float y, float z);

    /**************************************************************************************
    
    @name: Device::InitTexture
    @return: void
    *************************************************************************************/
    void InitTexture();

    void CreateTextureFromFile(const char* file_path);

    /**************************************************************************************
    
    @name: Device::DrawPlane
    @return: void
    @param: const vertex_t * p1
    @param: const vertex_t * p2
    @param: const vertex_t * p3
    @param: const vertex_t * p4
    *************************************************************************************/
    void DrawPlane(const T3DVertex* p1, const T3DVertex* p2, const T3DVertex* p3, const T3DVertex* p4);

    /**************************************************************************************
    
    @name: Device::DrawBox
    @return: void
    @param: float theta
    @param: const vertex_t * box_vertices
    *************************************************************************************/
    void DrawBox(float theta, const T3DVertex* box_vertices);
};

