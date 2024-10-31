#pragma once

#include <cstdint>
#include "tiny3d_matrix.h"

struct transform_t
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
    @name: transform_t::Init
    @return: void
    @param: int width
    @param: int height
    *************************************************************************************/
    void Init(int width, int height, float near_clip, float far_clip);

    /**************************************************************************************
    将矢量 x 进行 project
    @name: transform_t::Apply
    @return: void
    @param: vector_t * y
    @param: const vector_t * x
    *************************************************************************************/
    void Apply(vector_t* y, const vector_t* x) const;

    /**************************************************************************************
    检查齐次坐标同 cvv 的边界用于视锥裁剪
    @name: transform_t::CheckCVV
    @return: uint32_t
    @param: const vector_t * v
    *************************************************************************************/
    uint32_t CheckCVV(const vector_t* v) const;

    /**************************************************************************************
    归一化，得到屏幕坐标
    @name: transform_t::Homogenize
    @return: void
    @param: vector_t * homogenized_vertex
    @param: const vector_t * vertex
    *************************************************************************************/
    void Homogenize(vector_t* homogenized_vertex, const vector_t* vertex) const;

    inline void SetViewMatrix(const vector_t* eye, const vector_t* at, const vector_t* up)
    {
        matrix_set_lookat(&view_matrix_, eye, at, up);
    }

    inline void SetWorldMatrix(const matrix_t& m)
    {
        world_matrix_ = m;
    }
};