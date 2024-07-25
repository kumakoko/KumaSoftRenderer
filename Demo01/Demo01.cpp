#include "SDL.h"
#include "ksr_vector.h"
#include "ksr_camera.h"
#include "ksr_render_list.h"
#include "ksr_polygon.h"
#include "ksr_surface.h"
#include "ksr_shape_drawing.h"
#include "ksr_color.h"
#include <windows.h>

using namespace KSR;

KSR::POINT4D          cam_pos = { 0.0f,0.0f,-100.0f,1.0f }; // 摄像机的初始位置
KSR::VECTOR4D         cam_dir = { 0.0f,0.0f,0.0f,1.0f };    // 摄像机的初始朝向
KSR::CAM4DV1          cam;                                  // 主摄像机
KSR::RENDERLIST4DV1   rend_list;                            // 唯一的渲染列表
KSR::POLYF4DV1        poly1;                                // 唯一的待渲染的三角形our lonely polygon
KSR::POINT4D          poly1_pos = { 0.0f,0.0f,100.0f,1.0f };// 唯一的待渲染的三角形的世界坐标world position of polygon

static uint32_t screen_width = 640;
static uint32_t screen_height = 480;

void processInput(SDL_Event& event, bool& running);

void handleMouseDrag(SDL_Window* window, SDL_Event& event);

static void _InitSinglePolygon();

int main(int argc, char* argv[])
{
    KSR::InitializeGraphicSystem(640, 480, "Demo01");

    _InitSinglePolygon();

    // initialize the camera with 90 FOV, normalized coordinates
    KSR::Init_CAM4DV1(&cam,      // the camera object
        KSR::CAM_MODEL_EULER, // euler camera model
        &cam_pos,  // initial camera position
        &cam_dir,  // initial camera angles
        nullptr,      // no initial target
        50.0f,      // near and far clipping planes
        500.0f,
        90.0f,      // field of view in degrees
        screen_width,   // size of final screen viewport
        screen_height);

    // 主循环
    bool running = true;

    static KSR::MATRIX4X4 mrot; // general rotation matrix
    static float ang_y = 0;      // rotation angle

    int index; // looping var

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            processInput(event, running);
            handleMouseDrag(gWindow, event);
        }

        KSR::DDraw_Fill_Surface(KSR::lpddsback, 0);
        KSR::DDraw_Fill_Surface(KSR::lpddsprimary, 0);

        // initialize the renderlist
        KSR::Reset_RENDERLIST4DV1(&rend_list);

        // insert polygon into the renderlist
        KSR::Insert_POLYF4DV1_RENDERLIST4DV1(&rend_list, &poly1);

        // generate rotation matrix around y axis
        KSR::Build_XYZ_Rotation_MATRIX4X4(0, ang_y, 0, &mrot);

        // rotate polygon slowly
        
        if (++ang_y >= 360.0f)
            ang_y = 0.0f;
        

        // rotate the local coords of single polygon in renderlist
        KSR::Transform_RENDERLIST4DV1(&rend_list, &mrot, KSR::TRANSFORM_LOCAL_ONLY);

        // perform local/model to world transform
        KSR::Model_To_World_RENDERLIST4DV1(&rend_list, &poly1_pos);

        // generate camera matrix
        KSR::Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

        // apply world to camera transform
        KSR::World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

        // apply camera to perspective transformation
        KSR::Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

        // apply screen transform
        KSR::Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

        // lock the back buffer
        KSR::DDraw_Lock_Back_Surface();

        // render the polygon list
        KSR::Draw_RENDERLIST4DV1_Wire16(&rend_list, back_buffer, back_lpitch);

        //     KSR::Draw_Rectangle(0,0,300,300, 0xFFFFFFFF, KSR::lpddsback);

             // unlock the back buffer
        KSR::DDraw_Unlock_Back_Surface();

        // flip the surfaces
        KSR::DDraw_Flip();

        // Delay to simulate frame time
        //SDL_Delay(100);
        Sleep(30);
    }

    KSR::ShutdownGraphicSystem();
    return 0;
}

// 处理输入事件
void processInput(SDL_Event& event, bool& running)
{
    if (event.type == SDL_QUIT)
    {
        running = false;
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
    }
}

// 处理鼠标拖动事件
void handleMouseDrag(SDL_Window* window, SDL_Event& event)
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
static void _InitSinglePolygon()
{
    poly1.state = POLY4DV1_STATE_ACTIVE;
    poly1.attr = 0;
    poly1.color = 0xFFFF;//RGB16Bit(0, 255, 0);

    poly1.vlist[0].x = 0.0f;
    poly1.vlist[0].y = 50.0f;
    poly1.vlist[0].z = 0.0f;
    poly1.vlist[0].w = 1.0f;

    poly1.vlist[1].x = 50.0f;
    poly1.vlist[1].y = -50.0;
    poly1.vlist[1].z = 0.0f;
    poly1.vlist[1].w = 1.0;

    poly1.vlist[2].x = -50.0f;
    poly1.vlist[2].y = -50.0f;
    poly1.vlist[2].z = 0.0f;
    poly1.vlist[2].w = 1.0f;

    poly1.next = poly1.prev = nullptr;
}
