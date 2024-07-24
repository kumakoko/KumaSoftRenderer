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

    @name: KSR::Transform_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR rend_list 要变换的那个render list
    @param: MATRIX4X4_PTR mt 用来进行变换操作的矩阵
    @param: TransformControlFlag coord_select 要变换过去的坐标系
    *************************************************************************************/
    void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, MATRIX4X4_PTR mt, TransformControlFlag coord_select);

    /**************************************************************************************

    @name: KSR::Model_To_World_RENDERLIST4DV1
    @return: void
    @param: RENDERLIST4DV1_PTR rend_list
    @param: POINT4D_PTR world_pos
    @param: int coord_select
    *************************************************************************************/
    void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list, POINT4D_PTR world_pos, TransformControlFlag coord_select = TRANSFORM_LOCAL_TO_TRANS);


    void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    ///////////////////////////////////////////////////////////////

    void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    ////////////////////////////////////////////////////////////////

    void Camera_To_Perspective_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);

    //////////////////////////////////////////////////////////////

    void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1_PTR rend_list,CAM4DV1_PTR cam);
}
