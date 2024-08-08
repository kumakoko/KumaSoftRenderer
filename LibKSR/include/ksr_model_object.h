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

    // P509
    // an object ver 2.0 based on a vertex list and list of polygons //////////////////////////
// this new object has a lot more flexibility and it supports "framed" animation
// that is this object can hold hundreds of frames of an animated mesh as long as
// the mesh has the same polygons and geometry, but with changing vertex positions
// similar to the Quake II .md2 format
    typedef struct OBJECT4DV2_TYP
    {
        int   id;           // numeric id of this object
        char  name[64];     // ASCII name of object just for kicks
        int   state;        // state of object
        int   attr;         // attributes of object
        int   mati;         // material index overide (-1) - no material (new)
        float* avg_radius;  // [OBJECT4DV2_MAX_FRAMES];   // average radius of object used for collision detection
        float* max_radius;  // [OBJECT4DV2_MAX_FRAMES];   // maximum radius of object

        POINT4D world_pos;  // position of object in world

        VECTOR4D dir;       // rotation angles of object in local
        // cords or unit direction vector user defined???

        VECTOR4D ux, uy, uz;  // local axes to track full orientation
        // this is updated automatically during
        // rotation calls

        int num_vertices;   // number of vertices per frame of this object
        int num_frames;     // number of frames
        int total_vertices; // total vertices, redudant, but it saves a multiply in a lot of places
        int curr_frame;     // current animation frame (0) if single frame

        VERTEX4DTV1_PTR vlist_local; // [OBJECT4DV1_MAX_VERTICES]; // array of local vertices
        VERTEX4DTV1_PTR vlist_trans; // [OBJECT4DV1_MAX_VERTICES]; // array of transformed vertices

        // these are needed to track the "head" of the vertex list for mult-frame objects
        VERTEX4DTV1_PTR head_vlist_local;
        VERTEX4DTV1_PTR head_vlist_trans;

        // texture coordinates list (new)
        POINT2D_PTR tlist;       // 3*num polys at max

        BITMAP_IMAGE_PTR texture; // pointer to the texture information for simple texture mapping (new)

        int num_polys;           // number of polygons in object mesh
        POLY4DV2_PTR plist;      // ptr to polygons (new)

        int   ivar1, ivar2;      // auxiliary vars
        float fvar1, fvar2;      // auxiliary vars

        // METHODS //////////////////////////////////////////////////

        // setting the frame is so important that it should be a member function
        // calling functions without doing this can wreak havok!
        int Set_Frame(int frame);

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