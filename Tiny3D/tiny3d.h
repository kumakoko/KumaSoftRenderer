#include <cstdint>
#include "SDL.h"

extern uint8_t* g_BackBuffer;    // 后台页面的首指针
extern int g_BackSurfacePitch;
extern SDL_Surface* g_BackSurface;
extern SDL_Surface* g_ScreenSurface;
extern SDL_Window* g_Window;
extern uint32_t g_WindowRenderAreaWidth;
extern uint32_t g_WindowRenderAreaHeight;
extern const char* g_WindowTitle;
extern bool g_IsRunning;
extern bool g_IsMouseDragging;
extern int g_MouseWndOffsetX;
extern int g_MouseWndOffsetY;

/**************************************************************************************
初始化SDL库
@name: InitializeGraphicSystem
@return: void
*************************************************************************************/
void InitializeGraphicSystem();

void ShutdownGraphicSystem();

void LockBackSurface();

void UnlockBackSurface();

void ProcessInput(SDL_Event& event);

void OnKeyDown(SDL_Event& event);

void OnKeyUp(SDL_Event& event);

// 处理鼠标拖动事件
void OnMouseDragging(SDL_Window* window, SDL_Event& event);

void RenderScene();