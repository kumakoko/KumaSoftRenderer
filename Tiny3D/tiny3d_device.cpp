#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tiny3d_device.h"
#include "tiny3d_math.h"


void device_t::device_init(int width, int height)
{
    this->texture_width_ = 256;
    this->texture_height_ = 256;
    this->texture_ = new uint32_t[this->texture_width_ * this->texture_height_];
    this->z_buffer_ = new float[width * height];
    this->max_u_ = 1.0f;
    this->max_v_ = 1.0f;
    this->window_width_ = width;
    this->window_height_ = height;
    this->background_color_ = 0xFFc0c0c0;
    this->foreground_color_ = 0xFFFFFFFF;
    this->render_state_ = RENDER_STATE_TEXTURE;
    float near_clip = 1.0f;
    float far_clip = 500.0f;
    this->transform_.Init(width, height, near_clip, far_clip);
}

// 删除设备
void device_t::device_destroy()
{
    this->frame_buffer_ = nullptr;
    delete[] this->z_buffer_;
    this->z_buffer_ = nullptr;
    delete[] this->texture_;
    this->texture_ = nullptr;
}

// 清空 framebuffer 和 zbuffer
void device_t::device_clear(int mode)
{
    // 清空zbuffer
    for (uint32_t y = 0; y < this->window_height_ * this->window_width_; y++)
    {
        this->z_buffer_[y] = 0;
    }
}

// 画点
void device_t::WritePixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x < this->window_width_ && y < this->window_height_)
    {
        uint32_t idx = x + this->window_width_ * y;
        this->frame_buffer_[idx] = color;
    }
}

// 绘制线段
void device_t::DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t c)
{
    uint32_t x, y, rem = 0;

    if (x1 == x2 && y1 == y2)
    {
        WritePixel(x1, y1, c);
    }
    else if (x1 == x2)
    {
        int32_t inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc)
            WritePixel(x1, y, c);
        WritePixel(x2, y2, c);
    }
    else if (y1 == y2)
    {
        int32_t inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc) WritePixel(x, y1, c);
        WritePixel(x2, y2, c);
    }
    else
    {
        uint32_t dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        uint32_t dy = (y1 < y2) ? y2 - y1 : y1 - y2;

        if (dx >= dy)
        {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;

            for (x = x1, y = y1; x <= x2; x++)
            {
                WritePixel(x, y, c);
                rem += dy;

                if (rem >= dx)
                {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    WritePixel(x, y, c);
                }
            }

            WritePixel(x2, y2, c);
        }
        else
        {
            if (y2 < y1)
                x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;

            for (x = x1, y = y1; y <= y2; y++)
            {
                WritePixel(x, y, c);
                rem += dx;

                if (rem >= dy)
                {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    WritePixel(x, y, c);
                }
            }
            WritePixel(x2, y2, c);
        }
    }
}

// 根据坐标读取纹理
uint32_t device_t::GetTexel(float u, float v)
{
    int x, y;
    u = u * this->max_u_;
    v = v * this->max_v_;
    x = static_cast<int>(u + 0.5f);
    y = static_cast<int>(v + 0.5f);
    x = CMID(x, 0, this->texture_width_ - 1);
    y = CMID(y, 0, this->texture_height_ - 1);
    return this->texture_[y * texture_width_ + x];
}

// 绘制扫描线
void device_t::DrawScanline(scanline_t* scanline)
{
    uint32_t* fb = this->frame_buffer_ + this->window_width_ * scanline->y;
    float* zbuffer = this->z_buffer_ + this->window_width_ * scanline->y;
    int x = scanline->x;
    int w = scanline->w;
    int width = this->window_width_;
    int render_state = this->render_state_;

    for (; w > 0; x++, w--)
    {
        if (x >= 0 && x < width) // 只绘制在屏幕内扫描线部分
        {
            float rhw = scanline->v.rhw;

            if (rhw >= zbuffer[x]) // 比较Z缓冲区值
            {
                float w = 1.0f / rhw;
                zbuffer[x] = rhw;

                if (render_state & RENDER_STATE_COLOR)
                {
                    float r = scanline->v.color.r * w;
                    float g = scanline->v.color.g * w;
                    float b = scanline->v.color.b * w;
                    uint32_t R = static_cast<uint32_t>(r * 255.0f);
                    uint32_t G = static_cast<uint32_t>(g * 255.0f);
                    uint32_t B = static_cast<uint32_t>(b * 255.0f);
                    R = CMID(R, 0, 255);
                    G = CMID(G, 0, 255);
                    B = CMID(B, 0, 255);
                    fb[x] = 0xFF000000 | (R << 16) | (G << 8) | (B);
                }

                if (render_state & RENDER_STATE_TEXTURE)
                {
                    float u = scanline->v.tc.u * w;
                    float v = scanline->v.tc.v * w;
                    uint32_t cc = GetTexel(u, v);
                    fb[x] = 0xFF000000 | cc;
                }
            }
        }

        // 从左腰边的插值点开始，每循环一次递加一次【插值步】，然后进行渲染写入操作
        vertex_add(&scanline->v, &scanline->step);

        if (x >= width)
            break;
    }
}

