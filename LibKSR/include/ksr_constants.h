#pragma once

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