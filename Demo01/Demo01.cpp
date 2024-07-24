//#include "SDL.h"
//#include <stdio.h>
//
//SDL_Window* window = NULL;
//SDL_Surface* surface = NULL;
//#define WINDOW_WIDTH 1000
//#define WINDOW_HEIGHT 800
//
//void init()
//{
//    SDL_Init(SDL_INIT_VIDEO);
//    window = SDL_CreateWindow("SDL",
//        100, // x
//        100, // y
//        WINDOW_WIDTH, // w
//        WINDOW_HEIGHT, // h
//        SDL_WINDOW_FULLSCREEN);
//    surface = SDL_GetWindowSurface(window);
//}
//
//
//int main(int argc, const char* argv[])
//{
//    init();
//
//    //锁定surface
//    SDL_LockSurface(surface);
//
//    //生成一种颜色
//    Uint32 color = SDL_MapRGB(surface->format, 0xff, 0, 0);
//
//    //获取surface像素点集操作地址
//    Uint32* pixel = (Uint32*)surface->pixels;
//
//
//    int i;
//    //一个像素4个字节,要除以4
//    //for(i = 0; i < surface->pitch / 4 * 100; i++)
//    for (i = 100 * 640; i < 101 * 640; i++)
//    {
//        pixel[i] = color;
//    }
//    pixel[120 * 640 + 120] = color;
//    printf("i = %d\n", i);
//
//    SDL_UnlockSurface(surface);
//    //显示
//    SDL_UpdateWindowSurface(window);
//
//    SDL_Delay(5 * 1000);
//
//    SDL_FreeSurface(surface);
//    SDL_Quit();
//
//
//
//
//    return 0;
//}

#include <SDL.h>
#include <iostream>

// 函数声明
void processInput(SDL_Event& event, bool& running);
void handleMouseDrag(SDL_Window* window, SDL_Event& event);

int main(int argc, char* argv[]) 
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    
    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == nullptr) 
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 主循环
    bool running = true;
    
    while (running)
    {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) 
        {
            processInput(event, running);
            handleMouseDrag(window, event);
        }

        // 设置背景颜色为黑色
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 交换缓冲区
        SDL_RenderPresent(renderer);
    }

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
