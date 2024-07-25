#pragma once

#include <cstdint>
#include "SDL.h"

namespace KSR
{
    extern uint8_t* primary_buffer; // ��ҳ����ָ��
    extern uint8_t* back_buffer;    // ��̨ҳ�����ָ��
    extern int back_lpitch;

    extern SDL_Surface* lpddsprimary;
    extern SDL_Surface* lpddsback;
    extern SDL_Surface* gScreenSurface;
    extern SDL_Window* gWindow;// gWindow = SDL_CreateWindow("SDL Flip Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    extern uint32_t SCREEN_WIDTH;
    extern uint32_t SCREEN_HEIGHT;

    // ��ʼ��ͼ��ϵͳ
    void InitializeGraphicSystem(uint32_t sw,uint32_t sh, const char* wn);

    // �ر�ͼ��ϵͳ
    void ShutdownGraphicSystem();

    void DDraw_Fill_Surface(SDL_Surface* lpdds, uint32_t color, SDL_Rect* client = nullptr);

    uint8_t* DDraw_Lock_Back_Surface();

    void DDraw_Unlock_Back_Surface();

    void DDraw_Flip();
}