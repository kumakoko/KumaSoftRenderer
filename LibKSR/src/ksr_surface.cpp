/*********************************************************************************************
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

#include <utility>
#include "ksr_surface.h"
#include "ksr_color.h"
#include "ksr_shape_drawing.h"
#include "ksr_constants.h"

// https://stackoverflow.com/questions/28341281/how-can-i-do-double-buffering-in-sdl-2-0
/*SDL_RenderClear will clear the current rendering buffer.
SDL_RenderCopy will render textures to the current rendering buffer.
SDL_RenderPresent will swap the buffers, showing everything that was rendered using SDL_RenderCopy since the last SDL_RenderClear call.*/
namespace KSR
{
    int32_t dd_pixel_format = DD_PIXEL_FORMAT565;
    uint8_t* primary_buffer = nullptr; // 主页面首指针
    uint8_t* back_buffer = nullptr;    // 后台页面的首指针
    int back_lpitch = 0;
    SDL_Surface* lpddsprimary = nullptr;
    SDL_Surface* lpddsback = nullptr;
    SDL_Surface* gScreenSurface = nullptr;
    SDL_Window* gWindow = nullptr;
    uint32_t WINDOW_RENDER_AREA_WIDTH = 640;
    uint32_t WINDOW_RENDER_AREA_HEIGHT = 480;

    void InitializeGraphicSystem(uint32_t sw, uint32_t sh, const char* wn)
    {
        WINDOW_RENDER_AREA_WIDTH = sw;
        WINDOW_RENDER_AREA_HEIGHT = sh;
        max_clip_x = WINDOW_RENDER_AREA_WIDTH - 1;
        max_clip_y = WINDOW_RENDER_AREA_HEIGHT - 1;

        gWindow = SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, SDL_WINDOW_SHOWN);

        gScreenSurface = SDL_GetWindowSurface(gWindow);
        if (gScreenSurface == nullptr) {
            // throw 异常
             // printf("Window surface could not be retrieved! SDL_Error: %s\n", SDL_GetError());
            // return false;
        }

        //lpddsprimary = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        uint32_t rmask, gmask, bmask, amask;
        Get16BitSufaceRGBMask(rmask, gmask, bmask, amask);
        lpddsprimary = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, 16, rmask, gmask, bmask, amask);

        if (lpddsprimary == nullptr) {
            //printf("Surfaces could not be created! SDL_Error: %s\n", SDL_GetError());
           // return false;
        }

        lpddsback = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, WINDOW_RENDER_AREA_HEIGHT, 16, rmask, gmask, bmask, amask);
        //lpddsback = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        
        if (lpddsback == nullptr) {
            //  printf("Surfaces could not be created! SDL_Error: %s\n", SDL_GetError());
            //  return false;
        }
    }

    void ShutdownGraphicSystem()
    {
        SDL_FreeSurface(lpddsprimary);
        SDL_FreeSurface(lpddsback);
        SDL_DestroyWindow(gWindow);
        lpddsprimary = nullptr;
        lpddsback = nullptr;
        gWindow = nullptr;
        SDL_Quit();
    }


    void DDraw_Fill_Surface(SDL_Surface* lpdds, uint32_t color, SDL_Rect* client/* = nullptr*/)
    {
        SDL_FillRect(lpdds, client, color);
    }

    uint8_t* DDraw_Lock_Back_Surface()
    {
        // this function locks the secondary back surface and returns a pointer to it
        // and updates the global variables secondary buffer, and back_lpitch

        // is this surface already locked
        if (back_buffer)
        {
            // return to current lock
            return(back_buffer);
        }


        //锁定surface
        int ret = SDL_LockSurface(lpddsback);

        if (0 != ret)
        {
            // 在这里抛出异常
        }

        back_buffer = reinterpret_cast<uint8_t*>(lpddsback->pixels);
        back_lpitch = lpddsback->pitch;
        return back_buffer;
    }

    void DDraw_Unlock_Back_Surface()
    {
        // this unlocks the secondary

        // is this surface valid
        if (nullptr == back_buffer)
            return;

        // unlock the secondary surface
        //lpddsback->Unlock(nullptr);
        SDL_UnlockSurface(lpddsback);

        // reset the secondary surface
        back_buffer = nullptr;
        back_lpitch = 0;
    }

    void DDraw_Flip()
    {
        // Copy the contents of the back buffer to the front buffer
   //     SDL_BlitSurface(lpddsback, nullptr, lpddsprimary, nullptr);

        // Copy the front buffer to the screen surface
       // SDL_BlitSurface(lpddsprimary, nullptr, gScreenSurface, nullptr);

        // Copy the contents of the back buffer to the front buffer
        SDL_BlitSurface(lpddsback, nullptr, gScreenSurface, nullptr);

        // Copy the front buffer to the screen surface
       // SDL_BlitSurface(lpddsprimary, nullptr, gScreenSurface, nullptr);

        // Update the window surface
        SDL_UpdateWindowSurface(gWindow);

        // Swap the buffers
        /*SDL_Surface* temp = lpddsprimary;
        lpddsprimary = lpddsback;
        lpddsback = temp;*/
    //    std::swap(lpddsback, lpddsprimary);

        back_buffer = nullptr;
        back_lpitch = 0; // 因为交换过，所以这些都需要置空
        primary_buffer = nullptr;


    }
}

