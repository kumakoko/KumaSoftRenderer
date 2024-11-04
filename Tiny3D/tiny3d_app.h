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
    的构造函数
    @name: Tiny3DApp::Tiny3DApp
    @return: 
    *************************************************************************************/
    Tiny3DApp();

    /*********************************************************
    类App的析构函数

    @return
    *********************************************************/
    virtual ~Tiny3DApp();

    void InitRenderDevice();

    void InitializeGraphicSystem();

    void ShutdownGraphicSystem();

    void LockBackSurface();

    void UnlockBackSurface();

    void ProcessInput(SDL_Event& event);

    void OnKeyDown(SDL_Event& event);

    void OnKeyUp(SDL_Event& event);

    // 处理鼠标拖动事件
    void OnMouseDragging(SDL_Window* window, SDL_Event& e);

    void RenderScene();

    void Run();
protected:
    Device* g_Device;
    uint8_t* g_BackBuffer;    // 后台页面的首指针
    int g_BackSurfacePitch;
    SDL_Surface* g_BackSurface;
    SDL_Surface* g_ScreenSurface;
    SDL_Window* g_Window;
    uint32_t g_WindowRenderAreaWidth;
    uint32_t g_WindowRenderAreaHeight;
    const char* g_WindowTitle;
    bool g_IsRunning;
    bool g_IsMouseDragging;
    int g_MouseWndOffsetX;
    int g_MouseWndOffsetY;
    float g_Alpha = 0.0f;
    std::array<T3DVertex,8> g_BoxMesh;
};