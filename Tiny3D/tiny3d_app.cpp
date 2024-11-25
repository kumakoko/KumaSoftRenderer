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
#include <chrono>

#include "SDL_image.h"
#include "fmt/format.h"

#include "tiny3d_app.h"
#include "tiny3d_error.h"

Tiny3DApp::Tiny3DApp()
{
    box_mesh_[0] = { {  1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 };
    box_mesh_[1] = { { -1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 };
    box_mesh_[2] = { { -1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 };
    box_mesh_[3] = { {  1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 };
    box_mesh_[4] = { {  1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 };
    box_mesh_[5] = { { -1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 };
    box_mesh_[6] = { { -1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 };
    box_mesh_[7] = { {  1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 };

    back_buffer_pointer_ = nullptr;    // 后台页面的首指针

    back_surface_ = nullptr;
    screen_surface_ = nullptr;
    window_ = nullptr;
    wnd_render_area_width_ = 1024;
    wnd_render_area_height_ = 768;
    window_title_ = "Tiny3D";
    is_running_ = true;
    is_mouse_dragging_ = false;
    mouse_wnd_offset_x_ = 0;
    mouse_wnd_offset_y_ = 0;
    render_device_ = nullptr;
    box_rotation_delta_ = 0.0f;
}

Tiny3DApp::~Tiny3DApp()
{
}

void Tiny3DApp::InitializeGraphicSystem()
{
    window_ = SDL_CreateWindow(window_title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        wnd_render_area_width_, wnd_render_area_height_, SDL_WINDOW_SHOWN);

    // 初始化 SDL_image 并设置支持的图片格式
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG )))
    {
        //std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    screen_surface_ = SDL_GetWindowSurface(window_);

    if (screen_surface_ == nullptr)
    {
        throw Error(fmt::format("Window surface could not be retrieved! SDL_Error: {0}\n", SDL_GetError()));
    }

    back_surface_ = SDL_CreateRGBSurface(0, wnd_render_area_width_, wnd_render_area_height_, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (back_surface_ == nullptr)
    {
        throw Error(fmt::format("Surfaces could not be created! SDL_Error: {0}\n", SDL_GetError()));
    }
}

void Tiny3DApp::InitRenderDevice()
{
    render_device_ = new Device();
    render_device_->Initialize(wnd_render_area_width_, wnd_render_area_height_);
    render_device_->ResetCamera(3, 0, 0);
    render_device_->CreateTextureFromFile("assets/images/wood_box.jpg");
}

void Tiny3DApp::ShutdownGraphicSystem()
{
    SDL_FreeSurface(back_surface_);
    SDL_DestroyWindow(window_);
    back_surface_ = nullptr;
    window_ = nullptr;
    SDL_Quit();
}

void Tiny3DApp::DestroyRenderDevice()
{
    if (nullptr != render_device_)
    {
        delete render_device_;
        render_device_ = nullptr;
    }
}

void Tiny3DApp::LockBackSurface()
{
    if (back_buffer_pointer_)
    {
        return;
    }

    //锁定surface
    int ret = SDL_LockSurface(back_surface_);

    if (0 != ret)
    {
        // 在这里抛出异常
    }

    back_buffer_pointer_ = reinterpret_cast<uint8_t*>(back_surface_->pixels);
}

void Tiny3DApp::UnlockBackSurface()
{
    if (nullptr == back_buffer_pointer_)
        return;

    SDL_UnlockSurface(back_surface_);
    back_buffer_pointer_ = nullptr;
}

void Tiny3DApp::ProcessInput(SDL_Event& e)
{
    if (e.type == SDL_QUIT)
    {
        is_running_ = false;
    }
    else if (e.type == SDL_KEYDOWN)
    {
        OnKeyDown(e);
    }
    else if (e.type == SDL_KEYUP)
    {
        OnKeyUp(e);
    }
}

void Tiny3DApp::OnKeyDown(SDL_Event& e)
{
    if (e.key.keysym.sym == SDLK_ESCAPE)
    {
        is_running_ = false;
    }

    SDL_Keycode sym = e.key.keysym.sym;

    if (SDLK_LEFT == sym)
    {
        box_rotation_delta_ += 0.05f;
    }
    else if (SDLK_RIGHT == sym)
    {
        box_rotation_delta_ -= 0.05f;
    }
}

void Tiny3DApp::OnKeyUp(SDL_Event& e)
{
    SDL_Keycode sym = e.key.keysym.sym;

    switch (sym)
    {
    case SDLK_F1:
        render_device_->set_render_state(RENDER_STATE_WIREFRAME);
        break;
    case SDLK_F2:
        render_device_->set_render_state(RENDER_STATE_COLOR);
        break;
    case SDLK_F3:
        render_device_->set_render_state(RENDER_STATE_TEXTURE);
        break;
    }
}

// 处理鼠标拖动事件
void Tiny3DApp::OnMouseDragging(SDL_Window* window, SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        is_mouse_dragging_ = true;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int windowX, windowY;
        SDL_GetWindowPosition(window, &windowX, &windowY);
        mouse_wnd_offset_x_ = mouseX - windowX;
        mouse_wnd_offset_y_ = mouseY - windowY;
    }
    else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
    {
        is_mouse_dragging_ = false;
    }
    else if (e.type == SDL_MOUSEMOTION && is_mouse_dragging_)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_SetWindowPosition(window, mouseX - mouse_wnd_offset_x_, mouseY - mouse_wnd_offset_y_);
    }
}

void Tiny3DApp::RenderScene()
{
    LockBackSurface();

    render_device_->ResetZBuffer();

    render_device_->SetFrameBufer(back_buffer_pointer_);

    render_device_->DrawBox(box_rotation_delta_, box_mesh_.data());

    UnlockBackSurface();
}

void Tiny3DApp::Run()
{
    while (is_running_)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ProcessInput(event);
            OnMouseDragging(window_, event);
        }

        render_device_->ResetCamera(3.5, 0, 0);

        SDL_FillRect(back_surface_, nullptr, 0xFFc0c0c0); //ARGB
        SDL_FillRect(screen_surface_, nullptr, 0);

        RenderScene();

        SDL_BlitSurface(back_surface_, nullptr, screen_surface_, nullptr);
        SDL_UpdateWindowSurface(window_);

        back_buffer_pointer_ = nullptr;
    }
}