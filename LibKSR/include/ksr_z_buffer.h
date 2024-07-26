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

namespace KSR
{

    // 指明zbuffer是16bits还是32bits的
    extern const int ZBUFFER_ATTR_16BIT;
    extern const int ZBUFFER_ATTR_32BIT;

    // structure for zbuffer
    typedef struct ZBUFFERV1_TYP
    {
        std::uint8_t* zbuffer; // zbuffer首指针
        int attr;       // 指明zbuffer是16bits还是32bits的
        int width;      // zbuffer的宽度，单位是像素点个数
        int height;     // zbuffer的高度，单位是像素点个数
        int sizeq;      // total size in QUADs
    } ZBUFFERV1, *ZBUFFERV1_PTR;


    /******************************************************************************************************************
     * Desc:
     * Method:    Create_Zbuffer
     * Returns:   int
     * Parameter: ZBUFFERV1_PTR zb
     * Parameter: int width width
     * Parameter: int height height
     * Parameter: int attr attributes of zbuffer
     ****************************************************************************************************************/
    int Create_Zbuffer(ZBUFFERV1_PTR zb, int width, int height, int attr);

    int Delete_Zbuffer(ZBUFFERV1_PTR zb);

    void Clear_Zbuffer(ZBUFFERV1_PTR zb, std::uint32_t data);
}