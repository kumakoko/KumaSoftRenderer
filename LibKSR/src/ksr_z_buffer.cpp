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

#include <cstdlib>
#include <cstring>
#include "ksr_z_buffer.h"
#include "ksr_asm_optimize.h"

namespace KSR
{
    // defines for zbuffer
    const int ZBUFFER_ATTR_16BIT = 16;
    const int ZBUFFER_ATTR_32BIT = 32;

    int Create_Zbuffer(ZBUFFERV1_PTR zb, // pointer to a zbuffer object
        int width,      // width 
        int height,     // height
        int attr)       // attributes of zbuffer
    {
        // this function creates a zbuffer with the sent width, height, 
        // and bytes per word, in fact the the zbuffer is totally linear
        // but this function is nice so we can create it with some
        // intelligence

        // is this a valid object
        if (!zb)
            return 0;

        // is there any memory already allocated
        if (zb->zbuffer)
            free(zb->zbuffer);

        // set fields
        zb->width = width;
        zb->height = height;
        zb->attr = attr;

        // what size zbuffer 16/32 bit?
        if (attr & ZBUFFER_ATTR_16BIT)
        {
            // compute size in quads
            zb->sizeq = width * height / 2;

            // allocate memory
            if ((zb->zbuffer = (std::uint8_t*)malloc(width * height * sizeof(std::int16_t))))
                return 1;
            else
                return 0;

        } // end if
        else
            if (attr & ZBUFFER_ATTR_32BIT)
            {
                // compute size in quads
                zb->sizeq = width * height;

                // allocate memory
                if ((zb->zbuffer = (std::uint8_t*)malloc(width * height * sizeof(std::int32_t))))
                    return 1;
                else
                    return 0;
            } // end if
            else
                return 0;

    } // end Create_Zbuffer

    ////////////////////////////////////////////////////////////////

    int Delete_Zbuffer(ZBUFFERV1_PTR zb)
    {
        // this function deletes and frees the memory of the zbuffer

        // test for valid object
        if (zb)
        {
            // delete memory and zero object
            if (zb->zbuffer)
                free(zb->zbuffer);

            // clear memory
            memset((void*)zb, 0, sizeof(ZBUFFERV1));

            return 1;

        } // end if
        else
            return 0;

    } // end Delete_Zbuffer

    /////////////////////////////////////////////////////////////////

    void Clear_Zbuffer(ZBUFFERV1_PTR zb, std::uint32_t data)
    {
        // this function clears/sets the zbuffer to a value, the filling
        // is ALWAYS performed in QUADS, thus if your zbuffer is a 16-bit
        // zbuffer then you must build a quad that has two values each
        // the 16-bit value you want to fill with, otherwise just send 
        // the fill value casted to a std::uint32_t
        // 这个函数的作用是将指定的32位数据 data 重复填充到 zb->zbuffer 指向的内存区域，填充的次数为  zb->sizeq 次。
        Mem_Set_QUAD(zb->zbuffer, data, zb->sizeq);
    }
}