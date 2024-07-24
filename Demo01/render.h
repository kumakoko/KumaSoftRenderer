#pragma once

#include "ksr_vector.h"
#include "ksr_camera.h"
#include "ksr_render_list.h"
#include "ksr_polygon.h"

using namespace KSR;

extern POINT4D          cam_pos;    // 摄像机的初始位置和朝向initialize camera position and direction
extern VECTOR4D         cam_dir;    // 摄像机的初始位置和朝向initialize camera position and direction
extern CAM4DV1          cam;        // 主摄像机
extern RENDERLIST4DV1   rend_list;  // 唯一的渲染列表
extern POLYF4DV1        poly1;      // 唯一的待渲染的三角形our lonely polygon
extern POINT4D          poly1_pos;  // 唯一的待渲染的三角形的世界坐标world position of polygon

extern const std::uint32_t WINDOW_WIDTH;
extern const std::uint32_t WINDOW_HEIGHT;

int Game_Init(void* parms);

int Game_Main(void* parms);