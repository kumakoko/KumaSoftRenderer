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
    int max_clip_x = (SCREEN_WIDTH - 1);
    int min_clip_y = 0;
    int max_clip_y = (SCREEN_HEIGHT - 1);

    int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, uint8_t* dest_buffer, int lpitch)
    {
        // this function draws a clipped line

        int cxs, cys,
            cxe, cye;

        // clip and draw each line
        cxs = x0;
        cys = y0;
        cxe = x1;
        cye = y1;

        // clip the line
        if (Clip_Line(cxs, cys, cxe, cye))
            Draw_Line16(cxs, cys, cxe, cye, color, dest_buffer, lpitch);

        // return success
        return(1);

    } // end Draw_Clip_Line16


    ///////////////////////////////////////////////////////////

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
        // this function clips the sent line using the globally defined clipping
        // region

        // internal clipping codes
        const uint16_t CLIP_CODE_C = 0x0000;
        const uint16_t CLIP_CODE_N = 0x0008;
        const uint16_t CLIP_CODE_S = 0x0004;
        const uint16_t CLIP_CODE_E = 0x0002;
        const uint16_t CLIP_CODE_W = 0x0001;

        const uint16_t CLIP_CODE_NE = 0x000a;
        const uint16_t CLIP_CODE_SE = 0x0006;
        const uint16_t CLIP_CODE_NW = 0x0009;
        const uint16_t CLIP_CODE_SW = 0x0005;

        int xc1 = x1, yc1 = y1, xc2 = x2, yc2 = y2;
        uint16_t p1_code = 0, p2_code = 0;

        // determine codes for p1 and p2
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

        // try and trivially reject
        if ((p1_code & p2_code))
            return(0);

        // test for totally visible, if so leave points untouched
        if (p1_code == 0 && p2_code == 0)
            return(1);

        // determine end clip point for p1
        switch (p1_code)
        {
        case CLIP_CODE_C:
            break;
        case CLIP_CODE_N:
        {
            yc1 = min_clip_y;
            xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);
        }
        break;
        case CLIP_CODE_S:
        {
            yc1 = max_clip_y;
            xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);
        }
        break;
        case CLIP_CODE_W:
        {
            xc1 = min_clip_x;
            yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
        }
        break;
        case CLIP_CODE_E:
        {
            xc1 = max_clip_x;
            yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
        }
        break;

        // these cases are more complex, must compute 2 intersections
        case CLIP_CODE_NE:
        {
            // north hline intersection
            yc1 = min_clip_y;
            xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // east vline intersection
                xc1 = max_clip_x;
                yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
            } // end if

        }
        break;
        case CLIP_CODE_SE:
        {
            // south hline intersection
            yc1 = max_clip_y;
            xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                // east vline intersection
                xc1 = max_clip_x;
                yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
            } // end if

        }
        break;
        case CLIP_CODE_NW:
        {
            // north hline intersection
            yc1 = min_clip_y;
            xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                xc1 = min_clip_x;
                yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
            } // end if

        }
        break;
        case CLIP_CODE_SW:
        {
            // south hline intersection
            yc1 = max_clip_y;
            xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if (xc1 < min_clip_x || xc1 > max_clip_x)
            {
                xc1 = min_clip_x;
                yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
            } // end if

        }
        break;
        default:
            break;

        } // end switch

  // determine clip point for p2
        switch (p2_code)
        {
        case CLIP_CODE_C: break;

        case CLIP_CODE_N:
        {
            yc2 = min_clip_y;
            xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
        } break;

        case CLIP_CODE_S:
        {
            yc2 = max_clip_y;
            xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
        } break;

        case CLIP_CODE_W:
        {
            xc2 = min_clip_x;
            yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
        } break;

        case CLIP_CODE_E:
        {
            xc2 = max_clip_x;
            yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
        } break;

        // these cases are more complex, must compute 2 intersections
        case CLIP_CODE_NE:
        {
            // north hline intersection
            yc2 = min_clip_y;
            xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                // east vline intersection
                xc2 = max_clip_x;
                yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case CLIP_CODE_SE:
        {
            // south hline intersection
            yc2 = max_clip_y;
            xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                // east vline intersection
                xc2 = max_clip_x;
                yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case CLIP_CODE_NW:
        {
            // north hline intersection
            yc2 = min_clip_y;
            xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = min_clip_x;
                yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case CLIP_CODE_SW:
        {
            // south hline intersection
            yc2 = max_clip_y;
            xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if (xc2 < min_clip_x || xc2 > max_clip_x)
            {
                xc2 = min_clip_x;
                yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
            } // end if

        } break;

        default:break;

        } // end switch

  // do bounds check
        if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
            (yc1 < min_clip_y) || (yc1 > max_clip_y) ||
            (xc2 < min_clip_x) || (xc2 > max_clip_x) ||
            (yc2 < min_clip_y) || (yc2 > max_clip_y))
        {
            return(0);
        } // end if

    // store vars back
        x1 = xc1;
        y1 = yc1;
        x2 = xc2;
        y2 = yc2;

        return(1);

    } // end Clip_Line

    ///////////////////////////////////////////////////////////

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

    int Draw_Line16(int x0, int y0, // starting position 
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

        int lpitch_2 = lpitch >> 1; // uint16_t strided lpitch

        // pre-compute first pixel address in video buffer based on 16bit data
        uint16_t* vb_start2 = (uint16_t*)vb_start + x0 + y0 * lpitch_2;

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
            y_inc = lpitch_2;
        } // end if line is moving down
        else
        {
            y_inc = -lpitch_2;
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
                *vb_start2 = (uint16_t)color;

                // test if error has overflowed
                if (error >= 0)
                {
                    error -= dx2;

                    // move to next line
                    vb_start2 += y_inc;

                } // end if error overflowed

                // adjust the error term
                error += dy2;

                // move to the next pixel
                vb_start2 += x_inc;

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
                *vb_start2 = (uint16_t)color;

                // test if error overflowed
                if (error >= 0)
                {
                    error -= dy2;

                    // move to next line
                    vb_start2 += x_inc;

                } // end if error overflowed

             // adjust the error term
                error += dx2;

                // move to the next pixel
                vb_start2 += y_inc;

            } // end for

        } // end else |slope| > 1

     // return success
        return(1);

    } // end Draw_Line16

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
        // this function uses directdraw to draw a filled rectangle

        //DDBLTFX ddbltfx; // this contains the DDBLTFX structure
        //RECT fill_area;  // this contains the destination rectangle

        //// clear out the structure and set the size field 
        //DDRAW_INIT_STRUCT(ddbltfx);

        //// set the dwfillcolor field to the desired color
        //ddbltfx.dwFillColor = color;

        //// fill in the destination rectangle data (your data)
        //fill_area.top = y1;
        //fill_area.left = x1;
        //fill_area.bottom = y2;
        //fill_area.right = x2;

        //// ready to blt to surface, in this case blt to primary
        //lpdds->Blt(&fill_area, // ptr to dest rectangle
        //    NULL,       // ptr to source surface, NA            
        //    NULL,       // ptr to source rectangle, NA
        //    DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
        //    &ddbltfx);  // ptr to DDBLTFX structure

        SDL_Rect rect;
        rect.x = x1;
        rect.y = y1;
        rect.w = x2 - x1;
        rect.h = y2 - y1;

        SDL_FillRect(lpdds, &rect, color);

        // return success
    } // end Draw_Rectangle

}