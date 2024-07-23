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