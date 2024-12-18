﻿/*********************************************************************************************
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
#include "tiny3d_message_box.h"
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif


#if defined(WIN32) || defined(_WIN32)
void ErrorMessageBox(const std::wstring& title, const std::wstring& content)
{
    ::ShowCursor(true);

    int ret = ::MessageBoxW(nullptr, content.c_str(), title.c_str(), MB_YESNO | MB_ICONERROR);

    if (ret == IDYES)
    {
        ret = ::MessageBoxW(nullptr, L"Terminate now (exit(1)) ?", L"Eject! Eject!", MB_YESNO | MB_ICONQUESTION);

        if (ret == IDYES)
        {
            exit(1);
        }
        else
        {
            ::PostQuitMessage(0);
        }
    }
}
#endif