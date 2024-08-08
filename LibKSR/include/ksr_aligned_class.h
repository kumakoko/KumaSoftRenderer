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
// 提供align new接口的模板基类，本基类提供了operator new 和operator delete两个接口，
// 此两个接口都基于align allocate操作进行字节对齐分配
#pragma once

#include <cstdlib>
#include <new>

namespace KSR
{
    template<typename T>
    class AlignedClass
    {
    public:
        void* operator new(std::size_t size)
        {
            void* ptr = nullptr;
            std::size_t align_size = alignof(T);
#if defined(WIN32) || defined(_WIN32)
            ptr = _aligned_malloc(size, align_size);
#else
            void* ptr = std::aligned_alloc(align_size, size);
#endif
            if (!ptr)
            {
                throw std::bad_alloc();
            }

            return ptr;
        }

        void operator delete(void* ptr) noexcept
        {
#if defined(WIN32) || defined(_WIN32)
            _aligned_free(ptr);
#else
            std::free(ptr);
#endif
        }
    };
}
