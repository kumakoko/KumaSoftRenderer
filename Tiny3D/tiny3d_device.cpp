#include <cstdlib>
#include <cassert>
#include <cstring>
#include "SDL.h"
#include "SDL_image.h"

#include "tiny3d_device.h"
#include "tiny3d_math.h"
#include "tiny3d_error.h"


void Device::Initialize(int width, int height)
{
    this->texture_width_ = 0;
    this->texture_height_ = 0;
    this->texture_ = nullptr;
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
void Device::Destroy()
{
    this->frame_buffer_ = nullptr;
    delete[] this->z_buffer_;
    this->z_buffer_ = nullptr;
    delete[] this->texture_;
    this->texture_ = nullptr;
}

// 清空 framebuffer 和 zbuffer
void Device::ResetZBuffer()
{
    // 清空zbuffer
    for (uint32_t y = 0; y < this->window_height_ * this->window_width_; y++)
    {
        this->z_buffer_[y] = 0;
    }
}

// 画点
void Device::WritePixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x < this->window_width_ && y < this->window_height_)
    {
        uint32_t idx = x + this->window_width_ * y;
        this->frame_buffer_[idx] = color;
    }
}

// 绘制线段
void Device::DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t c)
{
    uint32_t x, y, rem = 0;

    // 首先检查 x1 == x2 && y1 == y2，即起点和终点重合的情况。这种情况下，仅需在 x1, y1
    // 位置调用 WritePixel 写入颜色 c 即可。
    if (x1 == x2 && y1 == y2)
    {
        WritePixel(x1, y1, c);
    }
    else if (x1 == x2) // o	如果 x1 == x2，说明线段垂直
    {
        // 通过检查 y1 <= y2 来确定绘制的方向（向上或向下）
        int32_t inc = (y1 <= y2) ? 1 : -1;

        for (y = y1; y != y2; y += inc)
        {
            // 从 y1行递增或递减到y2行，每次在x1列调用 WritePixel绘制
            WritePixel(x1, y, c);
        }

        WritePixel(x2, y2, c); // 最后，在x2,y2位置再调用一次WritePixel以确保终点被绘制
    }
    else if (y1 == y2) // 如果 y1 == y2，说明线段水平
    {
        // 如果 y1 == y2，说明线段水平。同样，通过检查 x1 <= x2 来确定绘制方向（向左或向右），
        // 然后在y1行从x1列到x2列逐步绘制。最后，调用 WritePixel(x2, y2, c) 绘制终点。
        int32_t inc = (x1 <= x2) ? 1 : -1;

        for (x = x1; x != x2; x += inc)
        {
            WritePixel(x, y1, c);
        }

        WritePixel(x2, y2, c); // 最后，在x2,y2位置再调用一次WritePixel以确保终点被绘制
    }
    else
    {
        // 如果线段既不是水平线也不是垂直线，将根据 dx（水平距离）和 dy（垂直距离）来选择绘制主方向

        uint32_t dx = (x1 < x2) ? x2 - x1 : x1 - x2; // 水平方向距离 
        uint32_t dy = (y1 < y2) ? y2 - y1 : y1 - y2; // 垂直方向距离 

        if (dx >= dy) // 如果 dx >= dy，说明水平距离较大，则在 x 方向上逐步绘制
        {
            if (x2 < x1)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
                //x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            }

            // 程序从 x1 到 x2 逐步递增 x，并调用 WritePixel 绘制每个像素。
            // 变量 rem 用来记录累积的误差。当累积的误差 rem >= dx 时，次方向 
            // y 会递增或递减一次，并更新 rem -= dx 来重置误差。
            for (x = x1, y = y1; x <= x2; x++)
            {
                WritePixel(x, y, c);
                rem += dy;

                // 从 x1 开始，每次绘制(x, y) 处的像素，并增加 rem += dy。
                // 当 rem >= dx 时，说明需要调整 y 方向的坐标，此时将 y 递增或递减一次，并 rem -= dx。
                if (rem >= dx)
                {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    WritePixel(x, y, c);
                }
            }

            WritePixel(x2, y2, c);
        }
        else // 如果 dx < dy，说明垂直距离较大，则在y方向上逐步绘制
        {
            if (y2 < y1)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
                //x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            }

            // 程序从 y1 到 y2 逐步递增 y，并在(x, y) 处绘制像素。
            // 变量 rem 记录误差。当 rem >= dy 时，水平方向 x 会递增或递减一次，随后 rem -= dy 来重置误差。
            for (x = x1, y = y1; y <= y2; y++)
            {
                WritePixel(x, y, c);
                rem += dx;

                // 从 y1 开始，每次绘制(x, y) 处的像素，并增加 rem += dx。
                // 当 rem >= dy 时，调整 x 方向的坐标，递增或递减一次 x 并 rem -= dy。
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
uint32_t Device::GetTexel(float u, float v)
{
    u = u * this->max_u_;
    v = v * this->max_v_;
    int32_t x = static_cast<int32_t>(u + 0.5f);
    int32_t y = static_cast<int32_t>(v + 0.5f);
    x = Clamp<int32_t>(x, 0, this->texture_width_ - 1);
    y = Clamp<int32_t>(y, 0, this->texture_height_ - 1);
    return this->texture_[y * texture_width_ + x];
}

// 绘制扫描线
void Device::DrawScanline(scanline_t* scanline)
{
    // 根据扫描线的y，即帧缓冲像素点所在行，算出要写入的frame buffer首指针
    // 以及对应的z buffer首指针
    uint32_t* fb = this->frame_buffer_ + this->window_width_ * scanline->y;
    float* zbuffer = this->z_buffer_ + this->window_width_ * scanline->y;

    int32_t x = scanline->left_end_point_x;
    int32_t w = scanline->width;
    int32_t width = static_cast<int32_t>(this->window_width_);

    for (; w > 0; x++, w--)
    {
        if (x >= 0 && x < width) // 只绘制在屏幕内扫描线部分
        {
            float rhw = scanline->interpolated_point.rhw;

            if (rhw >= zbuffer[x]) // 比较Z缓冲区值，只有大于当前zbuffer值，即比当前像素点靠近镜头的像素点会写入到fb
            {
                float w = 1.0f / rhw;
                zbuffer[x] = rhw;

                if (render_state_ & RENDER_STATE_COLOR)
                {
                    uint32_t R = static_cast<uint32_t>(scanline->interpolated_point.color.r * w * 255.0f);
                    uint32_t G = static_cast<uint32_t>(scanline->interpolated_point.color.g * w * 255.0f);
                    uint32_t B = static_cast<uint32_t>(scanline->interpolated_point.color.b * w * 255.0f);
                    R = Clamp<uint32_t>(R, 0, 255);
                    G = Clamp<uint32_t>(G, 0, 255);
                    B = Clamp<uint32_t>(B, 0, 255);
                    fb[x] = 0xFF000000 | (R << 16) | (G << 8) | (B);
                }

                if (render_state_ & RENDER_STATE_TEXTURE)
                {
                    float u = scanline->interpolated_point.tc.u * w;
                    float v = scanline->interpolated_point.tc.v * w;
                    uint32_t cc = GetTexel(u, v);
                    fb[x] = 0xFF000000 | cc;
                }
            }
        }

        // 从左腰边的插值点开始，每循环一次递加一次【插值步】，然后进行渲染写入操作
        T3DVertexAdd(&scanline->interpolated_point, &scanline->interpolated_step);

        if (x >= width)
            break;
    }
}

// 主渲染函数，把一个梯形分解成若干条扫描线，然后绘制
// 扫描线
void Device::RenderTrapezoid(Trapezoid* trap)
{
    scanline_t scanline;
    int32_t j;
    int32_t top = static_cast<int32_t>(trap->top() + 0.5f);           // 拿到梯形的顶边和底边Y坐标
    int32_t bottom = static_cast<int32_t>(trap->bottom() + 0.5f);
    int32_t wnd_h = static_cast<int32_t>(window_height_);

    for (j = top; j < bottom; j++)
    {
        if (j >= 0 && j < wnd_h) // 不能超出屏幕
        {
            trap->CalculateEdgeInterpolatedPoint(static_cast<float>(j) + 0.5f);

            // 算出插值点的梯形，接下来要算出扫描线
            trap->InitializeScanline(&scanline, j);

            // 到了这一步，算出了每条扫描线的【插值步】数据，可以绘制每一条扫描线
            DrawScanline(&scanline);
        }

        if (j >= wnd_h)
            break;
    }
}

// 根据 render_state 绘制原始三角形
void Device::DrawPrimitive(const T3DVertex* v1, const T3DVertex* v2, const T3DVertex* v3)
{
    T3DVector4 p1, p2, p3, c1, c2, c3;
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
        T3DVertex t1 = *v1, t2 = *v2, t3 = *v3;
        std::array<Trapezoid, 2> traps;
        int n;

        t1.pos = p1;
        t2.pos = p2;
        t3.pos = p3;
        t1.pos.w = c1.w;
        t2.pos.w = c2.w;
        t3.pos.w = c3.w;

        T3DVertexRHWInit(&t1); // 重新对t1，t2，t3的纹理映射坐标和颜色值做透视除
        T3DVertexRHWInit(&t2);
        T3DVertexRHWInit(&t3); 

        // 拆分三角形为0-2个梯形，并且返回可用梯形数量
        n = Trapezoid::SplitTriangleIntoTrapezoids(traps, &t1, &t2, &t3);

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

void Device::ResetCamera(float x, float y, float z)
{
    T3DVector4 eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
    this->transform_.SetViewMatrix(&eye, &at, &up);
    this->transform_.Update();
}

void Device::InitTexture()
{
    this->texture_ = new uint32_t[texture_width_ * texture_height_];

    for (uint32_t j = 0; j < texture_height_; j++)
    {
        for (uint32_t i = 0; i < texture_width_; i++)
        {
            uint32_t x = i / 32;
            uint32_t y = j / 32;
            this->texture_[j * texture_width_ + i] = ((x + y) & 1) ? 0xFFFFFFFF : 0xFF3FBCEF;
        }
    }

    this->max_u_ = static_cast<float>(this->texture_width_ - 1);
    this->max_v_ = static_cast<float>(this->texture_height_ - 1);
}

void Device::CreateTextureFromFile(const char* file_path)
{
    // 使用 SDL_image 库加载 PNG 或 JPG 图片
    SDL_Surface* img_surface = IMG_Load(file_path);

    if ( img_surface == nullptr )
    {
        IMG_Quit();
        return;
    }

    // 对 surface 进行读写操作
    SDL_LockSurface(img_surface);  // 锁定 surface 以进行直接像素访问
    texture_width_ = static_cast<uint32_t>(img_surface->w);
    texture_height_ = static_cast<uint32_t>(img_surface->h);
    this->texture_ = new uint32_t[texture_width_ * texture_height_];

    uint32_t bytes_per_px = static_cast<uint32_t>(img_surface->format->BytesPerPixel);
    uint32_t r_shift = static_cast<uint32_t>(img_surface->format->Rshift);
    uint32_t g_shift = static_cast<uint32_t>(img_surface->format->Gshift);
    uint32_t b_shift = static_cast<uint32_t>(img_surface->format->Bshift);
    uint32_t a_shift = static_cast<uint32_t>(img_surface->format->Ashift);

    uint32_t r_mask = static_cast<uint32_t>(img_surface->format->Rmask);
    uint32_t g_mask = static_cast<uint32_t>(img_surface->format->Gmask);
    uint32_t b_mask = static_cast<uint32_t>(img_surface->format->Bmask);
    uint32_t a_mask = static_cast<uint32_t>(img_surface->format->Amask);

    if (bytes_per_px != 3 && 4 != bytes_per_px)
    {
        IMG_Quit();
        SDL_UnlockSurface(img_surface);
        SDL_FreeSurface(img_surface);
        throw Error("Only support 3 or 4 bytes per pixel image file", __FILE__, __LINE__);
        return;
    }

    if (3 == bytes_per_px)
    {
        uint8_t* pixels = reinterpret_cast<uint8_t*>(img_surface->pixels);
        uint32_t pitch = static_cast<uint32_t>(img_surface->pitch);  // 每行的字节数

        int r_idx, g_idx, b_idx;

        if (r_mask == 0x00FF0000 && g_mask == 0x0000FF00 && b_mask == 0x000000FF) // RGB 顺序
        {
            r_idx = 0;
            g_idx = 1;
            b_idx = 2;
        }
        else if (r_mask == 0x000000FF && g_mask == 0x0000FF00 && b_mask == 0x00FF0000) // BGR 顺序
        {
            r_idx = 2;
            g_idx = 1;
            b_idx = 0;
        }
        else 
        {
            SDL_FreeSurface(img_surface);
            IMG_Quit();
            throw Error("不支持的颜色格式", __FILE__, __LINE__);
        }

        for (uint32_t y = 0; y < texture_height_; ++y)
        {
            for (uint32_t x = 0; x < texture_width_; ++x)
            {
                uint8_t* pixel = pixels + y * pitch + x * 3;
                uint8_t r = pixel[r_idx];
                uint8_t g = pixel[g_idx];
                uint8_t b = pixel[b_idx];
                uint32_t a32 = static_cast<uint32_t>(255) << 24;
                uint32_t r32 = static_cast<uint32_t>(r) << 0;
                uint32_t g32 = static_cast<uint32_t>(g) << 8;
                uint32_t b32 = static_cast<uint32_t>(b) << 16;
                this->texture_[y * texture_width_ + x] = a32 | r32 | g32 | b32;
            }
        }
    }
    else  if (4 == bytes_per_px)
    {
        uint32_t* pixels = reinterpret_cast<uint32_t*>(img_surface->pixels);
        uint32_t pixel;
        uint8_t r, g, b, a;
        uint32_t r32, g32, b32, a32;

        for (uint32_t y = 0; y < texture_height_; ++y)
        {
            for (uint32_t x = 0; x < texture_width_; ++x)
            {
                pixel = pixels[y * texture_width_ + x];
                SDL_GetRGBA(pixel, img_surface->format, &r, &g, &b, &a);
                r32 = static_cast<uint32_t>(r) << 0;
                g32 = static_cast<uint32_t>(g) << 8;
                b32 = static_cast<uint32_t>(b) << 16;
                a32 = static_cast<uint32_t>(255) << 24;
                this->texture_[y * texture_width_ + x] = a32 | r32 | g32 | b32;
            }
        }
    }
    else
    {
        IMG_Quit();
        SDL_UnlockSurface(img_surface);
        SDL_FreeSurface(img_surface);
        throw Error("Only support 3 or 4 bytes per pixel image file", __FILE__, __LINE__);
        return;
    }

    IMG_Quit();
    SDL_UnlockSurface(img_surface);
    SDL_FreeSurface(img_surface);

    this->max_u_ = static_cast<float>(this->texture_width_ - 1);
    this->max_v_ = static_cast<float>(this->texture_height_ - 1);
}

void Device::DrawPlane(const T3DVertex* p1, const T3DVertex* p2, const T3DVertex* p3, const T3DVertex* p4)
{
    T3DVertex _p1 = *p1, _p2 = *p2, _p3 = *p3, _p4 = *p4;
    _p1.tc.u = 0.0f, _p1.tc.v = 0.0f;
    _p2.tc.u = 0.0f, _p2.tc.v = 1.0f;
    _p3.tc.u = 1.0f, _p3.tc.v = 1.0f;
    _p4.tc.u = 1.0f, _p4.tc.v = 0.0f;
    DrawPrimitive(&_p1, &_p2, &_p3);
    DrawPrimitive(&_p3, &_p4, &_p1);
}

void Device::DrawBox(float theta, const T3DVertex* box_vertices)
{
    T3DMatrix4X4 m;
    T3DMatrixMakeRotation(&m, -1.0f, -0.5f, 1.0f, theta);
    transform_.SetWorldMatrix(m);
    transform_.Update();
    DrawPlane(&box_vertices[0], &box_vertices[1], &box_vertices[2], &box_vertices[3]);
    DrawPlane(&box_vertices[4], &box_vertices[5], &box_vertices[6], &box_vertices[7]);
    DrawPlane(&box_vertices[0], &box_vertices[4], &box_vertices[5], &box_vertices[1]);
    DrawPlane(&box_vertices[1], &box_vertices[5], &box_vertices[6], &box_vertices[2]);
    DrawPlane(&box_vertices[2], &box_vertices[6], &box_vertices[7], &box_vertices[3]);
    DrawPlane(&box_vertices[3], &box_vertices[7], &box_vertices[4], &box_vertices[0]);
}