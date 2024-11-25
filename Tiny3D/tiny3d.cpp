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

#include "fmt/format.h"

#include "tiny3d_app.h"
#include "tiny3d_string_convertor.h"
#include "tiny3d_message_box.h"
#include "tiny3d_error.h"

int main(int argc, char* argv[])
{
    Tiny3DApp* app = nullptr;

    try
    {
        app = new Tiny3DApp();
        app->InitializeGraphicSystem();
        app->InitRenderDevice();
        app->Run();
    }
    catch (Error e)
    {
        e.Notify();
    }
    catch (std::exception e)
    {
        std::wstring exception_desc;
        StringConvertor::ANSItoUTF16LE(e.what(), exception_desc);
        ErrorMessageBox(std::wstring(L" : Unhandled Exception, aborting"), exception_desc);
    }

    app->DestroyRenderDevice();
    app->ShutdownGraphicSystem();
    delete[] app;
    return 0;
}