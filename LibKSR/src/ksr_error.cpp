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
#include <sstream>
#include <SDL.h>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#include "ksr_error.h"
#include "ksr_message_box.h"
#include "ksr_string_convertor.h"

namespace KSR
{
    Error::Error(const std::wstring& message): 
        title_(L"Kuma Soft Renderer Exception"), 
        type_(EXCEPTION_MESSAGE), 
        message_(message)
    {
    }

    Error::Error(const std::wstring& message, const char* file, uint32_t line):
        title_(L"Kuma Soft Renderer Exception"),
        type_(EXCEPTION_REGULAR),
        message_(message),
        file_(file),
        line_(line) 
    {
        std::wstring file_name;
        StringConvertor::ANSItoUTF16LE(file_.c_str(), file_name);
        std::wstringstream wss;
        wss << message << L"\n" << file_name << L"\nLine " << line_;
        message_ = wss.str();
    }

    Error::Error(int SDLErrorCode, const char* file, uint32_t line) :
        title_(L"Kuma Soft Renderer Exception"),
        type_(EXCEPTION_REGULAR),
        file_(file),
        line_(line)
    {
        std::wstring file_name;
        std::wstring error_desc;
        StringConvertor::ANSItoUTF16LE(file_.c_str(), file_name);
        StringConvertor::ANSItoUTF16LE(SDL_GetError(), error_desc);
        std::wstringstream wss;
        wss << message_ << L"\n" << file_name << L"\nLine " << line_;
        wss << L"\SDL Error Code: " << SDLErrorCode;
        wss << L"\nSDL Error Desc: " << error_desc;
        message_ = wss.str();
    }

    const std::wstring& Error::AssembleOutput() const
    {
        return message_;
    }

    void Error::Prompt() const 
    {
        std::wstring output;
        output.append(this->AssembleOutput()).append(L"\nQuit Program?");
        MessageBox(title_, output);
    }

    void Error::Notify() const 
    {
#if defined(WIN32) || defined(_WIN32)
        ::ShowCursor(true);
#endif
        MessageBox(title_, AssembleOutput());
    }
}
