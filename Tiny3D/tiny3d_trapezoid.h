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

#include <array>
#include "tiny3d_geometry.h"
#include "tiny3d_scanline.h"

class Trapezoid
{
public:
    inline float top() const
    {
        return top_;
    }
    
    inline void set_top(float val) 
    {
        top_ = val; 
    }
    
    inline float bottom() const
    {
        return bottom_;
    }
    
    inline void set_bottom(float val) 
    {
        bottom_ = val; 
    }
    
    inline const edge_t& left() const
    {
        return left_;
    }

    inline edge_t& left()
    {
        return left_;
    }
    
    inline void set_left(const edge_t* val)
    {
        left_ = *val;
    }
    
    inline void set_left(const edge_t& val)
    {
        left_ = val;
    }
    
    inline const edge_t& right() const
    {
        return right_; 
    }

    inline edge_t& right()
    {
        return right_;
    }
    
    inline void set_right(const edge_t& val) 
    {
        right_ = val;
    }

    inline void set_right(const edge_t* val)
    {
        right_ = *val;
    }

    /**************************************************************************************
    根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
    @name: trapezoid_t::SplitTriangleIntoTrapezoids
    @return: int
    @param: std::array<trapezoid_t
    @param: 2> & trap
    @param: const vertex_t * p1
    @param: const vertex_t * p2
    @param: const vertex_t * p3
    *************************************************************************************/
    static int SplitTriangleIntoTrapezoids(std::array<Trapezoid, 2>& trap, const T3DVertex* p1, const T3DVertex* p2, const T3DVertex* p3);

    // 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
    /**************************************************************************************
    根据给定的y值，计算出本梯形的两条腰边的
    @name: trapezoid_t::CalculateEdgeInterpolatedPoint
    @return: void
    @param: float y
    *************************************************************************************/
    void CalculateEdgeInterpolatedPoint(float y);

    /**************************************************************************************
    根据左右两边的端点，初始化计算出扫描线的起点和步长
    @name: trapezoid_t::InitializeScanline
    @return: void
    @param: scanline_t * scanline
    @param: int y
    *************************************************************************************/
    void InitializeScanline(scanline_t* scanline, int y);
private:
    float top_;
    float bottom_;
    edge_t left_;   // 左腰边
    edge_t right_;  // 右腰边
};
