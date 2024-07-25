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
