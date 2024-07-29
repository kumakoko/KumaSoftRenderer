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

#include "demo_01_app.h"

void Demo01App::InitScene()
{
    single_polygon_.state = KSR::POLY4DV1_STATE_ACTIVE;
    single_polygon_.attr = 0;
    single_polygon_.color = 0xFFFF;//RGB16Bit(0, 255, 0);

    single_polygon_.vlist[0].x = 0.0f;
    single_polygon_.vlist[0].y = 50.0f;
    single_polygon_.vlist[0].z = 0.0f;
    single_polygon_.vlist[0].w = 1.0f;

    single_polygon_.vlist[1].x = 50.0f;
    single_polygon_.vlist[1].y = -50.0;
    single_polygon_.vlist[1].z = 0.0f;
    single_polygon_.vlist[1].w = 1.0;

    single_polygon_.vlist[2].x = -50.0f;
    single_polygon_.vlist[2].y = -50.0f;
    single_polygon_.vlist[2].z = 0.0f;
    single_polygon_.vlist[2].w = 1.0f;

    single_polygon_.next = single_polygon_.prev = nullptr;

    float near_clip_z = 50.0f;
    float far_clip_z = 500.0f;
    float fov = 90.0f;
    KSR::Init_CAM4DV1(&camera_, KSR::CAM_MODEL_EULER, &camera_position_, &camera_direction_, nullptr,
        near_clip_z, far_clip_z, fov, static_cast<float>(wnd_render_area_width_), static_cast<float>(wnd_render_area_height_));
}

void Demo01App::RenderScene()
{
    // 初始化render list，即清空render list
    KSR::Reset_RENDERLIST4DV1(&render_list_);

    // 往render list插入多边形
    KSR::Insert_POLYF4DV1_RENDERLIST4DV1(&render_list_, &single_polygon_);

    // 根据绕y轴旋转的角度，对应生成旋转矩阵
    KSR::Build_XYZ_Rotation_MATRIX4X4(0, rotation_angle_y_, 0, &rotation_matrix_);

    // 递增旋转角度
    rotation_angle_y_ += 0.1f;

    if (rotation_angle_y_ >= 360.0f)
        rotation_angle_y_ = 0.0f;

    // 把render list中的多边形的顶点坐标，基于其自身的局部坐标系，进行绕Y轴旋转
    KSR::Transform_RENDERLIST4DV1(&render_list_, &rotation_matrix_, KSR::TRANSFORM_LOCAL_ONLY);

    // 把多边形坐标，从模型空间，变换到世界空间
    KSR::Model_To_World_RENDERLIST4DV1(&render_list_, &single_polygon_position_);

    // 构建view matrix，即把顶点从世界空间变换到观察空间的矩阵
    KSR::Build_CAM4DV1_Matrix_Euler(&camera_, KSR::CAM_ROT_SEQ_ZYX);

    // 把多边形坐标，从世界空间，变换到观察空间
    KSR::World_To_Camera_RENDERLIST4DV1(&render_list_, &camera_);

    // 把多边形坐标从观察空间，变换到透视投影空间
    KSR::Camera_To_Perspective_RENDERLIST4DV1(&render_list_, &camera_);

    // 把多边形坐标从透视投影空间，变换到屏幕空间
    KSR::Perspective_To_Screen_RENDERLIST4DV1(&render_list_, &camera_);

    // lock back buffer，开始画线
    KSR::DDraw_Lock_Back_Surface();

    // 渲染给定的多边形
    KSR::Draw_RENDERLIST4DV1_Wire16(&render_list_, KSR::back_buffer, KSR::back_lpitch);

    // 画线完毕，unlock back buffer
    KSR::DDraw_Unlock_Back_Surface();
}