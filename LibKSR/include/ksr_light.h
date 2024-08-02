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
#include "ksr_color.h"
#include "ksr_vector.h"
#include "ksr_model_object.h"
#include "ksr_render_list.h"
#include "ksr_camera.h"

namespace KSR
{
    // create some constants for ease of access
    constexpr uint32_t AMBIENT_LIGHT_INDEX = 0; // ambient light index
    constexpr uint32_t INFINITE_LIGHT_INDEX = 1; // infinite light index
    constexpr uint32_t POINT_LIGHT_INDEX = 2; // point light index
    constexpr uint32_t SPOT_LIGHT_INDEX = 3; // spot light index

    // 灯光的开关状态
    enum LightOnOffState
    {
        LIGHTV1_STATE_ON = 1,   // 开
        LIGHTV1_STATE_OFF = 0   // 关
    };

    enum LightType 
    {
        LIGHTV1_ATTR_AMBIENT = 0x0001,    // basic ambient light
        LIGHTV1_ATTR_INFINITE = 0x0002,    // infinite light source
        LIGHTV1_ATTR_DIRECTIONAL = 0x0002,    // infinite light source (alias)
        LIGHTV1_ATTR_POINT = 0x0004,    // point light source
        LIGHTV1_ATTR_SPOTLIGHT1 = 0x0008,    // spotlight type 1 (simple)
        LIGHTV1_ATTR_SPOTLIGHT2 = 0x0010    // spotlight type 2 (complex)
    };


    // first light structure
    typedef struct LIGHTV1_TYP
    {
        int state; // state of light
        int id;    // id of light
        int attr;  // type of light, and extra qualifiers

        RGBAV1 c_ambient;   // ambient light intensity
        RGBAV1 c_diffuse;   // diffuse light intensity
        RGBAV1 c_specular;  // specular light intensity

        POINT4D  pos;       // position of light
        VECTOR4D dir;       // direction of light
        float kc, kl, kq;   // attenuation factors
        float spot_inner;   // inner angle for spot light
        float spot_outer;   // outer angle for spot light
        float pf;           // power factor/falloff for spot lights

        int   iaux1, iaux2; // auxiliary vars for future expansion
        float faux1, faux2;
        void* ptr;

    } LIGHTV1, * LIGHTV1_PTR;

    LIGHTV1 lights[MAX_LIGHTS];  // lights in system

    extern int num_lights;              // current number of lights

    /**************************************************************************************

    @name: KSR::Reset_Lights_LIGHTV1
    @return: void
    *************************************************************************************/
    void Reset_Lights_LIGHTV1();

    /**************************************************************************************

    @name: KSR::Init_Light_LIGHTV1
    @return: int
    @param: int index
    @param: LightOnOffState _state
    @param: int _attr
    @param: RGBAV1 _c_ambient
    @param: RGBAV1 _c_diffuse
    @param: RGBAV1 _c_specular
    @param: POINT4D_PTR _pos
    @param: VECTOR4D_PTR _dir
    @param: float _kc
    @param: float _kl
    @param: float _kq
    @param: float _spot_inner
    @param: float _spot_outer
    @param: float _pf
    *************************************************************************************/
    int Init_Light_LIGHTV1(int           index,      // index of light to create (0..MAX_LIGHTS-1)
        LightOnOffState          _state,      // state of light
        LightType          _attr,       // type of light, and extra qualifiers
        RGBAV1       _c_ambient,  // ambient light intensity
        RGBAV1       _c_diffuse,  // diffuse light intensity
        RGBAV1       _c_specular, // specular light intensity
        POINT4D_PTR  _pos,        // position of light
        VECTOR4D_PTR _dir,        // direction of light
        float        _kc,         // attenuation factors
        float        _kl,
        float        _kq,
        float        _spot_inner, // inner angle for spot light
        float        _spot_outer, // outer angle for spot light
        float        _pf);

    /**************************************************************************************

    @name: KSR::Light_OBJECT4DV1_World16
    @return: int
    @param: OBJECT4DV1_PTR obj  object to process
    @param: CAM4DV1_PTR cam     camera position
    @param: LIGHTV1_PTR lights  light list (might have more than one)
    @param: int max_lights      maximum lights in list
    *************************************************************************************/
    int Light_OBJECT4DV1_World16(OBJECT4DV1_PTR obj,  // object to process
        CAM4DV1_PTR cam,     // 
        LIGHTV1_PTR lights,  // 
        int max_lights);      // 

    int Insert_OBJECT4DV1_RENDERLIST4DV12(RENDERLIST4DV1_PTR rend_list,
        OBJECT4DV1_PTR obj,
        int insert_local,
        int lighting_on);
}
