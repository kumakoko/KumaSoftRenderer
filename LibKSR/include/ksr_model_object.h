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
#include "ksr_matrix.h"
#include "ksr_camera.h"
#include "ksr_transform.h"

namespace KSR
{
    constexpr int OBJECT4DV1_MAX_VERTICES = 1024;  // 64
    constexpr int OBJECT4DV1_MAX_POLYS = 1024; // 128

    // new
    constexpr int OBJECT4DV2_ATTR_SINGLE_FRAME = 0x0001; // single frame object (emulates ver 1.0)
    constexpr int OBJECT4DV2_ATTR_MULTI_FRAME = 0x0002; // multi frame object for .md2 support etc.
    constexpr int OBJECT4DV2_ATTR_TEXTURES = 0x0004; // flags if object contains textured polys?

    // an object based on a vertex list and list of polygons
    typedef struct OBJECT4DV1_TYP
    {
        int  id;            // 物体的数字ID
        char name[64];      // 物体的字符名称
        int  state;         // 本多边形的状态信息，比如多边形是否处于活动状态，是否被裁剪，等等，可参见POLY4DV1_STATE_ACTIVE等
        int  attr;          // 本多边形的物理属性，比如是否是双目的，是否反射光，是否透明的，等等，可参见POLY4DV1_ATTR_2SIDED等
        float avg_radius;   // 物体的平均半径，用于碰撞检测
        float max_radius;   // 物体的最大半径
        POINT4D world_pos;  // 物体在世界坐标系中的位置
        VECTOR4D dir;       // 物体在局部坐标系中的旋转角度，用户定义的坐标或单位方向向量
        VECTOR4D ux;
        VECTOR4D uy;
        VECTOR4D uz;        // 模型自身的基于局部坐标系的坐标轴，用来跟踪自身的朝向
        int num_vertices;   // 物体的顶点数
        int num_polys;      // 物体的多边形个数
        POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES]; // 用于存储顶点局部坐标的数组
        POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES]; // 存储变换后的顶点坐标的数组
        POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];           // 物体的多边形数组
    } OBJECT4DV1, * OBJECT4DV1_PTR;


    // 本model object结构，基于一个顶点列表和一个多边形列表
    // 该结构有更大的灵活性，支持分帧动画，即是说可以存储类似于quake .md2这一类的动画模型格式
    typedef struct OBJECT4DV2_TYP
    {
        int   id;               // 数字ID
        char  name[64];         // 物体名称，仅支持ASCII编码，64字节
        int   state;            // 物体状态
        int   attr;             // 物体属性
        int   mati;             // 材质编码，-1表示没有材质
        float* avg_radius;      // 物体的平均半径数组，可以用来执行碰撞检测，包含了[OBJECT4DV2_MAX_FRAMES]个元素;
        float* max_radius;      // 物体的最大半径数组，包含了[OBJECT4DV2_MAX_FRAMES]个元素[OBJECT4DV2_MAX_FRAMES];
        POINT4D world_pos;      // 物体在世界坐标系中的位置
        VECTOR4D dir;           // 物体在局部坐标系下的旋转角度，或者是方向向量
        VECTOR4D ux, uy, uz;    // 记录朝向的局部坐标轴，物体被旋转时他们将自动更新
        int num_vertices;                   // 每一个动画帧中包含的顶点数
        int num_frames;                     // 动画帧的帧数
        int total_vertices;                 // 全部的顶点数
        int curr_frame;                     // 当前帧，如果只有一帧，则为0
        VERTEX4DTV1_PTR vlist_local;        // 基于局部坐标系下的顶点的数组 [OBJECT4DV1_MAX_VERTICES];
        VERTEX4DTV1_PTR vlist_trans;        // 经过变换后的顶点数组[OBJECT4DV1_MAX_VERTICES]
        VERTEX4DTV1_PTR head_vlist_local;   // vlist_local链表的首指针
        VERTEX4DTV1_PTR head_vlist_trans;   // vlist_trans链表的首指针
        POINT2D_PTR tlist;                  // 纹理坐标列表， 3*最大的多边形数
        BITMAP_IMAGE_PTR texture;           // 新增的指向纹理信息的指针，可用于纹理映射
        int num_polys;                      // 物体网格中的多边形数
        POLY4DV2_PTR plist;                 // 新增的指向多边形的指针
        int ivar1, ivar2;                   // auxiliary vars
        float fvar1, fvar2;                 // auxiliary vars

        /**************************************************************************************
         指定当前帧
         * @name: Set_Frame
         * @return: bool
         * @param: int frame
         *************************************************************************************/
        bool Set_Frame(int frame);
    } OBJECT4DV2, * OBJECT4DV2_PTR;

    /**************************************************************************************

    @name: KSR::Compute_OBJECT4DV1_Radius
    @return: float
    @param: OBJECT4DV1_PTR obj
    *************************************************************************************/
    float Compute_OBJECT4DV1_Radius(OBJECT4DV1_PTR obj);

    /**************************************************************************************

     * @name: Load_OBJECT4DV1_PLG
     * @return: bool
     * @param: OBJECT4DV1_PTR obj
     * @param: const char * filename
     * @param: VECTOR4D_PTR scale initial scaling factors
     * @param: VECTOR4D_PTR pos   initial position
     * @param: VECTOR4D_PTR rot
     *************************************************************************************/
    bool Load_OBJECT4DV1_PLG(OBJECT4DV1_PTR obj, const char* filename, VECTOR4D_PTR scale, VECTOR4D_PTR pos, VECTOR4D_PTR rot);

    /**************************************************************************************

     * @name: Reset_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     *************************************************************************************/
    void Reset_OBJECT4DV1(OBJECT4DV1_PTR obj);

    /**************************************************************************************
     利用传递进函数的参数：变换矩阵mt，对object中的基于局部坐标系顶点，或者在其他坐标系下的顶点
     进行变换

     * @name: Transform_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj object to transform
     * @param: MATRIX4X4_PTR mt transformation matrix
     * @param: TransformControlFlag coord_select  selects coords to transform
     * @param: bool transform_basis 指定是否要对model object自身的朝向向量进行变换
     *************************************************************************************/
    void Transform_OBJECT4DV1(OBJECT4DV1_PTR obj, MATRIX4X4_PTR mt, TransformControlFlag coord_select, bool transform_basis);

    /**************************************************************************************

     * @name: Model_To_World_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: int coord_select
     *************************************************************************************/
    void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);

    /**************************************************************************************

     * @name: Remove_Backfaces_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: CAM4DV1_PTR cam
     *************************************************************************************/
    void Remove_Backfaces_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam);

    /**************************************************************************************

     * @name: World_To_Camera_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: CAM4DV1_PTR cam
     *************************************************************************************/
    void World_To_Camera_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam);

    /**************************************************************************************

     * @name: Camera_To_Perspective_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: CAM4DV1_PTR cam
     *************************************************************************************/
    void Camera_To_Perspective_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam);

    /**************************************************************************************

     * @name: Camera_To_Perspective_Screen_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: CAM4DV1_PTR cam
     *************************************************************************************/
    void Camera_To_Perspective_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam);

    /**************************************************************************************

     * @name: Perspective_To_Screen_OBJECT4DV1
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: CAM4DV1_PTR cam
     *************************************************************************************/
    void Perspective_To_Screen_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam);

    /**************************************************************************************
     详细解释参见KSR：：Draw_RENDERLIST4DV1_Wire16函数
     * @name: Draw_OBJECT4DV1_Wire16
     * @return: void
     * @param: OBJECT4DV1_PTR obj
     * @param: uint8_t * video_buffer
     * @param: int lpitch
     *************************************************************************************/
    void Draw_OBJECT4DV1_Wire16(OBJECT4DV1_PTR obj, uint8_t* video_buffer, int pitch);

    /**************************************************************************************
    对本model object进行剔除，使得其在不必要的情况下不走完整个流水线流程。本函数在观察空间中执行剔除
    @name: KSR::Cull_OBJECT4DV1
    @return: bool 被剔除返回true，否则返回false
    @param: OBJECT4DV1_PTR obj object to cull
    @param: CAM4DV1_PTR        cam camera to cull relative to
    @param: int cull_flags     clipping planes to consider
    *************************************************************************************/
    bool Cull_OBJECT4DV1(OBJECT4DV1_PTR obj, CAM4DV1_PTR cam, int cull_flags);
}