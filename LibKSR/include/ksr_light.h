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

#include "ksr_color.h"
#include "ksr_vector.h"
#include "ksr_model_object.h"
#include "ksr_camera.h"

namespace KSR
{

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

    } LIGHTV1, *LIGHTV1_PTR;
    
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
    @param: int _state
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
        int          _state,      // state of light
        int          _attr,       // type of light, and extra qualifiers
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
