#include <chrono>
#include "SDL.h"
#include "ksr_vector.h"
#include "ksr_camera.h"
#include "ksr_render_list.h"
#include "ksr_polygon.h"
#include "ksr_surface.h"
#include "ksr_shape_drawing.h"
#include "ksr_color.h"
#include "ksr_timer.h"

static KSR::POINT4D          s_CameraPosition = { 0.0f,0.0f,-100.0f,1.0f }; // 摄像机的初始位置
static KSR::VECTOR4D         s_CameraDirection = { 0.0f,0.0f,0.0f,1.0f };    // 摄像机的初始朝向
static KSR::CAM4DV1          s_Camera;                                  // 主摄像机
static KSR::RENDERLIST4DV1   s_RenderList;                            // 唯一的渲染列表
static KSR::POLYF4DV1        s_SinglePolygon;                                // 唯一的待渲染的三角形our lonely polygon
static KSR::POINT4D          s_SinglePolygonPosition = { 0.0f,0.0f,100.0f,1.0f };// 唯一的待渲染的三角形的世界坐标world position of polygon
static uint32_t              s_ScreenWidth = 1024;
static uint32_t              s_ScreenHeight = 768;
static KSR::MATRIX4X4        s_RotationMatrix; // general rotation matrix
static float                 s_RotationAngleY = 0;      // rotation angle
static bool                  s_IsRunning = true;

static void ProcessInput(SDL_Event& event, bool& s_IsRunning);

static void HandleMouseDrag(SDL_Window* window, SDL_Event& event);

static void InitSinglePolygon();
/*
int main(int argc, char* argv[])
{
    KSR::InitializeGraphicSystem(s_ScreenWidth, s_ScreenHeight, "[Kuma Soft Renderer] : 01-Triangle");

    InitSinglePolygon();

    KSR::Init_CAM4DV1(&s_Camera, KSR::CAM_MODEL_EULER, &s_CameraPosition, &s_CameraDirection, nullptr,
        50.0f, 500.0f, 90.0f, static_cast<float>(s_ScreenWidth), static_cast<float>(s_ScreenHeight));

    while (s_IsRunning)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            ProcessInput(event, s_IsRunning);
            HandleMouseDrag(KSR::gWindow, event);
        }

        KSR::Start_Clock();
        KSR::DDraw_Fill_Surface(KSR::lpddsback, 0);
        KSR::DDraw_Fill_Surface(KSR::gScreenSurface, 0);

        // 初始化render list，即清空render list
        KSR::Reset_RENDERLIST4DV1(&s_RenderList);

        // insert polygon into the renderlist
        KSR::Insert_POLYF4DV1_RENDERLIST4DV1(&s_RenderList, &s_SinglePolygon);

        // generate rotation matrix around y axis
        KSR::Build_XYZ_Rotation_MATRIX4X4(0, s_RotationAngleY, 0, &s_RotationMatrix);

        // rotate polygon slowly
        s_RotationAngleY += 0.1f;

        if (s_RotationAngleY >= 360.0f)
            s_RotationAngleY = 0.0f;

        // 把render list中的多边形的顶点坐标，基于其自身的局部坐标系，进行绕Y轴旋转
        KSR::Transform_RENDERLIST4DV1(&s_RenderList, &s_RotationMatrix, KSR::TRANSFORM_LOCAL_ONLY);

        // 把多边形坐标，从模型空间，变换到世界空间
        KSR::Model_To_World_RENDERLIST4DV1(&s_RenderList, &s_SinglePolygonPosition);

        // 构建view matrix，即把顶点从世界空间变换到观察空间的矩阵
        KSR::Build_CAM4DV1_Matrix_Euler(&s_Camera, KSR::CAM_ROT_SEQ_ZYX);

        // 把多边形坐标，从世界空间，变换到观察空间
        KSR::World_To_Camera_RENDERLIST4DV1(&s_RenderList, &s_Camera);

        // 把多边形坐标从观察空间，变换到透视投影空间
        KSR::Camera_To_Perspective_RENDERLIST4DV1(&s_RenderList, &s_Camera);

        // apply screen transform
        KSR::Perspective_To_Screen_RENDERLIST4DV1(&s_RenderList, &s_Camera);

        // lock the back buffer
        KSR::DDraw_Lock_Back_Surface();

        // render the polygon list
        KSR::Draw_RENDERLIST4DV1_Wire16(&s_RenderList, KSR::back_buffer, KSR::back_lpitch);

        // unlock the back buffer
        KSR::DDraw_Unlock_Back_Surface();

        // flip the surfaces
        KSR::DDraw_Flip();

        KSR::Wait_Clock(std::chrono::milliseconds(33));
    }

    KSR::ShutdownGraphicSystem();
    return 0;
}
*/

// 处理输入事件
void ProcessInput(SDL_Event& event, bool& s_IsRunning)
{
    if (event.type == SDL_QUIT)
    {
        s_IsRunning = false;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            s_IsRunning = false;
        }
    }
}

// 处理鼠标拖动事件
void HandleMouseDrag(SDL_Window* window, SDL_Event& event)
{
    static bool dragging = false;
    static int offsetX, offsetY;

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        dragging = true;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int windowX, windowY;
        SDL_GetWindowPosition(window, &windowX, &windowY);
        offsetX = mouseX - windowX;
        offsetY = mouseY - windowY;
    }
    else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
        dragging = false;
    }
    else if (event.type == SDL_MOUSEMOTION && dragging)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_SetWindowPosition(window, mouseX - offsetX, mouseY - offsetY);
    }
}

// 初始化这个唯一的待渲染三角形
static void InitSinglePolygon()
{
    s_SinglePolygon.state = KSR::POLY4DV1_STATE_ACTIVE;
    s_SinglePolygon.attr = 0;
    s_SinglePolygon.color = 0xFFFF;//RGB16Bit(0, 255, 0);

    s_SinglePolygon.vlist[0].x = 0.0f;
    s_SinglePolygon.vlist[0].y = 50.0f;
    s_SinglePolygon.vlist[0].z = 0.0f;
    s_SinglePolygon.vlist[0].w = 1.0f;

    s_SinglePolygon.vlist[1].x = 50.0f;
    s_SinglePolygon.vlist[1].y = -50.0;
    s_SinglePolygon.vlist[1].z = 0.0f;
    s_SinglePolygon.vlist[1].w = 1.0;

    s_SinglePolygon.vlist[2].x = -50.0f;
    s_SinglePolygon.vlist[2].y = -50.0f;
    s_SinglePolygon.vlist[2].z = 0.0f;
    s_SinglePolygon.vlist[2].w = 1.0f;

    s_SinglePolygon.next = s_SinglePolygon.prev = nullptr;
}
