#pragma once

#include <cstdint>
#include "tiny3d_matrix.h"

class Transform
{
private:
    matrix_t world_matrix_;         // 世界坐标变换
    matrix_t view_matrix_;          // 摄影机坐标变换
    matrix_t projection_matrix_;    // 投影变换
    matrix_t wvp_matrix_;           // WVP矩阵，即 world * view * projection
    float screen_width_;            // 屏幕宽
    float screen_height_;           // 屏幕高
public:
    /**************************************************************************************
     矩阵更新，计算 transform = world * view * projection
    @name: transform_t::Update
    @return: void
    *************************************************************************************/
    void Update();

    /**************************************************************************************
    初始化，设置屏幕长宽
    @name: Transform::Init
    @return: void
    @param: int width
    @param: int height
    *************************************************************************************/
    void Init(int width, int height, float near_clip, float far_clip);

    /**************************************************************************************
    将矢量 x 进行 project
    @name: Transform::Apply
    @return: void
    @param: T3DVector4 * y
    @param: const T3DVector4 * x
    *************************************************************************************/
    void Apply(T3DVector4* y, const T3DVector4* x) const;

    /**************************************************************************************
    检查齐次坐标同 cvv 的边界用于视锥裁剪
    @name: Transform::CheckCVV
    @return: uint32_t
    @param: const T3DVector4 * v
    *************************************************************************************/
    uint32_t CheckCVV(const T3DVector4* v) const;

    /**************************************************************************************
    归一化，得到屏幕坐标
    @name: Transform::Homogenize
    @return: void
    @param: T3DVector4 * homogenized_vertex
    @param: const T3DVector4 * vertex
    *************************************************************************************/
    void Homogenize(T3DVector4* homogenized_vertex, const T3DVector4* vertex) const;

    inline void SetViewMatrix(const T3DVector4* eye, const T3DVector4* at, const T3DVector4* up)
    {
        matrix_set_lookat(&view_matrix_, eye, at, up);
    }

    inline void SetWorldMatrix(const matrix_t& m)
    {
        world_matrix_ = m;
    }
};