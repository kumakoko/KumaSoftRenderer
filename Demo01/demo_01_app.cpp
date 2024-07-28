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
    s_SinglePolygon.state = KSR::POLY4DV1_STATE_ACTIVE;
    s_SinglePolygon.attr = 0;
    s_SinglePolygon.color = 0xFFFF;//RGB16Bit(0, 255, 0);

    s_SinglePolygon.vlist[0].x = 0.0f;
    s_SinglePolygon.vlist[0].y = 50.0f;
    s_SinglePolygon.vlist[0].z = 0.0f;
    s_SinglePolygon.vlist[0].w = 1.0f;

    s_SinglePolygon.vlist[1].x = 50.0f;
    s_SinglePolygon.vlist[1].y = -50.0;
    s_SinglePolygon.vlist[1].z = 0.0f;
    s_SinglePolygon.vlist[1].w = 1.0;

    s_SinglePolygon.vlist[2].x = -50.0f;
    s_SinglePolygon.vlist[2].y = -50.0f;
    s_SinglePolygon.vlist[2].z = 0.0f;
    s_SinglePolygon.vlist[2].w = 1.0f;

    s_SinglePolygon.next = s_SinglePolygon.prev = nullptr;

    KSR::Init_CAM4DV1(&s_Camera, KSR::CAM_MODEL_EULER, &s_CameraPosition, &s_CameraDirection, nullptr,
        50.0f, 500.0f, 90.0f, static_cast<float>(s_ScreenWidth), static_cast<float>(s_ScreenHeight));
}

void Demo01App::RenderScene()
{
    // 初始化render list，即清空render list
    KSR::Reset_RENDERLIST4DV1(&s_RenderList);

    // insert polygon into the renderlist
    KSR::Insert_POLYF4DV1_RENDERLIST4DV1(&s_RenderList, &s_SinglePolygon);

    // generate rotation matrix around y axis
    KSR::Build_XYZ_Rotation_MATRIX4X4(0, s_RotationAngleY, 0, &s_RotationMatrix);

    // rotate polygon slowly
    s_RotationAngleY += 0.1f;

    if (s_RotationAngleY >= 360.0f)
        s_RotationAngleY = 0.0f;

    // 把render list中的多边形的顶点坐标，基于其自身的局部坐标系，进行绕Y轴旋转
    KSR::Transform_RENDERLIST4DV1(&s_RenderList, &s_RotationMatrix, KSR::TRANSFORM_LOCAL_ONLY);

    // 把多边形坐标，从模型空间，变换到世界空间
    KSR::Model_To_World_RENDERLIST4DV1(&s_RenderList, &s_SinglePolygonPosition);

    // 构建view matrix，即把顶点从世界空间变换到观察空间的矩阵
    KSR::Build_CAM4DV1_Matrix_Euler(&s_Camera, KSR::CAM_ROT_SEQ_ZYX);

    // 把多边形坐标，从世界空间，变换到观察空间
    KSR::World_To_Camera_RENDERLIST4DV1(&s_RenderList, &s_Camera);

    // 把多边形坐标从观察空间，变换到透视投影空间
    KSR::Camera_To_Perspective_RENDERLIST4DV1(&s_RenderList, &s_Camera);

    // apply screen transform
    KSR::Perspective_To_Screen_RENDERLIST4DV1(&s_RenderList, &s_Camera);

    // lock the back buffer
    KSR::DDraw_Lock_Back_Surface();

    // render the polygon list
    KSR::Draw_RENDERLIST4DV1_Wire16(&s_RenderList, KSR::back_buffer, KSR::back_lpitch);

    // unlock the back buffer
    KSR::DDraw_Unlock_Back_Surface();
}