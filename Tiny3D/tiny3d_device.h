#pragma once

#include <cstdint>

#include "tiny3d_transform.h"
#include "tiny3d_geometry.h"

//=====================================================================
// 渲染设备
//=====================================================================

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色


struct device_t 
{
    transform_t transform;      // 坐标变换器
    uint32_t width;                  // 窗口宽度
    uint32_t height;                 // 窗口高度
    uint32_t* framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
    uint32_t framebuffer_pitch;
    float* zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针
    uint32_t* texture;          // 纹理：同样是每行索引
    int tex_width;              // 纹理宽度
    int tex_height;             // 纹理高度
    float max_u;                // 纹理最大宽度：tex_width - 1
    float max_v;                // 纹理最大高度：tex_height - 1
    int render_state;           // 渲染状态
    uint32_t background;         // 背景颜色
    uint32_t foreground;         // 线框颜色

public:
    inline void SetFrameBufer(uint8_t* buffer,uint32_t pitch)
    {
        framebuffer = reinterpret_cast<uint32_t*>(buffer);
        framebuffer_pitch = pitch;
    }


public:


    // 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
    void device_init(int width, int height);

    // 删除设备
    void device_destroy();

    // 设置当前纹理
    void device_set_texture(void* bits, long pitch, int w, int h);

    // 清空 framebuffer 和 zbuffer
    void device_clear(int mode);

    // 画点
    void device_pixel(uint32_t x, uint32_t y, uint32_t color);

    // 绘制线段
    void device_draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t c);

    // 根据坐标读取纹理
    uint32_t device_texture_read(float u, float v);

    // 绘制扫描线
    void device_draw_scanline(scanline_t* scanline);

    // 主渲染函数
    void device_render_trap(trapezoid_t* trap);

    // 根据 render_state 绘制原始三角形
    void device_draw_primitive(const vertex_t* v1, const vertex_t* v2, const vertex_t* v3);

    void camera_at_zero(float x, float y, float z);
};

