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

#include "ksr_constants.h"

namespace KSR
{
    const float EPSILON_E3 = (float)(1E-3);
    const float EPSILON_E4 = (float)(1E-4);
    const float EPSILON_E5 = (float)(1E-5);
    const float EPSILON_E6 = (float)(1E-6);

    const int PARM_LINE_NO_INTERSECT = 0;
    const int PARM_LINE_INTERSECT_IN_SEGMENT = 1;
    const int PARM_LINE_INTERSECT_OUT_SEGMENT = 2;
    const int PARM_LINE_INTERSECT_EVERYWHERE = 3;

    const float PI = 3.141592654f;
    const float PI2 = 6.283185307f;
    const float PI_DIV_2 = 1.570796327f;
    const float PI_DIV_4 = 0.785398163f;
    const float PI_INV = 0.318309886f;

}