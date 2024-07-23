#pragma once

#include <cstdint>

namespace KSR
{
    // the simple bitmap image
    typedef struct BITMAP_IMAGE_TYP
    {
        int state;          // state of bitmap
        int attr;           // attributes of bitmap
        int x, y;            // position of bitmap
        int width, height;  // size of bitmap
        int num_bytes;      // total bytes of bitmap
        int bpp;            // bits per pixel
        std::uint8_t* buffer;      // pixels of bitmap

    } BITMAP_IMAGE, * BITMAP_IMAGE_PTR;
}