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
    // these defined the general clipping rectangle for software clipping
    extern int min_clip_x;                      // clipping rectangle 
    extern int max_clip_x;
    extern int min_clip_y;
    extern int max_clip_y;

    int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int lpitch);
    int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int lpitch);
    int Clip_Line(int& x1, int& y1, int& x2, int& y2);
    int Draw_Line(int xo, int yo, int x1, int y1, int color, uint8_t* vb_start, int lpitch);
    int Draw_Line16(int xo, int yo, int x1, int y1, int color, uint8_t* vb_start, int lpitch);
    int Draw_Pixel(int x, int y, int color, uint8_t* video_buffer, int lpitch);
    void Draw_Rectangle(int x1, int y1, int x2, int y2, uint32_t color, SDL_Surface* lpdds);
}
