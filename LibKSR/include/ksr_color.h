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

     * @name: Get16BitSufaceRGBMask
     * @return: void
     * @param: uint32_t & rmask
     * @param: uint32_t & gmask
     * @param: uint32_t & bmask
     * @param: uint32_t & amask
     *************************************************************************************/
    void Get16BitSufaceRGBMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask);
}