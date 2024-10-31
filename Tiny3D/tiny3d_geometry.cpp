#include "tiny3d_geometry.h"
#include "tiny3d_math.h"

void vertex_rhw_init(vertex_t* v)
{
    float rhw = 1.0f / v->pos.w;
    v->rhw = rhw;
    v->tc.u *= rhw;
    v->tc.v *= rhw;
    v->color.r *= rhw;
    v->color.g *= rhw;
    v->color.b *= rhw;
}

void vertex_interp(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float t)
{
    vector_interp(&y->pos, &x1->pos, &x2->pos, t);
    y->tc.u = interp(x1->tc.u, x2->tc.u, t);
    y->tc.v = interp(x1->tc.v, x2->tc.v, t);
    y->color.r = interp(x1->color.r, x2->color.r, t);
    y->color.g = interp(x1->color.g, x2->color.g, t);
    y->color.b = interp(x1->color.b, x2->color.b, t);
    y->rhw = interp(x1->rhw, x2->rhw, t);
}


// x1 梯形左腰边的插值点
// x2 梯形右腰边的插值点
void vertex_division(vertex_t* step, const vertex_t* x1, const vertex_t* x2, float w)
{
    float inv = 1.0f / w;
    step->pos.x = (x2->pos.x - x1->pos.x) * inv;
    step->pos.y = (x2->pos.y - x1->pos.y) * inv;
    step->pos.z = (x2->pos.z - x1->pos.z) * inv;
    step->pos.w = (x2->pos.w - x1->pos.w) * inv;
    step->tc.u = (x2->tc.u - x1->tc.u) * inv;
    step->tc.v = (x2->tc.v - x1->tc.v) * inv;
    step->color.r = (x2->color.r - x1->color.r) * inv;
    step->color.g = (x2->color.g - x1->color.g) * inv;
    step->color.b = (x2->color.b - x1->color.b) * inv;
    step->rhw = (x2->rhw - x1->rhw) * inv;
}

void vertex_add(vertex_t* y, const vertex_t* x)
{
    y->pos.x += x->pos.x;
    y->pos.y += x->pos.y;
    y->pos.z += x->pos.z;
    y->pos.w += x->pos.w;
    y->rhw += x->rhw;
    y->tc.u += x->tc.u;
    y->tc.v += x->tc.v;
    y->color.r += x->color.r;
    y->color.g += x->color.g;
    y->color.b += x->color.b;
}