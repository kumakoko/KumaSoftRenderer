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

#include "tiny3d_app.h"

//void Demo01App::InitScene()
//{
//}
//
//void Demo01App::RenderScene()
//{
//   // 画线完毕，unlock back buffer
//    KSR::DDraw_Unlock_Back_Surface();
//}

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

Tiny3DApp* Tiny3DApp::s_instance_ = nullptr;

Tiny3DApp::Tiny3DApp()
{
    s_instance_ = this;
}

Tiny3DApp::~Tiny3DApp()
{
    s_instance_ = nullptr;
    SDL_FreeSurface(lpddsprimary);
    SDL_FreeSurface(lpddsback);
    SDL_DestroyWindow(gWindow);
    lpddsprimary = nullptr;
    lpddsback = nullptr;
    gWindow = nullptr;
    SDL_Quit();
}


//uint32_t WINDOW_RENDER_AREA_WIDTH = 640;
//uint32_t WINDOW_RENDER_AREA_HEIGHT = 480;

void Tiny3DApp::InitRenderer(uint32_t wnd_render_area_width, uint32_t wnd_render_area_height, const char* wnd_title)
{
    window_render_area_height_ = wnd_render_area_height;
    window_render_area_width_ = wnd_render_area_width;

   // WINDOW_RENDER_AREA_WIDTH = sw;
  //  WINDOW_RENDER_AREA_HEIGHT = sh;
    max_clip_x = window_render_area_width_ - 1;
    max_clip_y = window_render_area_height_ - 1;

    gWindow = SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, SDL_WINDOW_SHOWN);

    gScreenSurface = SDL_GetWindowSurface(gWindow);
    if (gScreenSurface == nullptr) {
        // throw 异常
         // printf("Window surface could not be retrieved! SDL_Error: %s\n", SDL_GetError());
        // return false;
    }

    //lpddsprimary = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    uint32_t rmask, gmask, bmask, amask;
    Get16BitSufaceRGBMask(rmask, gmask, bmask, amask);
    lpddsprimary = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, 16, rmask, gmask, bmask, amask);

    if (lpddsprimary == nullptr) {
        //printf("Surfaces could not be created! SDL_Error: %s\n", SDL_GetError());
       // return false;
    }

    lpddsback = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, 16, rmask, gmask, bmask, amask);
    //lpddsback = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (lpddsback == nullptr) {
        //  printf("Surfaces could not be created! SDL_Error: %s\n", SDL_GetError());
        //  return false;
    }
}

void Tiny3DApp::Get16BitSufaceRGBMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask)
{
    // 定义像素格式掩码
    // 判断系统字节序以设置掩码
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        rmask = 0xF800;
        gmask = 0x07E0;
        bmask = 0x001F;
        amask = 0x0000;
    }
    else
    {
        rmask = 0x1F << 11;
        gmask = 0x3F << 5;
        bmask = 0x1F;
        amask = 0x0000;
    }
}

void Tiny3DApp::InitScene()
{
}

void Tiny3DApp::Run()
{
    while (is_running_)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ProcessInput(event);
            OnMouseDragging(KSR::gWindow, event);
        }

        //   KSR::Start_Clock();


           //void DDraw_Fill_Surface(SDL_Surface * lpdds, uint32_t color, SDL_Rect * client/* = nullptr*/)
           //{
           //    SDL_FillRect(lpdds, client, color);
           //}
        SDL_FillRect(lpddsback, nullptr, 0);
        SDL_FillRect(gScreenSurface, nullptr, 0);


        //   KSR::DDraw_Fill_Surface(KSR::lpddsback, 0);
        //   KSR::DDraw_Fill_Surface(KSR::gScreenSurface, 0);

        RenderScene();

        SDL_BlitSurface(lpddsback, nullptr, gScreenSurface, nullptr);
        // Update the window surface
        SDL_UpdateWindowSurface(gWindow);



        //     KSR::Wait_Clock(std::chrono::milliseconds(33));
    }

    ShutdownGraphicSystem();
}

void Tiny3DApp::RenderScene()
{

}

void Tiny3DApp::ShutdownGraphicSystem()
{
    SDL_FreeSurface(lpddsprimary);
    SDL_FreeSurface(lpddsback);
    SDL_DestroyWindow(gWindow);
    lpddsprimary = nullptr;
    lpddsback = nullptr;
    gWindow = nullptr;
    SDL_Quit();
}

void App::ProcessInput(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        is_running_ = false;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        OnKeyDown(event);
    }
    else if (event.type == SDL_KEYUP)
    {
        OnKeyUp(event);
    }
}

void App::OnKeyDown(SDL_Event& event)
{
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
        is_running_ = false;
    }
}

void App::OnKeyUp(SDL_Event& event)
{

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
