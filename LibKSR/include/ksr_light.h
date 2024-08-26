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

    typedef struct LIGHTV1_TYP
    {
        int state;          // 光源的状态
        int id;             // 光源ID
        int attr;           // 光源的类型和其他的属性
        RGBAV1 c_ambient;   // 环境光强度
        RGBAV1 c_diffuse;   // 散射光强度
        RGBAV1 c_specular;  // 镜面反射光强度
        POINT4D  pos;       // 光源位置
        VECTOR4D dir;       // 光源方向
        float kc, kl, kq;   // 常数衰减因子 线性衰减因子  二次衰减因子
        float spot_inner;   // 聚光灯内锥角
        float spot_outer;   // 聚光灯外锥角
        float pf;           // 聚光灯指数因子
        int   iaux1, iaux2; // auxiliary vars for future expansion
        float faux1, faux2;
        void* ptr;
    } LIGHTV1, * LIGHTV1_PTR;

    extern LIGHTV1 lights[MAX_LIGHTS];  // 全局的光源数组lights in system
    extern int num_lights;              // 当前的光源数

    /**************************************************************************************

    @name: KSR::Reset_Lights_LIGHTV1
    @return: void
    *************************************************************************************/
    void Reset_Lights_LIGHTV1();

    /**************************************************************************************

    @name: KSR::Init_Light_LIGHTV1
    @return: int
    @param: int index               要创建的光源的索引，从0到MAX_LIGHTS-1
    @param: LightOnOffState state   光源的状态
    @param: LightType light_type    光源的类型和其他的属性
    @param: RGBAV1 ambient          环境光强度
    @param: RGBAV1 diffuse          散射光强度
    @param: RGBAV1 specular         镜面反射光强度
    @param: POINT4D_PTR position    光源位置
    @param: VECTOR4D_PTR direction  光源方向
    @param: float kc                常数衰减因子
    @param: float kl                线性衰减因子
    @param: float kq                二次衰减因子
    @param: float spot_inner        聚光灯内锥角
    @param: float spot_outer        聚光灯外锥角
    @param: float _pf               聚光灯指数因子
    *************************************************************************************/
    int Init_Light_LIGHTV1(int index, LightOnOffState state, LightType light_type, RGBAV1 ambient,
        RGBAV1 diffuse, RGBAV1 specular, POINT4D_PTR position, VECTOR4D_PTR direction,
        float kc, float kl, float kq, float spot_inner, float spot_outer, float pf);

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
}
