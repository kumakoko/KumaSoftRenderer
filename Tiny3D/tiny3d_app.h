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
#include <array>
#include "SDL.h"

#include "tiny3d_aligned_class.h"
#include "tiny3d_geometry.h"
#include "tiny3d_device.h"

class alignas(16) Tiny3DApp : public AlignedClass<Tiny3DApp>
{
public:
    /**************************************************************************************
    构造函数
    @name: Tiny3DApp::Tiny3DApp
    @return: 
    *************************************************************************************/
    Tiny3DApp();

    /*********************************************************
    类App的析构函数

    @return
    *********************************************************/
    virtual ~Tiny3DApp();

    /**************************************************************************************
    
    @name: Tiny3DApp::InitRenderDevice
    @return: void
    *************************************************************************************/
    void InitRenderDevice();

    /**************************************************************************************
    
    @name: Tiny3DApp::InitializeGraphicSystem
    @return: void
    *************************************************************************************/
    void InitializeGraphicSystem();

    /**************************************************************************************
    
    @name: Tiny3DApp::ShutdownGraphicSystem
    @return: void
    *************************************************************************************/
    void ShutdownGraphicSystem();

    /**************************************************************************************

    @name: Tiny3DApp::Run
    @return: void
    *************************************************************************************/
    void Run();

    /**************************************************************************************
    
    @name: Tiny3DApp::DestroyRenderDevice
    @return: void
    *************************************************************************************/
    void DestroyRenderDevice();

private:
    /**************************************************************************************
    
    @name: Tiny3DApp::LockBackSurface
    @return: void
    *************************************************************************************/
    void LockBackSurface();

    /**************************************************************************************
    
    @name: Tiny3DApp::UnlockBackSurface
    @return: void
    *************************************************************************************/
    void UnlockBackSurface();

    /**************************************************************************************
    
    @name: Tiny3DApp::ProcessInput
    @return: void
    @param: SDL_Event & e
    *************************************************************************************/
    void ProcessInput(SDL_Event& e);

    /**************************************************************************************
    
    @name: Tiny3DApp::OnKeyDown
    @return: void
    @param: SDL_Event & e
    *************************************************************************************/
    void OnKeyDown(SDL_Event& e);

    /**************************************************************************************
    
    @name: Tiny3DApp::OnKeyUp
    @return: void
    @param: SDL_Event & e
    *************************************************************************************/
    void OnKeyUp(SDL_Event& e);

    /**************************************************************************************
    处理鼠标拖动事件
    @name: Tiny3DApp::OnMouseDragging
    @return: void
    @param: SDL_Window * window
    @param: SDL_Event & e
    *************************************************************************************/
    void OnMouseDragging(SDL_Window* window, SDL_Event& e);

    /**************************************************************************************
    
    @name: Tiny3DApp::RenderScene
    @return: void
    *************************************************************************************/
    void RenderScene();
private:
    Device* render_device_;
    uint8_t* back_buffer_pointer_;    // 后台页面的首指针
    SDL_Surface* back_surface_;
    SDL_Surface* screen_surface_;
    SDL_Window* window_;
    uint32_t wnd_render_area_width_;
    uint32_t wnd_render_area_height_;
    const char* window_title_;
    bool is_running_;
    bool is_mouse_dragging_;
    int mouse_wnd_offset_x_;
    int mouse_wnd_offset_y_;
    float box_rotation_delta_ = 0.0f;
    std::array<T3DVertex,8> box_mesh_;
};