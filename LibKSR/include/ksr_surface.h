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

namespace KSR
{
    extern int32_t dd_pixel_format;                         // default pixel format

    extern uint8_t* primary_buffer; // 主页面首指针
    extern uint8_t* back_buffer;    // 后台页面的首指针
    extern int back_lpitch;

    extern SDL_Surface* lpddsprimary;
    extern SDL_Surface* lpddsback;
    extern SDL_Surface* gScreenSurface;
    extern SDL_Window* gWindow;
    extern uint32_t WINDOW_RENDER_AREA_WIDTH;
    extern uint32_t WINDOW_RENDER_AREA_HEIGHT;

    /**************************************************************************************
    初始化图形系统
    @name: KSR::InitializeGraphicSystem
    @return: void
    @param: uint32_t sw
    @param: uint32_t sh
    @param: const char * wn
    *************************************************************************************/
    void InitializeGraphicSystem(uint32_t sw,uint32_t sh, const char* wn);

    /**************************************************************************************
    关闭图形系统
    @name: KSR::ShutdownGraphicSystem
    @return: void
    *************************************************************************************/
    void ShutdownGraphicSystem();

    void DDraw_Fill_Surface(SDL_Surface* lpdds, uint32_t color, SDL_Rect* client = nullptr);

    uint8_t* DDraw_Lock_Back_Surface();

    void DDraw_Unlock_Back_Surface();

    void DDraw_Flip();
}