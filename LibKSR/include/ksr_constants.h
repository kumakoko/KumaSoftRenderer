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

    constexpr int PLX_SHADE_MODE_PURE_FLAG = 0x0000;  // this poly is a constant color
    constexpr int PLX_SHADE_MODE_CONSTANT_FLAG = 0x0000;  // alias
    constexpr int PLX_SHADE_MODE_FLAT_FLAG = 0x2000;  // this poly uses flat shading
    constexpr int PLX_SHADE_MODE_GOURAUD_FLAG = 0x4000;  // this poly used gouraud shading
    constexpr int PLX_SHADE_MODE_PHONG_FLAG = 0x6000; // this poly uses phong shading
    constexpr int PLX_SHADE_MODE_FASTPHONG_FLAG = 0x6000;  // this poly uses phong shading (alias)

    // defines for enhanced PLG file format -> PLX
    // the surface descriptor is still 16-bit now in the following format
    // d15                      d0
    //   CSSD | RRRR| GGGG | BBBB

    // C is the RGB/indexed color flag
    // SS are two bits that define the shading mode
    // D is the double sided flag
    // and RRRR, GGGG, BBBB are the red, green, blue bits for RGB mode
    // or GGGGBBBB is the 8-bit color index for 8-bit mode

    // bit masks to simplify testing????
    constexpr uint16_t PLX_RGB_MASK = 0x8000;   // mask to extract RGB or indexed color
    constexpr uint16_t PLX_SHADE_MODE_MASK = 0x6000;   // mask to extract shading mode
    constexpr uint16_t PLX_2SIDED_MASK = 0x1000;   // mask for double sided
    constexpr uint16_t PLX_COLOR_MASK = 0x0fff;   // xxxxrrrrggggbbbb, 4-bits per channel RGB
    // xxxxxxxxiiiiiiii, indexed mode 8-bit index

// these are the comparision flags after masking
// color mode of polygon
    constexpr uint16_t PLX_COLOR_MODE_RGB_FLAG = 0x8000;   // this poly uses RGB color
    constexpr uint16_t PLX_COLOR_MODE_INDEXED_FLAG = 0x0000;   // this poly uses an indexed 8-bit color 

    // double sided flag
    constexpr uint16_t PLX_2SIDED_FLAG = 0x1000;   // this poly is double sided
    constexpr uint16_t PLX_1SIDED_FLAG = 0x0000;   // this poly is single sided



    // defines for polygons and faces version 1

    // attributes of polygons and polygon faces
    constexpr uint16_t POLY4DV1_ATTR_2SIDED = 0x0001;
    constexpr uint16_t POLY4DV1_ATTR_TRANSPARENT = 0x0002;
    constexpr uint16_t POLY4DV1_ATTR_8BITCOLOR = 0x0004;
    constexpr uint16_t POLY4DV1_ATTR_RGB16 = 0x0008;
    constexpr uint16_t POLY4DV1_ATTR_RGB24 = 0x0010;

    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_PURE = 0x0020;
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_CONSTANT = 0x0020; // (alias)
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_FLAT = 0x0040;
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_GOURAUD = 0x0080;
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_PHONG = 0x0100;
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_FASTPHONG = 0x0100; // (alias)
    constexpr uint16_t POLY4DV1_ATTR_SHADE_MODE_TEXTURE = 0x0200;


    // states of polygons and faces
    constexpr uint16_t POLY4DV1_STATE_ACTIVE = 0x0001;
    constexpr uint16_t POLY4DV1_STATE_CLIPPED = 0x0002;
    constexpr uint16_t POLY4DV1_STATE_BACKFACE = 0x0004;


    // states for objects
    constexpr uint16_t OBJECT4DV1_STATE_ACTIVE = 0x0001;
    constexpr uint16_t OBJECT4DV1_STATE_VISIBLE = 0x0002;
    constexpr uint16_t OBJECT4DV1_STATE_CULLED = 0x0004;


    // general culling flags
    constexpr uint16_t CULL_OBJECT_X_PLANE = 0x0001;// cull on the x clipping planes
    constexpr uint16_t CULL_OBJECT_Y_PLANE = 0x0002;// cull on the y clipping planes
    constexpr uint16_t CULL_OBJECT_Z_PLANE = 0x0004; // cull on the z clipping planes
    constexpr uint16_t CULL_OBJECT_XYZ_PLANES = (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE);

    constexpr int32_t DD_PIXEL_FORMAT8 = 8;
    constexpr int32_t DD_PIXEL_FORMAT555 = 15;
    constexpr int32_t DD_PIXEL_FORMAT565 = 16;
    constexpr int32_t DD_PIXEL_FORMAT888 = 24;
    constexpr int32_t DD_PIXEL_FORMATALPHA888 = 32;

    /**************************************************************************************

    @name: KSR::DEG_TO_RAD
    @return: float
    @param: float ang
    *************************************************************************************/
    inline float DEG_TO_RAD(float ang)
    {
        return ang * PI / 180.0f;
    }

    /**************************************************************************************

    @name: KSR::RAD_TO_DEG
    @return: float
    @param: float rads
    *************************************************************************************/
    inline float RAD_TO_DEG(float rads)
    {
        return rads * 180.0f / PI;
    }
}