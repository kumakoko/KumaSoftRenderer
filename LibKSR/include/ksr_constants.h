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
    extern const float EPSILON_E3;
    extern const float EPSILON_E4;
    extern const float EPSILON_E5;
    extern const float EPSILON_E6;

    // defines for parametric line intersections
    extern const int PARM_LINE_NO_INTERSECT;
    extern const int PARM_LINE_INTERSECT_IN_SEGMENT;
    extern const int PARM_LINE_INTERSECT_OUT_SEGMENT;
    extern const int PARM_LINE_INTERSECT_EVERYWHERE;

    // 圆周率相关参数
    extern const float PI;
    extern const float PI2;
    extern const float PI_DIV_2;
    extern const float PI_DIV_4;
    extern const float PI_INV;



    inline float DEG_TO_RAD(float ang)
    {
        return ang * PI / 180.0f;
    }

    inline float RAD_TO_DEG(float rads)
    {
        return rads * 180.0f / PI;
    }
}