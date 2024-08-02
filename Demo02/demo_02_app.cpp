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

#include "demo_02_app.h"

void Demo02App::InitScene()
{
    KSR::RGB16Bit = KSR::RGB16Bit565;

    float near_clip_z = 50.0f;
    float far_clip_z = 500.0f;
    float fov = 90.0f;

    // initialize the camera with 90 FOV, normalized coordinates
    KSR::Init_CAM4DV1(&cam,      // the camera object
        KSR::CAM_MODEL_EULER, // the euler model
        &cam_pos,  // initial camera position
        &cam_dir,  // initial camera angles
        nullptr,      // no target
        near_clip_z,      // near and far clipping planes
        far_clip_z,
        fov,      // field of view in degrees
        static_cast<float>(wnd_render_area_width_), static_cast<float>(wnd_render_area_height_));

    KSR::Load_OBJECT4DV1_PLG(&obj, "assets/models/cube_2.plg", &vscale, &vpos, &vrot);

    // set the position of the cube in the world
    obj.world_pos.x = 0;
    obj.world_pos.y = 0;
    obj.world_pos.z = 100;
}

void Demo02App::OnKeyUp(SDL_Event& event)
{
    if (SDLK_UP == event.key.keysym.sym )
    {
        x_ang = 0.01f;
    }
    else if (SDLK_DOWN == event.key.keysym.sym)
    {
        x_ang = -0.01f;
    }

    if (SDLK_LEFT == event.key.keysym.sym)
    {
        z_ang = 0.01f;
    }
    else if (SDLK_RIGHT == event.key.keysym.sym)
    {
        z_ang = -0.01f;
    }
}

void Demo02App::RenderScene()
{
    // reset the object (this only matters for backface and object removal)
    KSR::Reset_OBJECT4DV1(&obj);

    // reset angles
    //x_ang = z_ang = 0;

    // 根据绕轴旋转的角度，构建出模型旋转的欧拉角
    KSR::Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

    // 根据构建出来的旋转矩阵，旋转model object
    KSR::Transform_OBJECT4DV1(&obj, &mrot, KSR::TRANSFORM_LOCAL_ONLY, true);

    // 把模型从局部坐标系变换到世界坐标系去
    KSR::Model_To_World_OBJECT4DV1(&obj);

    // 构建camera matrix，用来执行world->view变换
    KSR::Build_CAM4DV1_Matrix_Euler(&cam, KSR::CAM_ROT_SEQ_ZYX);

    // 模型网格的背面剔除
    KSR::Remove_Backfaces_OBJECT4DV1(&obj, &cam);

    // 把模型从世界坐标系变换到观察空间坐标系去
    KSR::World_To_Camera_OBJECT4DV1(&obj, &cam);

    // 把模型从观察空间坐标系变换到投影坐标系去
    KSR::Camera_To_Perspective_OBJECT4DV1(&obj, &cam);

    // 把模型从投影坐标系变换到屏幕坐标系去
    KSR::Perspective_To_Screen_OBJECT4DV1(&obj, &cam);

    // 锁定back buffer，准备写屏
    KSR::DDraw_Lock_Back_Surface();

    // 渲染model buffer到back buffer
    KSR::Draw_OBJECT4DV1_Wire16(&obj, KSR::back_buffer, KSR::back_lpitch);

    // 解锁back buffer
    KSR::DDraw_Unlock_Back_Surface();
}