// 基于surface实现的double buffering
//#include <SDL2/SDL.h>
//#include <stdio.h>
//
//// Screen dimensions
//const int WINDOW_RENDER_AREA_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
//
//bool init();
//bool loadMedia();
//void close();
//
//SDL_Window* gWindow = nullptr;
//SDL_Surface* gScreenSurface = nullptr;
//SDL_Surface* frontBuffer = nullptr;
//SDL_Surface* backBuffer = nullptr;
//
//bool init() {
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//        return false;
//    }
//
//    gWindow = SDL_CreateWindow("SDL Flip Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//    if (gWindow == nullptr) {
//        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//        return false;
//    }
//
//    gScreenSurface = SDL_GetWindowSurface(gWindow);
//    if (gScreenSurface == nullptr) {
//        printf("Window surface could not be retrieved! SDL_Error: %s\n", SDL_GetError());
//        return false;
//    }
//
//    frontBuffer = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
//    backBuffer = SDL_CreateRGBSurface(0, WINDOW_RENDER_AREA_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
//    if (frontBuffer == nullptr || backBuffer == nullptr) {
//        printf("Surfaces could not be created! SDL_Error: %s\n", SDL_GetError());
//        return false;
//    }
//
//    return true;
//}
//
//bool loadMedia() {
//    // Load media here (if needed)
//    return true;
//}
//
//void close() {
//    SDL_FreeSurface(frontBuffer);
//    SDL_FreeSurface(backBuffer);
//    SDL_DestroyWindow(gWindow);
//    frontBuffer = nullptr;
//    backBuffer = nullptr;
//    gWindow = nullptr;
//
//    SDL_Quit();
//}
//
//void flip() {
//    // Copy the contents of the back buffer to the front buffer
//    SDL_BlitSurface(backBuffer, nullptr, frontBuffer, nullptr);
//
//    // Copy the front buffer to the screen surface
//    SDL_BlitSurface(frontBuffer, nullptr, gScreenSurface, nullptr);
//
//    // Update the window surface
//    SDL_UpdateWindowSurface(gWindow);
//
//    // Swap the buffers
//    SDL_Surface* temp = frontBuffer;
//    frontBuffer = backBuffer;
//    backBuffer = temp;
//}
//
//int main(int argc, char* args[]) {
//    if (!init()) {
//        printf("Failed to initialize!\n");
//    }
//    else {
//        if (!loadMedia()) {
//            printf("Failed to load media!\n");
//        }
//        else {
//            bool quit = false;
//            SDL_Event e;
//
//            while (!quit) {
//                while (SDL_PollEvent(&e) != 0) {
//                    if (e.type == SDL_QUIT) {
//                        quit = true;
//                    }
//                }
//
//                // Render something to the back buffer
//                SDL_FillRect(backBuffer, nullptr, SDL_MapRGB(backBuffer->format, 0xFF, 0x00, 0x00)); // Red color
//
//                // Perform flip operation
//                flip();
//
//                // Delay to simulate frame time
//                SDL_Delay(100);
//            }
//        }
//    }
//
//    close();
//    return 0;
//}
