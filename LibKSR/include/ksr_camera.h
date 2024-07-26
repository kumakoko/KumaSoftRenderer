﻿/*********************************************************************************************
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
        CAM_MODEL_EULER = 0x0008,
        CAM_MODEL_UVN = 0x0010
    };

    // defines for camera rotation sequences
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
        UVN_MODE_SIMPLE = 0,
        UVN_MODE_SPHERICAL = 1
    };

    typedef struct CAM4DV1_TYP
    {
        int state;      // state of camera
        int attr;       // camera attributes

        POINT4D pos;    // world position of camera used by both camera models

        VECTOR4D dir;   // angles or look at direction of camera for simple 
        // euler camera models, elevation and heading for
        // uvn model

        VECTOR4D u;     // extra vectors to track the camera orientation
        VECTOR4D v;     // for more complex UVN camera model
        VECTOR4D n;

        VECTOR4D target; // look at target

        float view_dist;  // focal length 

        float fov;          // field of view for both horizontal and vertical axes

        // 3d clipping planes
        // if view volume is NOT 90 degree then general 3d clipping
        // must be employed
        float near_clip_z;     // near z=constant clipping plane
        float far_clip_z;      // far z=constant clipping plane

        PLANE3D rt_clip_plane;  // the right clipping plane
        PLANE3D lt_clip_plane;  // the left clipping plane
        PLANE3D tp_clip_plane;  // the top clipping plane
        PLANE3D bt_clip_plane;  // the bottom clipping plane                        

        float viewplane_width;     // 视平面，物体将会投影到这视平面上，width and height of view plane to project onto
        float viewplane_height;    // usually 2x2 for normalized projection or 
        // the exact same size as the viewport or screen window

// remember screen and viewport are synonomous 
        float viewport_width;     // size of screen/viewport
        float viewport_height;
        float viewport_center_x;  // center of view port (final image destination)
        float viewport_center_y;

        // aspect ratio
        float aspect_ratio;

        // these matrices are not necessarily needed based on the method of
        // transformation, for example, a manual perspective or screen transform
        // and or a concatenated perspective/screen, however, having these 
        // matrices give us more flexibility         

        MATRIX4X4 mcam;   // 世界空间到观察空间的变换矩阵 storage for the world to camera transform matrix
        MATRIX4X4 mper;   // 观察空间到透视投影空间的变换矩阵 storage for the camera to perspective transform matrix
        MATRIX4X4 mscr;   // 透视投影空间到屏幕空间的变换矩阵 storage for the perspective to screen transform matrix

    } CAM4DV1, * CAM4DV1_PTR;

    /**************************************************************************************

    @name: KSR::Init_CAM4DV1
    @return: void
    @param: CAM4DV1_PTR cam
    @param: CameraModelType cam_attr    摄像机的类型，欧拉相机或者时UVN相机
    @param: POINT4D_PTR cam_pos         摄像机在世界坐标系下的位置值
    @param: VECTOR4D_PTR cam_dir        摄像机在世界坐标系下的观察朝向
    @param: POINT4D_PTR cam_target      如果摄像机是UVN相机的话，摄像机的观察目标在世界坐标系下的位置值
    @param: float near_clip_z           近截平面
    @param: float far_clip_z            远截平面
    @param: float fov                   Field of View
    @param: float viewport_width        视口宽度
    @param: float viewport_height       视口高度
    *************************************************************************************/
    void Init_CAM4DV1(CAM4DV1_PTR cam, CameraModelType  cam_attr, POINT4D_PTR cam_pos, VECTOR4D_PTR cam_dir, 
        POINT4D_PTR cam_target, float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height);

    /**************************************************************************************
    
    @name: KSR::Build_CAM4DV1_Matrix_Euler
    @return: void
    @param: CAM4DV1_PTR cam
    @param: CameraRotationSequences cam_rot_seq
    *************************************************************************************/
    void Build_CAM4DV1_Matrix_Euler(CAM4DV1_PTR cam, CameraRotationSequences cam_rot_seq);

    /**************************************************************************************
    
    @name: KSR::Build_CAM4DV1_Matrix_UVN
    @return: void
    @param: CAM4DV1_PTR cam
    @param: UVNCameraType mode
    *************************************************************************************/
    void Build_CAM4DV1_Matrix_UVN(CAM4DV1_PTR cam, UVNCameraType mode);
}
