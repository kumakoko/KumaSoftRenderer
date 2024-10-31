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
#include "SDL.h"

class /*alignas(16)*/ Tiny3DApp/* : public AlignedClass<App>*/
{
public:
    /*********************************************************
    类App的构造函数

    @return
    *********************************************************/
    Tiny3DApp();

    /*********************************************************
    类App的析构函数

    @return
    *********************************************************/
    virtual ~Tiny3DApp();

    /**************************************************************************************

    @name: KSR::App::InitRenderer
    @return: void
    @param: uint32_t wnd_width      窗口的渲染区域的宽度
    @param: uint32_t wnd_height     窗口的渲染区域的高度
    @param: const char * wnd_title  窗口的标题
    *************************************************************************************/
    virtual void InitRenderer(uint32_t wnd_render_area_width, uint32_t wnd_render_area_height, const char* wnd_title);

    /**************************************************************************************
    初始化demo场景
    @name: KSR::App::InitScene
    @return: void
    *************************************************************************************/
    virtual void InitScene();

    /**************************************************************************************
    应用程序运行的主循环
    @name: KSR::App::Run
    @return: void
    *************************************************************************************/
    void Run();
protected:
    /**************************************************************************************

    @name: KSR::App::ProcessInput
    @return: void
    @param: SDL_Event & event
    *************************************************************************************/
    virtual void ProcessInput(SDL_Event& event);

    /**************************************************************************************
    处理鼠标拖动事件
    @name: KSR::App::OnMouseDragging
    @return: void
    @param: SDL_Window * window
    @param: SDL_Event & event
    *************************************************************************************/
    virtual void OnMouseDragging(SDL_Window* window, SDL_Event& event);

    /**************************************************************************************

    @name: KSR::App::OnKeyDown
    @return: void
    @param: SDL_Event & event
    *************************************************************************************/
    virtual void OnKeyDown(SDL_Event& event);

    /**************************************************************************************

    @name: KSR::App::OnKeyUp
    @return: void
    @param: SDL_Event & event
    *************************************************************************************/
    virtual void OnKeyUp(SDL_Event& event);

    /**************************************************************************************

    @name: KSR::App::RenderScene
    @return: void
    *************************************************************************************/
    virtual void RenderScene();

    void Get16BitSufaceRGBMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask);

    void ShutdownGraphicSystem();
protected:
    static Tiny3DApp* s_instance_;
protected:
    /** * @brief 窗口中的渲染区域的的宽度，即窗口中的“client area”，不包含标题栏，状态栏等等的那个渲染区域 */
    uint32_t window_render_area_width_ = 0;

    /** * @brief 窗口中的渲染区域的的高度 */
    uint32_t window_render_area_height_ = 0;

    /** * @brief 程序是否在运行中 */
    bool is_running_ = true;

    /** * @brief 程序是否在用鼠标拖拽着窗口 */
    bool is_mouse_dragging_ = false;

    /** * @brief 当鼠标点击窗口，开始拖拽时，鼠标点坐标的x分量，和窗口位置坐标的x分量的差值 */
    int mouse_wnd_offset_x_;

    /** * @brief 当鼠标点击窗口，开始拖拽时，鼠标点坐标的y分量，和窗口位置坐标的y分量的差值 */
    int mouse_wnd_offset_y_;

    SDL_Surface* gScreenSurface = nullptr;
    SDL_Surface* lpddsprimary = nullptr;
    SDL_Surface* lpddsback = nullptr;
    SDL_Window* gWindow = nullptr;

};