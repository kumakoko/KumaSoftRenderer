#pragma once

struct vector_t
{
    float x, y, z, w;
};

typedef vector_t point_t;

// 向量长度
float vector_length(const vector_t* v);

// 向量求和
void vector_add(vector_t* z, const vector_t* x, const vector_t* y);

// 向量求差
void vector_sub(vector_t* z, const vector_t* x, const vector_t* y);

// 向量点乘
float vector_dotproduct(const vector_t* x, const vector_t* y);

// 向量叉乘
void vector_crossproduct(vector_t* z, const vector_t* x, const vector_t* y);

// 向量插值，t取值 [0, 1]
void vector_interp(vector_t* z, const vector_t* x1, const vector_t* x2, float t);

// 向量归一化
void vector_normalize(vector_t* v);