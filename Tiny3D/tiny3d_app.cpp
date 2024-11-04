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
    g_BoxMesh[0] = { {  1, -1,  1, 1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 };
    g_BoxMesh[1] = { { -1, -1,  1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 };
    g_BoxMesh[2] = { { -1,  1,  1, 1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 };
    g_BoxMesh[3] = { {  1,  1,  1, 1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 };
    g_BoxMesh[4] = { {  1, -1, -1, 1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 };
    g_BoxMesh[5] = { { -1, -1, -1, 1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 };
    g_BoxMesh[6] = { { -1,  1, -1, 1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 };
    g_BoxMesh[7] = { {  1,  1, -1, 1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 };

    g_BackBuffer = nullptr;    // 后台页面的首指针
    g_BackSurfacePitch = 0;

    g_BackSurface = nullptr;
    g_ScreenSurface = nullptr;
    g_Window = nullptr;
    g_WindowRenderAreaWidth = 1024;
    g_WindowRenderAreaHeight = 768;
    g_WindowTitle = "Tiny3D";
    g_IsRunning = true;
    g_IsMouseDragging = false;
    g_MouseWndOffsetX = 0;
    g_MouseWndOffsetY = 0;
    g_Device = nullptr;
    g_Alpha = 0.0f;
}

Tiny3DApp::~Tiny3DApp()
{
}

void Tiny3DApp::InitializeGraphicSystem()
{
    g_Window = SDL_CreateWindow(g_WindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        g_WindowRenderAreaWidth, g_WindowRenderAreaHeight, SDL_WINDOW_SHOWN);

    // 初始化 SDL_image 并设置支持的图片格式
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG )))
    {
        //std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    g_ScreenSurface = SDL_GetWindowSurface(g_Window);

    if (g_ScreenSurface == nullptr)
    {
        throw Error(fmt::format("Window surface could not be retrieved! SDL_Error: {0}\n", SDL_GetError()));
    }

    g_BackSurface = SDL_CreateRGBSurface(0, g_WindowRenderAreaWidth, g_WindowRenderAreaHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (g_BackSurface == nullptr)
    {
        throw Error(fmt::format("Surfaces could not be created! SDL_Error: {0}\n", SDL_GetError()));
    }
}


void Tiny3DApp::InitRenderDevice()
{
    g_Device = new Device();
    g_Device->Initialize(g_WindowRenderAreaWidth, g_WindowRenderAreaHeight);
    g_Device->ResetCamera(3, 0, 0);
    //g_Device->InitTexture();
    g_Device->CreateTextureFromFile("assets/images/wood_box.jpg");
}

void Tiny3DApp::ShutdownGraphicSystem()
{
    SDL_FreeSurface(g_BackSurface);
    SDL_DestroyWindow(g_Window);
    g_BackSurface = nullptr;
    g_Window = nullptr;
    SDL_Quit();
}

void Tiny3DApp::LockBackSurface()
{
    if (g_BackBuffer)
    {
        return;
    }

    //锁定surface
    int ret = SDL_LockSurface(g_BackSurface);

    if (0 != ret)
    {
        // 在这里抛出异常
    }

    g_BackBuffer = reinterpret_cast<uint8_t*>(g_BackSurface->pixels);
    g_BackSurfacePitch = g_BackSurface->pitch;
}

void Tiny3DApp::UnlockBackSurface()
{
    if (nullptr == g_BackBuffer)
        return;

    SDL_UnlockSurface(g_BackSurface);
    g_BackBuffer = nullptr;
    g_BackSurfacePitch = 0;
}

void Tiny3DApp::ProcessInput(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        g_IsRunning = false;
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

void Tiny3DApp::OnKeyDown(SDL_Event& e)
{
    if (e.key.keysym.sym == SDLK_ESCAPE)
    {
        g_IsRunning = false;
    }

    SDL_Keycode sym = e.key.keysym.sym;

    if (SDLK_LEFT == sym)
    {
        g_Alpha += 0.01f;
    }
    else if (SDLK_RIGHT == sym)
    {
        g_Alpha -= 0.01f;
    }
}

void Tiny3DApp::OnKeyUp(SDL_Event& e)
{

}

// 处理鼠标拖动事件
void Tiny3DApp::OnMouseDragging(SDL_Window* window, SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        g_IsMouseDragging = true;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int windowX, windowY;
        SDL_GetWindowPosition(window, &windowX, &windowY);
        g_MouseWndOffsetX = mouseX - windowX;
        g_MouseWndOffsetY = mouseY - windowY;
    }
    else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
    {
        g_IsMouseDragging = false;
    }
    else if (e.type == SDL_MOUSEMOTION && g_IsMouseDragging)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_SetWindowPosition(window, mouseX - g_MouseWndOffsetX, mouseY - g_MouseWndOffsetY);
    }
}

void Tiny3DApp::RenderScene()
{
    LockBackSurface();

    g_Device->ResetZBuffer();
    g_Device->SetFrameBufer(g_BackBuffer);

    g_Device->DrawBox(g_Alpha, g_BoxMesh.data());

    UnlockBackSurface();
}

void Tiny3DApp::Run()
{
    while (g_IsRunning)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ProcessInput(event);
            OnMouseDragging(g_Window, event);
        }

        g_Device->ResetCamera(3.5, 0, 0);

        //Start_Clock();
        SDL_FillRect(g_BackSurface, nullptr, 0xFFc0c0c0); //ARGB
        SDL_FillRect(g_ScreenSurface, nullptr, 0);

        RenderScene();

        SDL_BlitSurface(g_BackSurface, nullptr, g_ScreenSurface, nullptr);
        SDL_UpdateWindowSurface(g_Window);

        g_BackBuffer = nullptr;
        g_BackSurfacePitch = 0; // 因为交换过，所以这些都需要置空
        //  Wait_Clock(std::chrono::milliseconds(33));
    }
}