// 主渲染函数，把一个梯形分解成若干条扫描线，然后绘制
// 扫描线
void device_t::RenderTrapezoid(trapezoid_t* trap)
{
    scanline_t scanline;
    uint32_t j, top, bottom;
    top = static_cast<int>(trap->top + 0.5f);           // 拿到梯形的顶边和底边Y坐标
    bottom = static_cast<int>(trap->bottom + 0.5f);

    for (j = top; j < bottom; j++)
    {
        if (j >= 0 && j < this->window_height_) // 不能超出屏幕
        {
            trap->trapezoid_edge_interp(static_cast<float>(j) + 0.5f);

            // 算出插值点的梯形，接下来要算出扫描线
            trap->trapezoid_init_scan_line(&scanline, j);

            // 到了这一步，算出了每条扫描线的【插值步】数据，可以绘制每一条扫描线
            DrawScanline(&scanline);
        }

        if (j >= this->window_height_)
            break;
    }
}

// 根据 render_state 绘制原始三角形
void device_t::DrawPrimitive(const vertex_t* v1, const vertex_t* v2, const vertex_t* v3)
{
    point_t p1, p2, p3, c1, c2, c3;
    int render_state = this->render_state_;

    // 把传递进来的顶点，乘以WVP矩阵，变换到裁剪空间
    this->transform_.Apply(&c1, &v1->pos);
    this->transform_.Apply(&c2, &v2->pos);
    this->transform_.Apply(&c3, &v3->pos);

    // 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
    // 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
    if (this->transform_.CheckCVV(&c1) != 0)
        return;
    if (this->transform_.CheckCVV(&c2) != 0)
        return;
    if (this->transform_.CheckCVV(&c3) != 0)
        return;

    // 把裁剪空间归一化到齐次的NDC空间
    this->transform_.Homogenize(&p1, &c1);
    this->transform_.Homogenize(&p2, &c2);
    this->transform_.Homogenize(&p3, &c3);

    // 纹理或者色彩绘制
    if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR))
    {
        vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
        std::array<trapezoid_t,2> traps;
        int n;

        t1.pos = p1;
        t2.pos = p2;
        t3.pos = p3;
        t1.pos.w = c1.w;
        t2.pos.w = c2.w;
        t3.pos.w = c3.w;

        vertex_rhw_init(&t1); // 初始化 w
        vertex_rhw_init(&t2); // 初始化 w
        vertex_rhw_init(&t3); // 初始化 w

        // 拆分三角形为0-2个梯形，并且返回可用梯形数量
        n = trapezoid_t::trapezoid_init_triangle(traps, &t1, &t2, &t3);

        if (n >= 1)
            RenderTrapezoid(&traps[0]);
        if (n >= 2)
            RenderTrapezoid(&traps[1]);
    }

    if (render_state & RENDER_STATE_WIREFRAME) // 线框绘制
    {
        uint32_t p1x = static_cast<uint32_t>(p1.x);
        uint32_t p2x = static_cast<uint32_t>(p2.x);
        uint32_t p3x = static_cast<uint32_t>(p3.x);
        uint32_t p1y = static_cast<uint32_t>(p1.y);
        uint32_t p2y = static_cast<uint32_t>(p2.y);
        uint32_t p3y = static_cast<uint32_t>(p3.y);
        DrawLine(p1x, p1y, p2x, p2y, this->foreground_color_);
        DrawLine(p1x, p1y, p3x, p3y, this->foreground_color_);
        DrawLine(p3x, p3y, p2x, p2y, this->foreground_color_);
    }
}

void device_t::ResetCamera(float x, float y, float z)
{
    point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
    this->transform_.SetViewMatrix(&eye, &at, &up);//matrix_set_lookat(&this->transform.view, &eye, &at, &up);
    this->transform_.Update();
}

void device_t::InitTexture()
{
    this->texture_ = new uint32_t[texture_width_ * texture_height_];

    for (uint32_t j = 0; j < texture_height_; j++)
    {
        for (uint32_t i = 0; i < texture_width_; i++)
        {
            uint32_t x = i / 32;
            uint32_t y = j / 32;
            this->texture_[j * 256 + i] = ((x + y) & 1) ? 0xFFFFFFFF : 0xFF3FBCEF;
        }
    }

    this->max_u_ = static_cast<float>(this->texture_width_ - 1);
    this->max_v_ = static_cast<float>(this->texture_height_ - 1);
}

void device_t::DrawPlane(const vertex_t* p1, const vertex_t* p2, const vertex_t* p3, const vertex_t* p4)
{
    vertex_t _p1 = *p1, _p2 = *p2, _p3 = *p3, _p4 = *p4;
    _p1.tc.u = 0.0f, _p1.tc.v = 0.0f;
    _p2.tc.u = 0.0f, _p2.tc.v = 1.0f;
    _p3.tc.u = 1.0f, _p3.tc.v = 1.0f;
    _p4.tc.u = 1.0f, _p4.tc.v = 0.0f;
    DrawPrimitive(&_p1, &_p2, &_p3);
    DrawPrimitive(&_p3, &_p4, &_p1);
}

void device_t::DrawBox(float theta,const vertex_t* box_vertices)
{
    matrix_t m;
    matrix_set_rotate(&m, -1, -0.5, 1, theta);
    transform_.SetWorldMatrix(m);
    transform_.Update();
    DrawPlane(&box_vertices[0], &box_vertices[1], &box_vertices[2], &box_vertices[3]);
    DrawPlane(&box_vertices[4], &box_vertices[5], &box_vertices[6], &box_vertices[7]);
    DrawPlane(&box_vertices[0], &box_vertices[4], &box_vertices[5], &box_vertices[1]);
    DrawPlane(&box_vertices[1], &box_vertices[5], &box_vertices[6], &box_vertices[2]);
    DrawPlane(&box_vertices[2], &box_vertices[6], &box_vertices[7], &box_vertices[3]);
    DrawPlane(&box_vertices[3], &box_vertices[7], &box_vertices[4], &box_vertices[0]);
}