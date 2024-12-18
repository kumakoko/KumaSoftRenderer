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

struct T3DVector4
{
    union
    {
#if defined(_MSC_VER)
        __declspec(align(16)) float m[4];
#else
        float m[4];
#endif
        struct
        {
            float x,y,z,w;
        };
    };
};

// 向量长度
float T3DVector4Length(const T3DVector4* v);

// 向量求和
void T3DVector4Add(T3DVector4* r, const T3DVector4* lhs, const T3DVector4* rhs);

// 向量求差
void T3DVector4Subtract(T3DVector4* r, const T3DVector4* lhs, const T3DVector4* rhs);

// 向量点乘
float T3DVector4Dot(const T3DVector4* x, const T3DVector4* y);

// 向量叉乘
void T3DVector4Cross(T3DVector4* r, const T3DVector4* a, const T3DVector4* b);

// 向量插值，t取值 [0, 1]
void T3DVector4Interpolate(T3DVector4* z, const T3DVector4* x1, const T3DVector4* x2, float t);

// 向量归一化
void T3DVector4Normalize(T3DVector4* v);