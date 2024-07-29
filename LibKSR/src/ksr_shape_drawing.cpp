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

#include "ksr_shape_drawing.h"
#include "ksr_surface.h"

namespace KSR
{
    // these defined the general clipping rectangle
    int min_clip_x = 0;                             // clipping rectangle 
    int max_clip_x = (WINDOW_RENDER_AREA_WIDTH - 1);
    int min_clip_y = 0;
    int max_clip_y = (WINDOW_RENDER_AREA_HEIGHT - 1);

    int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int pitch)
    {
        // cxs,cys,cxe,cye为裁剪过后的线段起始和结束坐标值
        int cxs = x0, cys = y0;
        int cxe = x1, cye = y1;

        // 对线条进行裁剪之后，再绘制
        if (Clip_Line(cxs, cys, cxe, cye))
            Draw_Line16(cxs, cys, cxe, cye, color, dest_buffer, pitch);

        return 1;
    }

    int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int lpitch)
    {
        // this function draws a wireframe triangle

        int cxs, cys,
            cxe, cye;

        // clip and draw each line
        cxs = x0;
        cys = y0;
        cxe = x1;
        cye = y1;

        // clip the line
        if (Clip_Line(cxs, cys, cxe, cye))
            Draw_Line(cxs, cys, cxe, cye, color, dest_buffer, lpitch);

        // return success
        return(1);

    } // end Draw_Clip_Line

    ///////////////////////////////////////////////////////////

    int Clip_Line(int& x1, int& y1, int& x2, int& y2)
    {
        // 这些常量表示线段端点的位置编码，用来确定端点是否在裁剪区域外以及在哪个方向上。
        const uint16_t CLIP_CODE_C = 0x0000; // 中间
        const uint16_t CLIP_CODE_N = 0x0008; // 上（北）
        const uint16_t CLIP_CODE_S = 0x0004; // 下（南）
        const uint16_t CLIP_CODE_E = 0x0002; // 右（东）
        const uint16_t CLIP_CODE_W = 0x0001; // 左（西）
        const uint16_t CLIP_CODE_NE = 0x000a; // 右上（东北）
        const uint16_t CLIP_CODE_SE = 0x0006; // 右下（东南）
        const uint16_t CLIP_CODE_NW = 0x0009; // 左上（西北）
        const uint16_t CLIP_CODE_SW = 0x0005; // 左下（西南）

        int xc1 = x1, yc1 = y1;
        int xc2 = x2, yc2 = y2;
        uint16_t p1_code = 0;
        uint16_t p2_code = 0;

        // 这些代码段计算出两个端点的裁剪码 p1_code 和 p2_code。
        if (y1 < min_clip_y)
            p1_code |= CLIP_CODE_N;
        else
            if (y1 > max_clip_y)
                p1_code |= CLIP_CODE_S;

        if (x1 < min_clip_x)
            p1_code |= CLIP_CODE_W;
        else
            if (x1 > max_clip_x)
                p1_code |= CLIP_CODE_E;

        if (y2 < min_clip_y)
            p2_code |= CLIP_CODE_N;
        else
            if (y2 > max_clip_y)
                p2_code |= CLIP_CODE_S;

        if (x2 < min_clip_x)
            p2_code |= CLIP_CODE_W;
        else
            if (x2 > max_clip_x)
                p2_code |= CLIP_CODE_E;

        // 检查线段是否完全在裁剪区域外 如果两个端点的裁剪码的按位与结果不为0，则线段完全在裁剪区域外，函数返回 0。
        if (p1_code & p2_code)
            return 0;

        // 检查线段是否完全在裁剪区域内 如果两个端点的裁剪码都为0，则线段完全在裁剪区域内，函数返回 1。
        if (p1_code == 0 && p2_code == 0)
            return 1;

        // 计算线段与裁剪区域的交点
        // 对于每个端点，根据其裁剪码计算线段与裁剪区域边界的交点，并更新端点坐标。
        if (CLIP_CODE_C == p1_code)
        {
            // 如果 p1_code 是 CLIP_CODE_C，表示点 p1 在裁剪区域内，不需要裁剪。
        }
        else if (CLIP_CODE_N == p1_code)
        {
            /*如果 p1_code 是 CLIP_CODE_N，表示点 p1 在裁剪区域的上方。此时，需要计算点 p1 与裁剪区域上边界的交点。
            计算新的 yc1 为 min_clip_y。使用直线方程计算新的 xc1。*/
            yc1 = min_clip_y;
            xc1 = x1 + 0.5f + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);
        }
        else if (CLIP_CODE_S == p1_code)
        {
            /*如果 p1_code 是 CLIP_CODE_S，表示点 p1 在裁剪区域的下方。此时，需要计算点 p1 与裁剪区域下边界的交点。
            计算新的 yc1 为 max_clip_y。使用直线方程计算新的 xc1。*/
            yc1 = max_clip_y;
            xc1 = x1 + 0.5f + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);
        }
        else if (CLIP_CODE_W == p1_code) // 西边界
        {
            /*如果 p1_code 是 CLIP_CODE_W，表示点 p1 在裁剪区域的左边。此时，需要计算点 p1 与裁剪区域左边界的交点。
            计算新的 xc1 为 min_clip_x。使用直线方程计算新的 yc1。*/
            xc1 = min_clip_x;
            yc1 = y1 + 0.5f + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
        }
        else if (CLIP_CODE_E == p1_code)// 东边界
        {
            /*如果 p1_code 是 CLIP_CODE_E，表示点 p1 在裁剪区域的右边。此时，需要计算点 p1 与裁剪区域右边界的交点。
            计算新的 xc1 为 max_clip_x。使用直线方程计算新的 yc1。*/
            xc1 = max_clip_x;
            yc1 = y1 + 0.5f + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
        }
        else if (CLIP_CODE_NE == p1_code) // 东北边界
        {
            /*如果 p1_code 是 CLIP_CODE_NE，表示点 p1 在裁剪区域的右上角。此时，需要计算点 p1 与裁剪区域上边界和右边界的交点。
            先计算点 p1 与上边界的交点。如果交点在裁剪区域的左右边界外，则再计算点 p1 与右边界的交点。*/
            // 先计算北向的水平边界是否相交
            yc1 = min_clip_y;
            xc1 = x1 + 0.5f + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // 再计算东向的垂直边界是否相交
                xc1 = max_clip_x;
                yc1 = y1 + 0.5f + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
            }
        }
        else if (CLIP_CODE_SE == p1_code) // 东南边界
        {
            /*如果 p1_code 是 CLIP_CODE_SE，表示点 p1 在裁剪区域的右下角。此时，需要计算点 p1 与裁剪区域下边界和右边界的交点。
              先计算点 p1 与下边界的交点。如果交点在裁剪区域的左右边界外，则再计算点 p1 与右边界的交点。*/
              // 先计算南向的水平边界是否相交
            yc1 = max_clip_y;
            xc1 = x1 + 0.5f + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // 再计算东向的垂直边界是否相交
                xc1 = max_clip_x;
                yc1 = y1 + 0.5f + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
            }
        }
        else if (CLIP_CODE_NW == p1_code)
        {
            /*如果 p1_code 是 CLIP_CODE_NW，表示点 p1 在裁剪区域的左上角。此时，需要计算点 p1 与裁剪区域上边界和左边界的交点。
            先计算点 p1 与上边界的交点。如果交点在裁剪区域的左右边界外，则再计算点 p1 与左边界的交点。*/
            // 先计算北向的水平边界是否相交
            yc1 = min_clip_y;
            xc1 = x1 + 0.5f + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // 再计算西向的垂直边界是否相交
                xc1 = min_clip_x;
                yc1 = y1 + 0.5f + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
            } // end if
        }
        else if (CLIP_CODE_SW == p1_code)
        {
            /*如果 p1_code 是 CLIP_CODE_SW，表示点 p1 在裁剪区域的左下角。此时，需要计算点 p1 与裁剪区域下边界和左边界的交点。
            先计算点 p1 与下边界的交点。如果交点在裁剪区域的左右边界外，则再计算点 p1 与左边界的交点。*/
            // 先计算南向的水平边界是否相交
            yc1 = max_clip_y;
            xc1 = x1 + 0.5f + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // 再计算西向的垂直边界是否相交
                xc1 = min_clip_x;
                yc1 = y1 + 0.5f + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
            }
        }


        if (CLIP_CODE_C == p2_code)
        {
        }
        else if (CLIP_CODE_N == p2_code)
        {
            yc2 = min_clip_y;
            xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
        }
        else if (CLIP_CODE_S == p2_code)
        {
            yc2 = max_clip_y;
            xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
        }
        else if (CLIP_CODE_W == p2_code)
        {
            xc2 = min_clip_x;
            yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
        }
        else if (CLIP_CODE_E == p2_code)
        {
            xc2 = max_clip_x;
            yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
        }
        else if (CLIP_CODE_NE == p2_code)
        {
            yc2 = min_clip_y;
            xc2 = x2 + 0.5f + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = max_clip_x;
                yc2 = y2 + 0.5f + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
            }
        }
        else if (CLIP_CODE_SE == p2_code)
        {
            yc2 = max_clip_y;
            xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = max_clip_x;
                yc2 = y2 + 0.5f + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
            }
        }
        else if (CLIP_CODE_NW == p2_code)
        {
            yc2 = min_clip_y;
            xc2 = x2 + 0.5f + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = min_clip_x;
                yc2 = y2 + 0.5f + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
            }
        }
        else if (CLIP_CODE_SW == p2_code)
        {
            yc2 = max_clip_y;
            xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = min_clip_x;
                yc2 = y2 + 0.5f + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
            }
        }


        // 最后检查交点是否在裁剪区域内
        if ((xc1 < min_clip_x) || (xc1 > max_clip_x) || (yc1 < min_clip_y) || (yc1 > max_clip_y) ||
            (xc2 < min_clip_x) || (xc2 > max_clip_x) || (yc2 < min_clip_y) || (yc2 > max_clip_y))
        {
            return 0;
        }

        x1 = xc1;
        y1 = yc1;
        x2 = xc2;
        y2 = yc2;

        return 1;
    }

    int Draw_Line(int x0, int y0, // starting position 
        int x1, int y1, // ending position
        int color,     // color index
        uint8_t* vb_start, int lpitch) // video buffer and memory pitch
    {
        // this function draws a line from xo,yo to x1,y1 using differential error
        // terms (based on Bresenahams work)

        int dx,             // difference in x's
            dy,             // difference in y's
            dx2,            // dx,dy * 2
            dy2,
            x_inc,          // amount in pixel space to move during drawing
            y_inc,          // amount in pixel space to move during drawing
            error,          // the discriminant i.e. error i.e. decision variable
            index;          // used for looping

        // pre-compute first pixel address in video buffer
        vb_start = vb_start + x0 + y0 * lpitch;

        // compute horizontal and vertical deltas
        dx = x1 - x0;
        dy = y1 - y0;

        // test which direction the line is going in i.e. slope angle
        if (dx >= 0)
        {
            x_inc = 1;

        } // end if line is moving right
        else
        {
            x_inc = -1;
            dx = -dx;  // need absolute value

        } // end else moving left

     // test y component of slope

        if (dy >= 0)
        {
            y_inc = lpitch;
        } // end if line is moving down
        else
        {
            y_inc = -lpitch;
            dy = -dy;  // need absolute value

        } // end else moving up

     // compute (dx,dy) * 2
        dx2 = dx << 1;
        dy2 = dy << 1;

        // now based on which delta is greater we can draw the line
        if (dx > dy)
        {
            // initialize error term
            error = dy2 - dx;

            // draw the line
            for (index = 0; index <= dx; index++)
            {
                // set the pixel
                *vb_start = color;

                // test if error has overflowed
                if (error >= 0)
                {
                    error -= dx2;

                    // move to next line
                    vb_start += y_inc;

                } // end if error overflowed

                // adjust the error term
                error += dy2;

                // move to the next pixel
                vb_start += x_inc;

            } // end for

        } // end if |slope| <= 1
        else
        {
            // initialize error term
            error = dx2 - dy;

            // draw the line
            for (index = 0; index <= dy; index++)
            {
                // set the pixel
                *vb_start = color;

                // test if error overflowed
                if (error >= 0)
                {
                    error -= dy2;

                    // move to next line
                    vb_start += x_inc;

                } // end if error overflowed

             // adjust the error term
                error += dx2;

                // move to the next pixel
                vb_start += y_inc;

            } // end for

        } // end else |slope| > 1

     // return success
        return(1);

    } // end Draw_Line

    ///////////////////////////////////////////////////////////

    int Draw_Line16(int xo, int yo, int x1, int y1, int color, uint8_t* vb_start, int pitch)
    {
        int dx, dy;         // 计算水平和垂直方向的差值。
        int dx2, dy2;       // 差值的两倍，用于误差项的计算。         
        int x_inc, y_inc;   // 用于确定线条绘制的方向
        int error;          // Bresenham算法中的误差项
        uint16_t color_16bits = static_cast<uint16_t>(color);
        int pitch_2 = pitch >> 1;   /* 将字节步幅转换为16位单位（2字节）。比如原来每1个1个字节来读取，3
                                       2个字节一行，pitch为32现在每2个2个字节来读取，32个字节一行，
                                       读取16次就要换行了，所以pitch为16，pitch右移1位等于除以2*/

        // 计算视频缓冲区中第一个像素地址
        ptrdiff_t px_offset = static_cast<ptrdiff_t>(xo + yo * pitch_2);
        uint16_t* vb_start2 = reinterpret_cast<uint16_t*>(vb_start) + px_offset;

        // 计算水平和垂直的差值
        dx = x1 - xo;
        dy = y1 - yo;

        // 确定线的方向-水平方向增量
        if (dx >= 0)
        {
            x_inc = 1; // 线向右边绘制
        }
        else
        {
            x_inc = -1; // 线向左边绘制
            dx = -dx;
        }

        if (dy >= 0) // 垂直方向增量
        {
            y_inc = pitch_2; // 线向下边绘制
        }
        else
        {
            y_inc = -pitch_2; // 线向上边绘制
            dy = -dy;
        }

        // 计算 dx 和 dy 的两倍
        dx2 = dx << 1;
        dy2 = dy << 1;

        if (dx > dy)     // 根据较大的 delta 值决定绘制算法
        {
            error = dy2 - dx; // 初始化误差项

            for (int index = 0; index <= dx; index++) // 绘制线段
            {
                *vb_start2 = color_16bits; // 设置像素颜色

                if (error >= 0) // 检查误差项是否溢出
                {
                    error -= dx2;
                    vb_start2 += y_inc; // 移动到下一行
                }

                error += dy2;       // 调整误差项
                vb_start2 += x_inc; // 移动到下一个像素
            }
        }
        else
        {
            error = dx2 - dy; // 初始化误差项

            for (int index = 0; index <= dy; index++) // 绘制线段
            {
                *vb_start2 = color_16bits; // 设置像素颜色

                if (error >= 0)  // 检查误差项是否溢出
                {
                    error -= dy2;
                    vb_start2 += x_inc;  // 移动到下一行
                }

                error += dx2;       // 调整误差项
                vb_start2 += y_inc; // 移动到下一个像素
            }
        }

        return 1;
    }

    ///////////////////////////////////////////////////////////

    int Draw_Pixel(int x, int y, int color,
        uint8_t* video_buffer, int lpitch)
    {
        // this function plots a single pixel at x,y with color

        video_buffer[x + y * lpitch] = color;

        // return success
        return(1);

    } // end Draw_Pixel

    ///////////////////////////////////////////////////////////   

    int Draw_Pixel16(int x, int y, int color,
        uint8_t* video_buffer, int lpitch)
    {
        // this function plots a single pixel at x,y with color

        ((uint16_t*)video_buffer)[x + y * (lpitch >> 1)] = color;

        // return success
        return(1);

    } // end Draw_Pixel16

    ///////////////////////////////////////////////////////////   


    void Draw_Rectangle(int x1, int y1, int x2, int y2, uint32_t color, SDL_Surface* lpdds)
    {
        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = x2 - x1;
        rect.h = y2 - y1;
        SDL_FillRect(lpdds, &rect, color);
    }

}