#pragma once

#include <cstdint>

namespace KSR
{
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
}