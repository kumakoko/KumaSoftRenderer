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
#include "ksr_model_object.h"

class alignas(16) Demo03App : public KSR::App
{
public:
    virtual void InitScene() override;
    virtual void RenderScene() override;
protected:
    virtual void OnKeyUp(SDL_Event& event) override;
protected:
    void InitLights();
    void UpdateLights();
private:
    // initialize camera position and direction
    KSR::POINT4D  cam_pos = { 0.0f,0.0f,0.0f,1.0f };
    KSR::POINT4D  cam_target = { 0.0f,0.0f,0.0f,1.0f };
    KSR::VECTOR4D cam_dir = { 0.0f,0.0f,0.0f,1.0f };

    // all your initialization code goes here...
    KSR::VECTOR4D vscale = { 5.0f,5.0f,5.0f,1.0f };
    KSR::VECTOR4D vpos = { 0.0f,0.0f,0.0f,1.0f };
    KSR::VECTOR4D vrot = { 0.0f,0.0f,0.0f,1.0f };
    KSR::MATRIX4X4 mrot; // general rotation matrix

    KSR::CAM4DV1    cam;     // the single camera
    KSR::OBJECT4DV1 obj;     // used to hold our cube mesh     

    uint32_t wnd_render_area_width_ = 1024;
    uint32_t wnd_render_area_height_ = 768;

    float x_ang = 0.0f;
    float y_ang = 0.0f;
    float z_ang = 0.0f;

    KSR::RENDERLIST4DV1 rend_list; // the render list

    float plight_ang = 0.0f;
    float slight_ang = 0.0f; // angles for light motion

    int lighting_mode;
};