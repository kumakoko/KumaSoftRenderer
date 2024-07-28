/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include <chrono>
#include "ksr_app.h"
#include "ksr_surface.h"
#include "ksr_timer.h"

namespace KSR
{
    App* App::s_instance_ = nullptr;

    App::App()
    {
        s_instance_ = this;
    }

    App::~App()
    {
        s_instance_ = nullptr;
        KSR::ShutdownGraphicSystem();
    }

    void App::InitRenderer(uint32_t wnd_render_area_width, uint32_t wnd_render_area_height, const char* wnd_title)
    {
        window_render_area_height_ = wnd_render_area_height;
        window_render_area_width_ = wnd_render_area_width;
        KSR::InitializeGraphicSystem(window_render_area_width_, window_render_area_height_, wnd_title);
    }

    void App::InitScene()
    {
    }

    void App::Run()
    {
        while (is_running_)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                ProcessInput(event);
                OnMouseDragging(KSR::gWindow, event);
            }

            KSR::Start_Clock();
            KSR::DDraw_Fill_Surface(KSR::lpddsback, 0);
            KSR::DDraw_Fill_Surface(KSR::gScreenSurface, 0);

            RenderScene();

            KSR::DDraw_Flip();
            KSR::Wait_Clock(std::chrono::milliseconds(33));
        }

        KSR::ShutdownGraphicSystem();
    }

    void App::RenderScene()
    {

    }

    void App::ProcessInput(SDL_Event& event)
    {
        if (event.type == SDL_QUIT)
        {
            is_running_ = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                is_running_ = false;
            }
        }
    }

    // 处理鼠标拖动事件
    void App::OnMouseDragging(SDL_Window* window, SDL_Event& event)
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            is_mouse_dragging_ = true;
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            int windowX, windowY;
            SDL_GetWindowPosition(window, &windowX, &windowY);
            mouse_wnd_offset_x_ = mouseX - windowX;
            mouse_wnd_offset_y_ = mouseY - windowY;
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            is_mouse_dragging_ = false;
        }
        else if (event.type == SDL_MOUSEMOTION && is_mouse_dragging_)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_SetWindowPosition(window, mouseX - mouse_wnd_offset_x_, mouseY - mouse_wnd_offset_y_);
        }
    }
}
