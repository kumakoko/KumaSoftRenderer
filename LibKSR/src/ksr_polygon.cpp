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

#include "ksr_polygon.h"

namespace KSR
{
    // states of polygons and faces
    const std::uint16_t POLY4DV1_STATE_ACTIVE = 0x0001;
    const std::uint16_t POLY4DV1_STATE_CLIPPED = 0x0002;
    const std::uint16_t POLY4DV1_STATE_BACKFACE = 0x0004;

    void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, std::uint8_t* vbuffer, int mempitch)
    {
        // this function draws a general n sided polygon 

        int ydiff1, ydiff2,         // difference between starting x and ending x
            xdiff1, xdiff2,         // difference between starting y and ending y
            start,                  // starting offset of line between edges
            length,                 // distance from edge 1 to edge 2
            errorterm1, errorterm2, // error terms for edges 1 & 2
            offset1, offset2,       // offset of current pixel in edges 1 & 2
            count1, count2,         // increment count for edges 1 & 2
            xunit1, xunit2;         // unit to advance x offset for edges 1 & 2


#ifdef DEBUG_ON
        // track rendering stats
        debug_polys_rendered_per_frame++;
#endif

        // initialize count of number of edges drawn:
        int edgecount = poly->num_verts - 1;

        // determine which vertex is at top of polygon:

        int firstvert = 0;         // start by assuming vertex 0 is at top

        int min_y = poly->vlist[0].y; // find y coordinate of vertex 0

        for (int index = 1; index < poly->num_verts; index++)
        {
            // Search thru vertices
            if ((poly->vlist[index].y) < min_y)
            {
                // is another vertex higher?
                firstvert = index;
                min_y = poly->vlist[index].y;
            } // end if

        } // end for index

    // finding starting and ending vertices of first two edges:
        int startvert1 = firstvert;      // get starting vertex of edge 1
        int startvert2 = firstvert;      // get starting vertex of edge 2
        int xstart1 = static_cast<float>(poly->vlist[startvert1].x + poly->x0);
        int ystart1 = static_cast<float>(poly->vlist[startvert1].y + poly->y0);
        int xstart2 = static_cast<float>(poly->vlist[startvert2].x + poly->x0);
        int ystart2 = static_cast<float>(poly->vlist[startvert2].y + poly->y0);
        int endvert1 = startvert1 - 1;           // get ending vertex of edge 1

        if (endvert1 < 0)
            endvert1 = poly->num_verts - 1;    // check for wrap

        int xend1 = poly->vlist[endvert1].x + poly->x0;      // get x & y coordinates
        int yend1 = poly->vlist[endvert1].y + poly->y0;      // of ending vertices
        int endvert2 = startvert2 + 1;           // get ending vertex of edge 2

        if (endvert2 == (poly->num_verts))
            endvert2 = 0;  // Check for wrap

        int xend2 = poly->vlist[endvert2].x + poly->x0;      // get x & y coordinates
        int yend2 = poly->vlist[endvert2].y + poly->y0;      // of ending vertices

        // draw the polygon:

        while (edgecount > 0)
        {
            // continue drawing until all edges drawn
            offset1 = mempitch * ystart1 + xstart1;  // offset of edge 1
            offset2 = mempitch * ystart2 + xstart2;  // offset of edge 2

            // initialize error terms
            // for edges 1 & 2
            errorterm1 = 0;
            errorterm2 = 0;

            // get absolute value of
            if ((ydiff1 = yend1 - ystart1) < 0)
                ydiff1 = -ydiff1;

            // x & y lengths of edges
            if ((ydiff2 = yend2 - ystart2) < 0)
                ydiff2 = -ydiff2;

            if ((xdiff1 = xend1 - xstart1) < 0)
            {
                // get value of length
                xunit1 = -1;                    // calculate X increment
                xdiff1 = -xdiff1;
            } // end if
            else
            {
                xunit1 = 1;
            } // end else

            if ((xdiff2 = xend2 - xstart2) < 0)
            {
                // Get value of length
                xunit2 = -1;                   // calculate X increment
                xdiff2 = -xdiff2;
            } // end else
            else
            {
                xunit2 = 1;
            } // end else

         // choose which of four routines to use
            if (xdiff1 > ydiff1)
            {
                // if x length of edge 1 is greater than y length
                if (xdiff2 > ydiff2)
                {
                    // if X length of edge 2 is greater than y length

                    // increment edge 1 on X and edge 2 on X:
                    count1 = xdiff1;    // count for x increment on edge 1
                    count2 = xdiff2;    // count for x increment on edge 2

                    while (count1 && count2)
                    {
                        // continue drawing until one edge is done
                        // calculate edge 1:
                        while ((errorterm1 < xdiff1) && (count1 > 0))
                        {
                            // finished w/edge 1?
                            if (count1--)
                            {
                                // count down on edge 1
                                offset1 += xunit1;  // increment pixel offset
                                xstart1 += xunit1;
                            } // end if

                            errorterm1 += ydiff1; // increment error term

                            if (errorterm1 < xdiff1)
                            {  // if not more than XDIFF
                                vbuffer[offset1] = (std::uint8_t)poly->color; // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm1 -= xdiff1; // if time to increment X, restore error term

                        // calculate edge 2:

                        while ((errorterm2 < xdiff2) && (count2 > 0))
                        {
                            // finished w/edge 2?
                            if (count2--)
                            {
                                // count down on edge 2
                                offset2 += xunit2;  // increment pixel offset
                                xstart2 += xunit2;
                            } // end if

                            errorterm2 += ydiff2; // increment error term

                            if (errorterm2 < xdiff2)
                            {  // if not more than XDIFF
                                vbuffer[offset2] = (std::uint8_t)poly->color;  // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm2 -= xdiff2; // if time to increment X, restore error term

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        { // if negative...
                            length = -length;       // make it positive
                            start = offset2;        // and set START to edge 2
                        } // end if
                        else
                            start = offset1;     // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)
                        {  // From edge to edge...
                            vbuffer[index] = (std::uint8_t)poly->color;         // ...draw the line
                        } // end for index

                        offset1 += mempitch;           // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;           // advance edge 2 offset to next line
                        ystart2++;

                    } // end if

                } // end if
                else
                {
                    // increment edge 1 on X and edge 2 on Y:
                    count1 = xdiff1;    // count for X increment on edge 1
                    count2 = ydiff2;    // count for Y increment on edge 2

                    while (count1 && count2)
                    {  // continue drawing until one edge is done
                       // calculate edge 1:
                        while ((errorterm1 < xdiff1) && (count1 > 0))
                        { // finished w/edge 1?
                            if (count1--)
                            {
                                // count down on edge 1
                                offset1 += xunit1;  // increment pixel offset
                                xstart1 += xunit1;
                            } // end if

                            errorterm1 += ydiff1; // increment error term

                            if (errorterm1 < xdiff1)
                            {  // If not more than XDIFF
                                vbuffer[offset1] = (std::uint8_t)poly->color; // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm1 -= xdiff1; // If time to increment X, restore error term

                        // calculate edge 2:
                        errorterm2 += xdiff2; // increment error term

                        if (errorterm2 >= ydiff2)
                        { // if time to increment Y...
                            errorterm2 -= ydiff2;        // ...restore error term
                            offset2 += xunit2;           // ...and advance offset to next pixel
                            xstart2 += xunit2;
                        } // end if

                        count2--;

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        { // if negative...
                            length = -length;       // ...make it positive
                            start = offset2;        // and set START to edge 2
                        } // end if
                        else
                            start = offset1;        // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)  // from edge to edge
                        {
                            vbuffer[index] = (std::uint8_t)poly->color;         // ...draw the line
                        } // end for index

                        offset1 += mempitch;           // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;           // advance edge 2 offset to next line
                        ystart2++;

                    } // end while
                } // end if
            } // end if
            else
            {
                if (xdiff2 > ydiff2)
                {
                    // increment edge 1 on Y and edge 2 on X:

                    count1 = ydiff1;  // count for Y increment on edge 1
                    count2 = xdiff2;  // count for X increment on edge 2

                    while (count1 && count2)
                    {  // continue drawing until one edge is done
                      // calculate edge 1:

                        errorterm1 += xdiff1; // Increment error term

                        if (errorterm1 >= ydiff1)
                        {  // if time to increment Y...
                            errorterm1 -= ydiff1;         // ...restore error term
                            offset1 += xunit1;            // ...and advance offset to next pixel
                            xstart1 += xunit1;
                        } // end if

                        count1--;

                        // Calculate edge 2:

                        while ((errorterm2 < xdiff2) && (count2 > 0))
                        { // finished w/edge 1?
                            if (count2--)
                            { // count down on edge 2
                                offset2 += xunit2;  // increment pixel offset
                                xstart2 += xunit2;
                            } // end if

                            errorterm2 += ydiff2; // increment error term

                            if (errorterm2 < xdiff2)
                            {  // if not more than XDIFF
                                vbuffer[offset2] = (std::uint8_t)poly->color; // ...plot a pixel
                            } // end if
                        } // end while

                        errorterm2 -= xdiff2;  // if time to increment X, restore error term

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        {    // if negative...
                            length = -length;  // ...make it positive
                            start = offset2;   // and set START to edge 2
                        } // end if
                        else
                            start = offset1;  // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++) // from edge to edge...
                        {
                            vbuffer[index] = (std::uint8_t)poly->color;      // ...draw the line
                        } // end for index

                        offset1 += mempitch;         // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;         // advance edge 2 offset to next line
                        ystart2++;

                    } // end if
                } // end if
                else
                {
                    // increment edge 1 on Y and edge 2 on Y:
                    count1 = ydiff1;  // count for Y increment on edge 1
                    count2 = ydiff2;  // count for Y increment on edge 2

                    while (count1 && count2)
                    {
                        // continue drawing until one edge is done
                        // calculate edge 1:
                        errorterm1 += xdiff1;  // increment error term

                        if (errorterm1 >= ydiff1)
                        {                           // if time to increment Y
                            errorterm1 -= ydiff1;         // ...restore error term
                            offset1 += xunit1;            // ...and advance offset to next pixel
                            xstart1 += xunit1;
                        } // end if

                        count1--;

                        // calculate edge 2:
                        errorterm2 += xdiff2;            // increment error term

                        if (errorterm2 >= ydiff2)
                        {                           // if time to increment Y
                            errorterm2 -= ydiff2;         // ...restore error term
                            offset2 += xunit2;            // ...and advance offset to next pixel
                            xstart2 += xunit2;
                        } // end if

                        --count2;

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1;  // determine length of horizontal line

                        if (length < 0)
                        {
                            // if negative...
                            length = -length;        // ...make it positive
                            start = offset2;         // and set START to edge 2
                        } // end if
                        else
                            start = offset1;         // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)
                        { // from edge to edge
                            vbuffer[index] = (std::uint8_t)poly->color;   // ...draw the linee
                        } // end for index

                        offset1 += mempitch;            // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;            // advance edge 2 offset to next line
                        ystart2++;

                    } // end while

                } // end else

            } // end if

            // another edge (at least) is complete. Start next edge, if any.
            if (!count1)
            {                      // if edge 1 is complete...
                --edgecount;           // decrement the edge count
                startvert1 = endvert1;   // make ending vertex into start vertex
                --endvert1;            // and get new ending vertex

                if (endvert1 < 0)
                    endvert1 = poly->num_verts - 1; // check for wrap

                xend1 = poly->vlist[endvert1].x + poly->x0;  // get x & y of new end vertex
                yend1 = poly->vlist[endvert1].y + poly->y0;
            } // end if

            if (!count2)
            {                     // if edge 2 is complete...
                --edgecount;          // decrement the edge count
                startvert2 = endvert2;  // make ending vertex into start vertex
                endvert2++;           // and get new ending vertex

                if (endvert2 == (poly->num_verts))
                    endvert2 = 0;                // check for wrap

                xend2 = poly->vlist[endvert2].x + poly->x0;  // get x & y of new end vertex
                yend2 = poly->vlist[endvert2].y + poly->y0;

            } // end if

        } // end while

    } // end Draw_Filled_Polygon2D



    void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, std::uint8_t* _vbuffer, int mempitch)
    {
        // this function draws a general n sided polygon 

        int ydiff1, ydiff2,         // difference between starting x and ending x
            xdiff1, xdiff2,         // difference between starting y and ending y
            start,                  // starting offset of line between edges
            length,                 // distance from edge 1 to edge 2
            errorterm1, errorterm2, // error terms for edges 1 & 2
            offset1, offset2,       // offset of current pixel in edges 1 & 2
            count1, count2,         // increment count for edges 1 & 2
            xunit1, xunit2;         // unit to advance x offset for edges 1 & 2


#ifdef DEBUG_ON
        // track rendering stats
        debug_polys_rendered_per_frame++;
#endif


        // recast vbuffer into short version since this is a 16 bit mode
        std::uint16_t* vbuffer = (std::uint16_t*)_vbuffer;

        // convert mempitch into WORD or 16bit stride
        mempitch = (mempitch >> 1);

        // initialize count of number of edges drawn:
        int edgecount = poly->num_verts - 1;

        // determine which vertex is at top of polygon:

        int firstvert = 0;         // start by assuming vertex 0 is at top

        int min_y = poly->vlist[0].y; // find y coordinate of vertex 0

        for (int index = 1; index < poly->num_verts; index++)
        {
            // Search thru vertices
            if ((poly->vlist[index].y) < min_y)
            {
                // is another vertex higher?
                firstvert = index;
                min_y = poly->vlist[index].y;
            } // end if

        } // end for index

    // finding starting and ending vertices of first two edges:
        int startvert1 = firstvert;      // get starting vertex of edge 1
        int startvert2 = firstvert;      // get starting vertex of edge 2
        int xstart1 = poly->vlist[startvert1].x + poly->x0;
        int ystart1 = poly->vlist[startvert1].y + poly->y0;
        int xstart2 = poly->vlist[startvert2].x + poly->x0;
        int ystart2 = poly->vlist[startvert2].y + poly->y0;
        int endvert1 = startvert1 - 1;           // get ending vertex of edge 1

        if (endvert1 < 0)
            endvert1 = poly->num_verts - 1;    // check for wrap

        int xend1 = poly->vlist[endvert1].x + poly->x0;      // get x & y coordinates
        int yend1 = poly->vlist[endvert1].y + poly->y0;      // of ending vertices
        int endvert2 = startvert2 + 1;           // get ending vertex of edge 2

        if (endvert2 == (poly->num_verts))
            endvert2 = 0;  // Check for wrap

        int xend2 = poly->vlist[endvert2].x + poly->x0;      // get x & y coordinates
        int yend2 = poly->vlist[endvert2].y + poly->y0;      // of ending vertices

        // draw the polygon:

        while (edgecount > 0)
        {
            // continue drawing until all edges drawn
            offset1 = mempitch * ystart1 + xstart1;  // offset of edge 1
            offset2 = mempitch * ystart2 + xstart2;  // offset of edge 2

            // initialize error terms
            // for edges 1 & 2
            errorterm1 = 0;
            errorterm2 = 0;

            // get absolute value of
            if ((ydiff1 = yend1 - ystart1) < 0)
                ydiff1 = -ydiff1;

            // x & y lengths of edges
            if ((ydiff2 = yend2 - ystart2) < 0)
                ydiff2 = -ydiff2;

            if ((xdiff1 = xend1 - xstart1) < 0)
            {
                // get value of length
                xunit1 = -1;                    // calculate X increment
                xdiff1 = -xdiff1;
            } // end if
            else
            {
                xunit1 = 1;
            } // end else

            if ((xdiff2 = xend2 - xstart2) < 0)
            {
                // Get value of length
                xunit2 = -1;                   // calculate X increment
                xdiff2 = -xdiff2;
            } // end else
            else
            {
                xunit2 = 1;
            } // end else

         // choose which of four routines to use
            if (xdiff1 > ydiff1)
            {
                // if x length of edge 1 is greater than y length
                if (xdiff2 > ydiff2)
                {
                    // if X length of edge 2 is greater than y length

                    // increment edge 1 on X and edge 2 on X:
                    count1 = xdiff1;    // count for x increment on edge 1
                    count2 = xdiff2;    // count for x increment on edge 2

                    while (count1 && count2)
                    {
                        // continue drawing until one edge is done
                        // calculate edge 1:
                        while ((errorterm1 < xdiff1) && (count1 > 0))
                        {
                            // finished w/edge 1?
                            if (count1--)
                            {
                                // count down on edge 1
                                offset1 += xunit1;  // increment pixel offset
                                xstart1 += xunit1;
                            } // end if

                            errorterm1 += ydiff1; // increment error term

                            if (errorterm1 < xdiff1)
                            {  // if not more than XDIFF
                                vbuffer[offset1] = (std::uint16_t)poly->color; // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm1 -= xdiff1; // if time to increment X, restore error term

                        // calculate edge 2:

                        while ((errorterm2 < xdiff2) && (count2 > 0))
                        {
                            // finished w/edge 2?
                            if (count2--)
                            {
                                // count down on edge 2
                                offset2 += xunit2;  // increment pixel offset
                                xstart2 += xunit2;
                            } // end if

                            errorterm2 += ydiff2; // increment error term

                            if (errorterm2 < xdiff2)
                            {  // if not more than XDIFF
                                vbuffer[offset2] = (std::uint16_t)poly->color;  // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm2 -= xdiff2; // if time to increment X, restore error term

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        { // if negative...
                            length = -length;       // make it positive
                            start = offset2;        // and set START to edge 2
                        } // end if
                        else
                            start = offset1;     // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)
                        {  // From edge to edge...
                            vbuffer[index] = (std::uint16_t)poly->color;         // ...draw the line
                        } // end for index

                        offset1 += mempitch;           // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;           // advance edge 2 offset to next line
                        ystart2++;

                    } // end if

                } // end if
                else
                {
                    // increment edge 1 on X and edge 2 on Y:
                    count1 = xdiff1;    // count for X increment on edge 1
                    count2 = ydiff2;    // count for Y increment on edge 2

                    while (count1 && count2)
                    {  // continue drawing until one edge is done
                       // calculate edge 1:
                        while ((errorterm1 < xdiff1) && (count1 > 0))
                        { // finished w/edge 1?
                            if (count1--)
                            {
                                // count down on edge 1
                                offset1 += xunit1;  // increment pixel offset
                                xstart1 += xunit1;
                            } // end if

                            errorterm1 += ydiff1; // increment error term

                            if (errorterm1 < xdiff1)
                            {  // If not more than XDIFF
                                vbuffer[offset1] = (std::uint16_t)poly->color; // ...plot a pixel
                            } // end if

                        } // end while

                        errorterm1 -= xdiff1; // If time to increment X, restore error term

                        // calculate edge 2:
                        errorterm2 += xdiff2; // increment error term

                        if (errorterm2 >= ydiff2)
                        { // if time to increment Y...
                            errorterm2 -= ydiff2;        // ...restore error term
                            offset2 += xunit2;           // ...and advance offset to next pixel
                            xstart2 += xunit2;
                        } // end if

                        count2--;

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        { // if negative...
                            length = -length;       // ...make it positive
                            start = offset2;        // and set START to edge 2
                        } // end if
                        else
                            start = offset1;        // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)  // from edge to edge
                        {
                            vbuffer[index] = (std::uint16_t)poly->color;         // ...draw the line
                        } // end for index

                        offset1 += mempitch;           // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;           // advance edge 2 offset to next line
                        ystart2++;

                    } // end while
                } // end if
            } // end if
            else
            {
                if (xdiff2 > ydiff2)
                {
                    // increment edge 1 on Y and edge 2 on X:

                    count1 = ydiff1;  // count for Y increment on edge 1
                    count2 = xdiff2;  // count for X increment on edge 2

                    while (count1 && count2)
                    {  // continue drawing until one edge is done
                      // calculate edge 1:

                        errorterm1 += xdiff1; // Increment error term

                        if (errorterm1 >= ydiff1)
                        {  // if time to increment Y...
                            errorterm1 -= ydiff1;         // ...restore error term
                            offset1 += xunit1;            // ...and advance offset to next pixel
                            xstart1 += xunit1;
                        } // end if

                        count1--;

                        // Calculate edge 2:

                        while ((errorterm2 < xdiff2) && (count2 > 0))
                        { // finished w/edge 1?
                            if (count2--)
                            { // count down on edge 2
                                offset2 += xunit2;  // increment pixel offset
                                xstart2 += xunit2;
                            } // end if

                            errorterm2 += ydiff2; // increment error term

                            if (errorterm2 < xdiff2)
                            {  // if not more than XDIFF
                                vbuffer[offset2] = (std::uint16_t)poly->color; // ...plot a pixel
                            } // end if
                        } // end while

                        errorterm2 -= xdiff2;  // if time to increment X, restore error term

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1; // determine length of horizontal line

                        if (length < 0)
                        {    // if negative...
                            length = -length;  // ...make it positive
                            start = offset2;   // and set START to edge 2
                        } // end if
                        else
                            start = offset1;  // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++) // from edge to edge...
                        {
                            vbuffer[index] = (std::uint16_t)poly->color;      // ...draw the line
                        } // end for index

                        offset1 += mempitch;         // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;         // advance edge 2 offset to next line
                        ystart2++;

                    } // end if
                } // end if
                else
                {
                    // increment edge 1 on Y and edge 2 on Y:
                    count1 = ydiff1;  // count for Y increment on edge 1
                    count2 = ydiff2;  // count for Y increment on edge 2

                    while (count1 && count2)
                    {
                        // continue drawing until one edge is done
                        // calculate edge 1:
                        errorterm1 += xdiff1;  // increment error term

                        if (errorterm1 >= ydiff1)
                        {                           // if time to increment Y
                            errorterm1 -= ydiff1;         // ...restore error term
                            offset1 += xunit1;            // ...and advance offset to next pixel
                            xstart1 += xunit1;
                        } // end if

                        count1--;

                        // calculate edge 2:
                        errorterm2 += xdiff2;            // increment error term

                        if (errorterm2 >= ydiff2)
                        {                           // if time to increment Y
                            errorterm2 -= ydiff2;         // ...restore error term
                            offset2 += xunit2;            // ...and advance offset to next pixel
                            xstart2 += xunit2;
                        } // end if

                        --count2;

                        // draw line from edge 1 to edge 2:

                        length = offset2 - offset1;  // determine length of horizontal line

                        if (length < 0)
                        {
                            // if negative...
                            length = -length;        // ...make it positive
                            start = offset2;         // and set START to edge 2
                        } // end if
                        else
                            start = offset1;         // else set START to edge 1

                        for (int index = start; index < start + length + 1; index++)
                        { // from edge to edge
                            vbuffer[index] = (std::uint16_t)poly->color;   // ...draw the linee
                        } // end for index

                        offset1 += mempitch;            // advance edge 1 offset to next line
                        ystart1++;
                        offset2 += mempitch;            // advance edge 2 offset to next line
                        ystart2++;

                    } // end while

                } // end else

            } // end if

            // another edge (at least) is complete. Start next edge, if any.
            if (!count1)
            {                      // if edge 1 is complete...
                --edgecount;           // decrement the edge count
                startvert1 = endvert1;   // make ending vertex into start vertex
                --endvert1;            // and get new ending vertex

                if (endvert1 < 0)
                    endvert1 = poly->num_verts - 1; // check for wrap

                xend1 = poly->vlist[endvert1].x + poly->x0;  // get x & y of new end vertex
                yend1 = poly->vlist[endvert1].y + poly->y0;
            } // end if

            if (!count2)
            {                     // if edge 2 is complete...
                --edgecount;          // decrement the edge count
                startvert2 = endvert2;  // make ending vertex into start vertex
                endvert2++;           // and get new ending vertex

                if (endvert2 == (poly->num_verts))
                    endvert2 = 0;                // check for wrap

                xend2 = poly->vlist[endvert2].x + poly->x0;  // get x & y of new end vertex
                yend2 = poly->vlist[endvert2].y + poly->y0;

            } // end if

        } // end while

    } // end Draw_Filled_Polygon2D16
}