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
#include "ksr_matrix.h"
#include "ksr_vector.h"
#include "ksr_plane.h"

namespace KSR
{
    enum CameraModelType
    {
        CAM_MODEL_EULER = 0x0008, // 由欧拉角指定的相机
        CAM_MODEL_UVN = 0x0010    // 由UVN向量指定的相机
    };

    // 定义了摄像机绕轴旋转的先后顺序
    enum CameraRotationSequences
    {
        CAM_ROT_SEQ_XYZ = 0,
        CAM_ROT_SEQ_YXZ = 1,
        CAM_ROT_SEQ_XZY = 2,
        CAM_ROT_SEQ_YZX = 3,
        CAM_ROT_SEQ_ZYX = 4,
        CAM_ROT_SEQ_ZXY = 5
    };

    // defines for special types of camera projections
    enum CameraProjectionType
    {
        CAM_PROJ_NORMALIZED = 0x0001,
        CAM_PROJ_SCREEN = 0x0002,
        CAM_PROJ_FOV90 = 0x0004
    };

    enum UVNCameraType
    {
        UVN_MODE_SIMPLE = 0,    // 低级简单模型，使用目标位置和观察参考点
        UVN_MODE_SPHERICAL = 1  // 球面坐标模式，分量x和y被用作观察向量的方位角和仰角，观察参考点为相机位置
    };

    typedef struct CAM4DV1_TYP
    {
        int state;              // 相机当前的状态
        CameraModelType attr;   // 相机的属性，参见CameraModelType
        POINT4D pos;            // 相机自身在世界空间中的坐标
        VECTOR4D dir;           // 欧拉角相机模型或者UVN相机模型的注视方向。如果是欧拉角相机模型，xy分量分别表示仰角和俯视角 
        VECTOR4D u;             // UVN相机模型的朝向向量
        VECTOR4D v;
        VECTOR4D n;
        POINT4D target;         // UVN模型的注视目标
        float view_dist;        // focal length 
        float fov;              // 水平视野和垂直视野角
        float near_clip_z;      // 近裁剪面的z坐标值
        float far_clip_z;       // 远裁剪面的z坐标值
        PLANE3D rt_clip_plane;  // 视截体的右裁剪面
        PLANE3D lt_clip_plane;  // 视截体的左裁剪面
        PLANE3D tp_clip_plane;  // 视截体的上裁剪面
        PLANE3D bt_clip_plane;  // 视截体的下裁剪面                        
        float viewplane_width;  // 定义了视平面的高（宽）。物体将会投影到这视平面上，对于归一化投影（normalized projection），视平面
        float viewplane_height; // 定义为2x2大小，如果将透视变换和屏幕变换合二为一的话，视平面的大小和视口（viewport，屏幕窗口）相同
        float viewport_width;   // 视口（屏幕）的高（宽）
        float viewport_height;  // 视口（屏幕）的高（宽）
        float viewport_center_x;// 视口（屏幕）的中点坐标x分量
        float viewport_center_y;// 视口（屏幕）的中点坐标y分量
        float aspect_ratio;     // 视口（屏幕）的宽与高的比值
        MATRIX4X4 mcam;         // 世界空间到观察空间的变换矩阵
        MATRIX4X4 mper;         // 观察空间到透视投影空间的变换矩阵
        MATRIX4X4 mscr;         // 透视投影空间到屏幕空间的变换矩阵
    } CAM4DV1, * CAM4DV1_PTR;

    /**************************************************************************************

    @name: KSR::Init_CAM4DV1
    @return: void
    @param: CAM4DV1_PTR camera
    @param: CameraModelType camera_attribute    摄像机的类型，欧拉相机或者时UVN相机
    @param: POINT4D_PTR camera_pos              摄像机在世界坐标系下的位置值
    @param: VECTOR4D_PTR camera_direction       摄像机在世界坐标系下的观察朝向
    @param: POINT4D_PTR cam_target              如果摄像机是UVN相机的话，摄像机的观察目标在世界坐标系下的位置值
    @param: float near_clip_z                   近截平面
    @param: float far_clip_z                    远截平面
    @param: float fov                           FOV
    @param: float viewport_width                视口宽度
    @param: float viewport_height               视口高度
    *************************************************************************************/
    void Init_CAM4DV1(CAM4DV1_PTR camera, CameraModelType camera_attribute, POINT4D_PTR camera_pos, VECTOR4D_PTR camera_direction, 
        POINT4D_PTR camera_target, float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height);

    /**************************************************************************************
    
    @name: KSR::Build_CAM4DV1_Matrix_Euler
    @return: void
    @param: CAM4DV1_PTR cam
    @param: CameraRotationSequences cam_rot_seq
    *************************************************************************************/
    void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam, CameraRotationSequences cam_rot_seq);

    /**************************************************************************************
    本函数根据相机的注视向量n，向上向量v，和右向量u。创建一个相机变换矩阵，并将其存储到传入的相机
    对象中。
    参数mode指定如何计算uvn：

    @name: KSR::Build_CAM4DV1_Matrix_UVN
    @return: void
    @param: CAM4DV1_PTR cam 指定如何计算uvn：参见UVN_MODE_SIMPLE和UVN_MODE_SPHERICAL的说明
    @param: UVNCameraType mode
    *************************************************************************************/
    void Build_CAM4DV1_Matrix_UVN(CAM4DV1_PTR cam, UVNCameraType mode);
}
