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
#include "ksr_aligned_class.h"

namespace KSR
{
    class alignas(16) App : public AlignedClass<App>
    {
    public:
        /*********************************************************
        类App的构造函数

        @return
        *********************************************************/
        App();

        /*********************************************************
        类App的析构函数

        @return
        *********************************************************/
        virtual ~App();

        /**************************************************************************************

        @name: KSR::App::InitRenderer
        @return: void
        @param: uint32_t wnd_width      窗口的宽度
        @param: uint32_t wnd_height     窗口的高度
        @param: const char * wnd_title  窗口的标题
        *************************************************************************************/
        virtual void InitRenderer(uint32_t wnd_width, uint32_t wnd_height, const char* wnd_title);

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
        
        @name: KSR::App::RenderScene
        @return: void
        *************************************************************************************/
        virtual void RenderScene();
    protected:
        static App* s_instance_;
    protected:
        /** * @brief 窗口的宽度 */
        uint32_t window_width_ = 0;

        /** * @brief 窗口的高度 */
        uint32_t window_height_ = 0;

        bool s_IsRunning = true;

        bool dragging = false;
        int offsetX;
        int offsetY;
    };
}