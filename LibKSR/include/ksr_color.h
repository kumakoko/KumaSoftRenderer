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
    // RGB+alpha color
    typedef struct RGBAV1_TYP
    {
        union
        {
            int32_t rgba;                  // 紧凑格式
            uint8_t rgba_M[4];             // 数组格式
            struct
            {
                uint8_t a;
                uint8_t b;
                uint8_t g;
                uint8_t r;
            };
        };
    } RGBAV1, * RGBAV1_PTR;

    constexpr uint32_t MAX_LIGHTS = 8;

    /**************************************************************************************

     * @name: RGB16Bit565
     * @return: uint16_t
     * @param: int32_t r
     * @param: int32_t g
     * @param: int32_t b
     *************************************************************************************/
    uint16_t RGB16Bit565(int32_t r, int32_t g, int32_t b);

    /**************************************************************************************

     * @name: RGB16Bit555
     * @return: uint16_t
     * @param: int32_t r
     * @param: int32_t g
     * @param: int32_t b
     *************************************************************************************/
    uint16_t RGB16Bit555(int32_t r, int32_t g, int32_t b);


    extern uint16_t(*RGB16Bit)(int32_t r, int32_t g, int32_t b);

    /**************************************************************************************
    当创建16位的surface时，获取到创建所需的RGBA四个通道各自的 掩码值
     * @name: Get16BitSufaceRGBMask
     * @return: void
     * @param: uint32_t & rmask 获取到创建所需的R通道的掩码值
     * @param: uint32_t & gmask 获取到创建所需的G通道的掩码值
     * @param: uint32_t & bmask 获取到创建所需的B通道的掩码值
     * @param: uint32_t & amask 获取到创建所需的A通道的掩码值
     *************************************************************************************/
    void Get16BitSufaceRGBMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask);

    /**************************************************************************************

    @name: KSR::SET_BIT
    @return: void
    @param: T1 & word
    @param: T2 & bit_flag
    *************************************************************************************/
    template<typename T1, typename T2>
    inline void SET_BIT(T1& word, T2& bit_flag)
    {
        word = word | bit_flag;
    }

    /**************************************************************************************

    @name: KSR::RESET_BIT
    @return: void
    @param: T1 & word
    @param: T2 & bit_flag
    *************************************************************************************/
    template<typename T1, typename T2>
    inline void RESET_BIT(T1& word, T2& bit_flag)
    {
        word = word & (~bit_flag);
    }

    /**************************************************************************************

    @name: KSR::_RGBA32BIT
    @return: uint32_t
    @param: uint32_t r
    @param: uint32_t g
    @param: uint32_t b
    @param: uint32_t a
    *************************************************************************************/
    inline uint32_t _RGBA32BIT(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
    {
        return a + (b << 8) + (g << 16) + (r << 24);
    }

    /**************************************************************************************
    this builds extract the RGB components of a 16 bit color value in 5.5.5 format (1-bit alpha mode)

    @name: KSR::_RGB555FROM16BIT
    @return: void
    @param: uint32_t RGB
    @param: uint32_t* r
    @param: uint32_t* g
    @param: uint32_t* b
    *************************************************************************************/
    inline void _RGB555FROM16BIT(uint32_t RGB, uint32_t* r, uint32_t* g, uint32_t* b)
    {
        *r = (RGB >> 10) & 0x1f;
        *g = (RGB >> 5) & 0x1f;
        *b = RGB & 0x1f;
    }

    /**************************************************************************************
    this extracts the RGB components of a 16 bit color value in 5.6.5 format (green dominate mode)

    @name: KSR::_RGB565FROM16BIT
    @return: void
    @param: uint32_t RGB
    @param: uint32_t* r
    @param: uint32_t* g
    @param: uint32_t* b
    *************************************************************************************/
    inline void  _RGB565FROM16BIT(uint32_t RGB, uint32_t* r, uint32_t* g, uint32_t* b)
    {
        *r = (RGB >> 11) & 0x1f;
        *g = (RGB >> 5) & 0x3f;
        *b = RGB & 0x1f;
    }

}