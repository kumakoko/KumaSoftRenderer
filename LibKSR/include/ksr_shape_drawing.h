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

    /**************************************************************************************
    
    @name: KSR::Draw_Clip_Line
    @return: int
    @param: int x0
    @param: int y0
    @param: int x1
    @param: int y1
    @param: int color
    @param: uint8_t * dest_buffer
    @param: int lpitch
    *************************************************************************************/
    int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int lpitch);

    /**************************************************************************************
    以(x0,y0)为起点，为终点，基于屏幕空间，以16位的surface位深度，以color指定的颜色值，在内存起始
    地址为dest_buffer，跨度为pitch的地址空间，绘制一条线段
    @name: KSR::Draw_Clip_Line16
    @return: int
    @param: int x0
    @param: int y0
    @param: int x1
    @param: int y1
    @param: int color
    @param: uint8_t * dest_buffer
    @param: int pitch
    *************************************************************************************/
    int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int pitch);

    /**************************************************************************************
    这段代码是一个用来进行线段裁剪的函数Clip_Line，其目的是将给定的线段裁剪到一个全局定义的剪裁区域
    内。函数使用 Cohen-Sutherland 剪裁算法，通过检查线段的两个端点是否在裁剪区域之外，然后调整这些
    点的位置以使得线段在裁剪区域之内。

    @name: KSR::Clip_Line
    @return: int 返回值为 0 表示线段在裁剪区域之外，不可见；返回 1 表示线段在裁剪区域之内或部分可见。
    @param: int & x1 线段的起始端点坐标x分量
    @param: int & y1 线段的起始端点坐标y分量
    @param: int & x2 线段的结束端点坐标x分量 
    @param: int & y2 线段的结束端点坐标y分量 
    *************************************************************************************/
    int Clip_Line(int& x1, int& y1, int& x2, int& y2);

    /**************************************************************************************
    
    @name: KSR::Draw_Line
    @return: int
    @param: int xo
    @param: int yo
    @param: int x1
    @param: int y1
    @param: int color
    @param: uint8_t * vb_start
    @param: int lpitch
    *************************************************************************************/
    int Draw_Line(int xo, int yo, int x1, int y1, int color, uint8_t* vb_start, int lpitch);

    /**************************************************************************************
    
    @name: KSR::Draw_Line16
    @return: int
    @param: int xo
    @param: int yo
    @param: int x1
    @param: int y1
    @param: int color
    @param: uint8_t * vb_start
    @param: int lpitch
    *************************************************************************************/
    int Draw_Line16(int xo, int yo, int x1, int y1, int color, uint8_t* vb_start, int lpitch);

    /**************************************************************************************
    
    @name: KSR::Draw_Pixel
    @return: int
    @param: int x
    @param: int y
    @param: int color
    @param: uint8_t * video_buffer
    @param: int lpitch
    *************************************************************************************/
    int Draw_Pixel(int x, int y, int color, uint8_t* video_buffer, int lpitch);

    /**************************************************************************************
    
    @name: KSR::Draw_Rectangle
    @return: void
    @param: int x1
    @param: int y1
    @param: int x2
    @param: int y2
    @param: uint32_t color
    @param: SDL_Surface * lpdds
    *************************************************************************************/
    void Draw_Rectangle(int x1, int y1, int x2, int y2, uint32_t color, SDL_Surface* lpdds);
}
