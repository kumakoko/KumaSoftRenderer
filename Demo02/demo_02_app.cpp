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

void Demo02App::RenderScene()
{
    // reset the object (this only matters for backface and object removal)
    KSR::Reset_OBJECT4DV1(&obj);

    // reset angles
    x_ang = z_ang = 0;

    // generate rotation matrix around y axis
    KSR::Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

    // rotate the local coords of single polygon in renderlist
    KSR::Transform_OBJECT4DV1(&obj, &mrot, KSR::TRANSFORM_LOCAL_ONLY, 1);

    // perform local/model to world transform
    KSR::Model_To_World_OBJECT4DV1(&obj);

    // generate camera matrix
    KSR::Build_CAM4DV1_Matrix_Euler(&cam, KSR::CAM_ROT_SEQ_ZYX);

    // remove backfaces
    KSR::Remove_Backfaces_OBJECT4DV1(&obj, &cam);

    // apply world to camera transform
    KSR::World_To_Camera_OBJECT4DV1(&obj, &cam);

    // apply camera to perspective transformation
    KSR::Camera_To_Perspective_OBJECT4DV1(&obj, &cam);

    // apply screen transform
    KSR::Perspective_To_Screen_OBJECT4DV1(&obj, &cam);

    // lock the back buffer
    KSR::DDraw_Lock_Back_Surface();

    // render the object
    KSR::Draw_OBJECT4DV1_Wire16(&obj, KSR::back_buffer, KSR::back_lpitch);

    // unlock the back buffer
    KSR::DDraw_Unlock_Back_Surface();
}