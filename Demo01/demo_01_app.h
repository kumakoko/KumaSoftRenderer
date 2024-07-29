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

#include "ksr_app.h"
#include "ksr_vector.h"
#include "ksr_camera.h"
#include "ksr_vector.h"
#include "ksr_camera.h"
#include "ksr_render_list.h"
#include "ksr_polygon.h"
#include "ksr_surface.h"
#include "ksr_shape_drawing.h"
#include "ksr_color.h"
#include "ksr_timer.h"

class alignas(16) Demo01App : public KSR::App
{
public:
    virtual void InitScene() override;
    virtual void RenderScene() override;
private:
    KSR::POINT4D          camera_position_ = { 0.0f,0.0f,-100.0f,1.0f };    // 摄像机的初始位置
    KSR::VECTOR4D         camera_direction_ = { 0.0f,0.0f,0.0f,1.0f };      // 摄像机的初始朝向
    KSR::CAM4DV1          camera_;                                          // 主摄像机
    KSR::RENDERLIST4DV1   render_list_;                                     // 唯一的渲染列表
    KSR::POLYF4DV1        single_polygon_;                                      // 唯一的待渲染的三角形our lonely polygon
    KSR::POINT4D          single_polygon_position_ = { 0.0f,0.0f,100.0f,1.0f }; // 唯一的待渲染的三角形的世界坐标world position of polygon
    uint32_t              wnd_render_area_width_ = 1024;
    uint32_t              wnd_render_area_height_ = 768;
    KSR::MATRIX4X4        rotation_matrix_;                                     // 多边形的旋转矩阵
    float                 rotation_angle_y_ = 0;                                // 多边形绕自身的y轴旋转的角度
};