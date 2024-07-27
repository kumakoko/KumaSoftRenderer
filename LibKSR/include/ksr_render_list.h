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

#include "ksr_vector.h"
#include "ksr_polygon.h"
#include "ksr_camera.h"
#include "ksr_transform.h"

namespace KSR
{
    constexpr int RENDERLIST4DV1_MAX_POLYS = 32768;
    constexpr int RENDERLIST4DV2_MAX_POLYS = 32768;

    typedef struct RENDERLIST4DV1_TYP
    {
        int state; // state of renderlist ???
        int attr;  // attributes of renderlist ???

        // the render list is an array of pointers each pointing to 
        // a self contained "renderable" polygon face POLYF4DV1
        POLYF4DV1_PTR poly_ptrs[RENDERLIST4DV1_MAX_POLYS];

        // additionally to cut down on allocatation, de-allocation
        // of polygons each frame, here's where the actual polygon
        // faces will be stored
        POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];

        int num_polys; // number of polys in render list

    } RENDERLIST4DV1, * RENDERLIST4DV1_PTR;


    // object to hold the render list version 2.0, this way we can have more
// than one render list at a time
    typedef struct RENDERLIST4DV2_TYP
    {
        int state; // state of renderlist ???
        int attr;  // attributes of renderlist ???

        // the render list is an array of pointers each pointing to 
        // a self contained "renderable" polygon face POLYF4DV2
        POLYF4DV2_PTR poly_ptrs[RENDERLIST4DV2_MAX_POLYS];

        // additionally to cut down on allocatation, de-allocation
        // of polygons each frame, here's where the actual polygon
        // faces will be stored
        POLYF4DV2 poly_data[RENDERLIST4DV2_MAX_POLYS];

        int num_polys; // number of polys in render list

    } RENDERLIST4DV2, * RENDERLIST4DV2_PTR;


    /**************************************************************************************

    @name: KSR::Reset_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR rend_list
    *************************************************************************************/
    void Reset_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list);

    /**************************************************************************************

    @name: KSR::Insert_POLYF4DV1_RENDERLIST4DV1
    @return: int
    @param: RENDERLIST4DV1_PTR rend_list
    @param: POLYF4DV1_PTR poly
    *************************************************************************************/
    int Insert_POLYF4DV1_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POLYF4DV1_PTR poly);

    /**************************************************************************************
    利用传递进函数的参数：变换矩阵mt，对render list中的基于局部坐标系顶点，或者在其他坐标系下的顶点
    进行变换
    @name: KSR::Transform_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR render_list      要变换的那个render list
    @param: MATRIX4X4_PTR mat_transform         用来进行变换操作的矩阵
    @param: TransformControlFlag coord_select   顶点坐标变换的控制标志，参见ksr_transform.h中
                                                TransformControlFlag枚举值的定义
    *************************************************************************************/
    void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mat_transform, TransformControlFlag coord_select);

    /**************************************************************************************
    把渲染列表中的多边形的顶点，从局部坐标系变换到世界坐标系。本函数并没有使用矩阵，而是直接利用位置
    平移量world_pos去计算
    
    当coord_select为TRANSFORM_LOCAL_TO_TRANS时：这函数将多边形的存储在vlist数组成员中的局部（模型）
    坐标，变换到世界坐标，变换后的结果，存储在多边形的tvlist数组成员变量中。

    当coord_select为TRANSFORM_TRANS_ONLY或者是TRANSFORM_LOCAL_ONLY时：这函数将多边形的存储在tvlist
    数组成员中的局部（模型）坐标，变换到世界坐标，变换后的结果，原样存回到tvlist数组成员变量中。

    @name: KSR::Model_To_World_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR render_list  要变换的那个render list
    @param: POINT4D_PTR world_pos           每一个顶点从局部坐标系变换到世界坐标系时，顶点的平移量
    @param: int coord_select                顶点坐标变换的控制标志，参见ksr_transform.h中
                                            TransformControlFlag枚举值的定义
    *************************************************************************************/
    void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, POINT4D_PTR world_pos, TransformControlFlag coord_select = TRANSFORM_LOCAL_TO_TRANS);


    /**************************************************************************************
    此函数根据传入的观察变换矩阵（相机变换矩阵），将渲染列表中的每个多边形的坐标变换到基于观察空间（相机空间）

    如果在流水线的上游已经将每个物体转换为多边形了且将其插入到render list中的话，就是使用此函数来
    进行变换。

    将物体转换为多边形的操作是在物体的剔除、把物体的局部坐标系变换到世界坐标系下，以及背面消除之后
    进行的。这样子做可以最大限度地减少了每个物体中被插入到渲染列表中的多边形的数量。

    本函数已经假设了多边形已经进行了局部坐标系到世界坐标系的变换。且这些多边形的坐标数据已经存储
    在成员变量tvlist中了

    @name: KSR::World_To_Camera_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR render_list 要变换的那个render list
    @param: CAM4DV1_PTR camera 当前执行渲染的camera
    *************************************************************************************/
    void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list,CAM4DV1_PTR camera);

    

    void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    

    void Camera_To_Perspective_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    

    void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    void Draw_RENDERLIST4DV1_Wire16(RENDERLIST4DV1_PTR rend_list, uint8_t* video_buffer, int lpitch);
}
