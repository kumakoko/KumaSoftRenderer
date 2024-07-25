#pragma once
#include "ksr_vector.h"
#include "ksr_matrix.h"

namespace KSR
{
    enum TransformControlFlag
    {
        // transformation control flags
        TRANSFORM_LOCAL_ONLY = 0, // perform the transformation in place on the local/world vertex list 
        TRANSFORM_TRANS_ONLY = 1, // perfrom the transformation in place on the  "transformed" vertex list
        TRANSFORM_LOCAL_TO_TRANS = 2// perform the transformation to the local vertex list, but store the results in the transformed vertex list
    };

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR3D_4X4
    @return: void
    @param: VECTOR3D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR3D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR va, MATRIX4X4_PTR mb, VECTOR3D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR3D_4X3
    @return: void
    @param: VECTOR3D_PTR va
    @param: MATRIX4X3_PTR mb
    @param: VECTOR3D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR3D_4X3(VECTOR3D_PTR va, MATRIX4X3_PTR mb, VECTOR3D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR4D_4X4
    @return: void
    @param: VECTOR4D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR4D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod);

    /**************************************************************************************
    
    @name: KSR::Mat_Mul_VECTOR4D_4X3
    @return: void
    @param: VECTOR4D_PTR va
    @param: MATRIX4X4_PTR mb
    @param: VECTOR4D_PTR vprod
    *************************************************************************************/
    void Mat_Mul_VECTOR4D_4X3(VECTOR4D_PTR va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod);
}
