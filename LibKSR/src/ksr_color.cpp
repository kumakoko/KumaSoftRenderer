#include "SDL.h"
#include "ksr_color.h"

namespace KSR
{
    // this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
    static uint16_t _RGB16BIT555(int32_t r, int32_t g, int32_t b)
    {
        int32_t final = (b & 31) + ((g & 31) << 5) + ((r & 31) << 10);
        return static_cast<uint16_t>(final);
    }

    // this builds a 16 bit color value in 5.6.5 format (green dominate mode)
    static uint16_t _RGB16BIT565(int32_t r, int32_t g, int32_t b)
    {
        int32_t final = (b & 31) + ((g & 63) << 5) + ((r & 31) << 11);
        return static_cast<uint16_t>(final);
    }

    uint16_t RGB16Bit565(int32_t r, int32_t g, int32_t b)
    {
        // this function simply builds a 5.6.5 format 16 bit pixel
        // assumes input is RGB 0-255 each channel
        r >>= 3; g >>= 2; b >>= 3;
        return(_RGB16BIT565((r), (g), (b)));

    } // end RGB16Bit565

    //////////////////////////////////////////////////////////

    uint16_t RGB16Bit555(int32_t r, int32_t g, int32_t b)
    {
        // this function simply builds a 5.5.5 format 16 bit pixel
        // assumes input is RGB 0-255 each channel
        r >>= 3; g >>= 3; b >>= 3;
        return(_RGB16BIT555((r), (g), (b)));

    } // end RGB16Bit555


    void Get16BitSufaceRGBMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask)
    {  
        // �������ظ�ʽ����
      

        // �ж�ϵͳ�ֽ�������������
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) 
        {
            rmask = 0xF800;
            gmask = 0x07E0;
            bmask = 0x001F;
            amask = 0x0000;
        }
        else
        {
            rmask = 0x1F << 11;
            gmask = 0x3F << 5;
            bmask = 0x1F;
            amask = 0x0000;
        }
    }
}