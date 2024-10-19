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
#include <string>
#include <cstdint>
#include <exception>

#include "fmt/format.h"

#include "ksr_string_convertor.h"
#include "ksr_message_box.h"
#include "ksr_error.h"

#include "tiny3d_math.h"
#include "tiny3d.h"

uint8_t* g_BackBuffer = nullptr;    // 后台页面的首指针
int g_BackSurfacePitch = 0;
SDL_Surface* lpddsprimary = nullptr;
SDL_Surface* g_BackSurface = nullptr;
SDL_Surface* g_ScreenSurface = nullptr;
SDL_Window* g_Window = nullptr;
uint32_t g_WindowRenderAreaWidth = 1024;
uint32_t g_WindowRenderAreaHeight = 768;
const char* g_WindowTitle = "Tiny3D";
bool g_IsRunning = true;
bool g_IsMouseDragging = false;
int g_MouseWndOffsetX = 0;
int g_MouseWndOffsetY = 0;

void InitializeGraphicSystem()
{
    g_Window = SDL_CreateWindow(g_WindowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        g_WindowRenderAreaWidth, g_WindowRenderAreaHeight, SDL_WINDOW_SHOWN);

    g_ScreenSurface = SDL_GetWindowSurface(g_Window);

    if (g_ScreenSurface == nullptr)
    {
        throw KSR::Error(fmt::format("Window surface could not be retrieved! SDL_Error: {0}\n", SDL_GetError()));
    }

    g_BackSurface = SDL_CreateRGBSurface(0, g_WindowRenderAreaWidth, g_WindowRenderAreaHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    if (g_BackSurface == nullptr)
    {
        throw KSR::Error(fmt::format("Surfaces could not be created! SDL_Error: {0}\n", SDL_GetError()));
    }
}

void ShutdownGraphicSystem()
{
    SDL_FreeSurface(g_BackSurface);
    SDL_DestroyWindow(g_Window);
    g_BackSurface = nullptr;
    g_Window = nullptr;
    SDL_Quit();
}

void LockBackSurface()
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

void UnlockBackSurface()
{
    if (nullptr == g_BackBuffer)
        return;

    SDL_UnlockSurface(g_BackSurface);
    g_BackBuffer = nullptr;
    g_BackSurfacePitch = 0;
}

void ProcessInput(SDL_Event& event)
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

void OnKeyDown(SDL_Event& event)
{
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
        g_IsRunning = false;
    }
}

void OnKeyUp(SDL_Event& event)
{

}

// 处理鼠标拖动事件
void OnMouseDragging(SDL_Window* window, SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        g_IsMouseDragging = true;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int windowX, windowY;
        SDL_GetWindowPosition(window, &windowX, &windowY);
        g_MouseWndOffsetX = mouseX - windowX;
        g_MouseWndOffsetY = mouseY - windowY;
    }
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
        g_IsMouseDragging = false;
    }
    else if (event.type == SDL_MOUSEMOTION && g_IsMouseDragging)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_SetWindowPosition(window, mouseX - g_MouseWndOffsetX, mouseY - g_MouseWndOffsetY);
    }
}

void RenderScene()
{
    LockBackSurface();

    UnlockBackSurface();
}

int main(int argc, char* argv[])
{
    try
    {
        InitializeGraphicSystem();

        while (g_IsRunning)
        {
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                ProcessInput(event);
                OnMouseDragging(g_Window, event);
            }

            //KSR::Start_Clock();
            SDL_FillRect(g_BackSurface, nullptr, 0);
            SDL_FillRect(g_ScreenSurface, nullptr, 0);

            RenderScene();
            SDL_BlitSurface(g_BackSurface, nullptr, g_ScreenSurface, nullptr);
            SDL_UpdateWindowSurface(g_Window);

            g_BackBuffer = nullptr;
            g_BackSurfacePitch = 0; // 因为交换过，所以这些都需要置空
            //  KSR::Wait_Clock(std::chrono::milliseconds(33));
        }

        ShutdownGraphicSystem();
    }
    catch (KSR::Error e)
    {
        e.Notify();
    }
    catch (std::exception e)
    {
        std::wstring exception_desc;
        KSR::StringConvertor::ANSItoUTF16LE(e.what(), exception_desc);
        KSR::ErrorMessageBox(KSR::StringConvertor::ANSItoUTF16LE(g_WindowTitle).append(L" : Unhandled Exception, aborting"), exception_desc);
    }

    ShutdownGraphicSystem();
    return 0;
}