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

#include "demo_03_app.h"
#include "ksr_light.h"

void Demo03App::InitScene()
{
    KSR::RGB16Bit = KSR::RGB16Bit565;

    float near_clip_z = 200.0f;
    float far_clip_z = 12000.0f;
    float fov = 120.0f;

    // initialize the camera with 90 FOV, normalized coordinates
    KSR::Init_CAM4DV1(&cam,      // the camera object
        KSR::CAM_MODEL_EULER, // the euler model
        &cam_pos,  // initial camera position
        &cam_dir,  // initial camera angles
        &cam_target,      // no target
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

void Demo03App::InitLights()
{
    KSR::RGBAV1 white, gray, black, red, green, blue;

    white.rgba = KSR::_RGBA32BIT(255, 255, 255, 0);
    gray.rgba = KSR::_RGBA32BIT(100, 100, 100, 0);
    black.rgba = KSR::_RGBA32BIT(0, 0, 0, 0);
    red.rgba = KSR::_RGBA32BIT(255, 0, 0, 0);
    green.rgba = KSR::_RGBA32BIT(0, 255, 0, 0);
    blue.rgba = KSR::_RGBA32BIT(0, 0, 255, 0);

    // ambient light
    KSR::Init_Light_LIGHTV1(KSR::AMBIENT_LIGHT_INDEX,
        KSR::LIGHTV1_STATE_ON,      // turn the light on
        KSR::LIGHTV1_ATTR_AMBIENT,  // ambient light type
        gray, black, black,    // color for ambient term only
        NULL, NULL,            // no need for pos or dir
        0.0f, 0.0f, 0.0f,                 // no need for attenuation
        0.0f, 0.0f, 0.0f);                // spotlight info NA


    KSR::VECTOR4D dlight_dir = { -1.0f,0.0f,-1.0f,0.0f };

    // directional light
    Init_Light_LIGHTV1(KSR::INFINITE_LIGHT_INDEX,
        KSR::LIGHTV1_STATE_ON,      // turn the light on
        KSR::LIGHTV1_ATTR_INFINITE, // infinite light type
        black, gray, black,    // color for diffuse term only
        NULL, &dlight_dir,     // need direction only
        0.0f, 0.0f, 0.0f,                 // no need for attenuation
        0.0f, 0.0f, 0.0f);                // spotlight info NA


    KSR::VECTOR4D plight_pos = { 0.0f,200.0f,0.0f,0.0f };

    // point light
    Init_Light_LIGHTV1(KSR::POINT_LIGHT_INDEX,
        KSR::LIGHTV1_STATE_ON,      // turn the light on
        KSR::LIGHTV1_ATTR_POINT,    // pointlight type
        black, green, black,   // color for diffuse term only
        &plight_pos, NULL,     // need pos only
        0.0f, 0.001f, 0.0f,              // linear attenuation only
        0.0f, 0.0f, 1.0f);                // spotlight info NA

    KSR::VECTOR4D slight_pos = { 0.0f,200.0f,0.0f,0.0f };
    KSR::VECTOR4D slight_dir = { -1.0f,0.0f,-1.0f,0.0f };

    // spot light
    Init_Light_LIGHTV1(KSR::SPOT_LIGHT_INDEX,
        KSR::LIGHTV1_STATE_ON,         // turn the light on
        KSR::LIGHTV1_ATTR_SPOTLIGHT2,  // spot light type 2
        black, red, black,      // color for diffuse term only
        &slight_pos, &slight_dir, // need pos only
        0.0f, 0.001f, 0.0f,                 // linear attenuation only
        0.0f, 0.0f, 1.0f);                   // spotlight powerfactor only
}

void Demo03App::UpdateLights()
{
    // move point light source in ellipse around game world
    KSR::lights[KSR::POINT_LIGHT_INDEX].pos.x = 4000.0f * cosf(plight_ang);
    KSR::lights[KSR::POINT_LIGHT_INDEX].pos.y = 200.0f;
    KSR::lights[KSR::POINT_LIGHT_INDEX].pos.z = 4000.0f * sinf(plight_ang);

    if ((plight_ang += 3) > 360)
        plight_ang = 0;

    // move spot light source in ellipse around game world
    KSR::lights[KSR::SPOT_LIGHT_INDEX].pos.x = 2000.0f * cosf(slight_ang);
    KSR::lights[KSR::SPOT_LIGHT_INDEX].pos.y = 200.0f;
    KSR::lights[KSR::SPOT_LIGHT_INDEX].pos.z = 2000.0f * sinf(slight_ang);

    if ((slight_ang -= 5) < 0)
        slight_ang = 360;
}

void Demo03App::OnKeyUp(SDL_Event& event)
{
    if (SDLK_UP == event.key.keysym.sym)
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

void Demo03App::RenderScene()
{
    // draw the sky
    KSR::Draw_Rectangle(0, 0, wnd_render_area_width_ - 1, wnd_render_area_height_ / 2, KSR::RGB16Bit(0, 140, 192), KSR::lpddsback);

    // draw the ground
    KSR::Draw_Rectangle(0, 0, wnd_render_area_width_ - 1, wnd_render_area_height_ / 2, KSR::RGB16Bit(103, 62, 3), KSR::lpddsback);

    KSR::Reset_RENDERLIST4DV1(&rend_list);

    UpdateLights();

    // generate camera matrix
    Build_CAM4DV1_Matrix_Euler(&cam, KSR::CAM_ROT_SEQ_ZYX);

    if (lighting_mode == 1)
        Light_OBJECT4DV1_World16(&obj, &cam, KSR::lights, 4);

    // insert the object into render list
    Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj, 0, lighting_mode);
}