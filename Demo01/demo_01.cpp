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
#include <string>
#include <cstdint>
#include <exception>

#include "demo_01_app.h"
#include "ksr_string_convertor.h"
#include "ksr_message_box.h"
#include "ksr_error.h"

int main(int argc, char* argv[])
{
    Demo01App* app = nullptr;

    try
    {
        const uint32_t screen_width = 1024;
        const uint32_t screen_height = 768;
        const char* title = "[Kuma Soft Renderer] : 01-Triangle";
        app = new Demo01App();
        app->InitRenderer(screen_width, screen_height, title);
        app->InitScene();
        app->Run();
    }
    catch (KSR::Error e)
    {
        e.Notify();
    }
    catch (std::exception e)
    {
        std::wstring exception_desc;
        KSR::StringConvertor::ANSItoUTF16LE(e.what(), exception_desc);
        KSR::ErrorMessageBox(std::wstring(L"[Kuma Soft Renderer] : 01-Triangle : Unhandled Exception, aborting"), exception_desc);
    }

    delete app;
    return 0;
}