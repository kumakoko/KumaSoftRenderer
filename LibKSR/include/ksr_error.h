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
#pragma once

#include <exception>
#include <string>
#include <vector>

namespace KSR
{
    class Error : public std::exception
    {
    public:
        /**************************************************************************************

         * @name: Error
         * @return:
         * @param: const std::wstring & message 错误消息字符串
         *************************************************************************************/
        Error(const std::wstring& message);

        Error(const std::string& message);

        Error(const char* message);

        /**************************************************************************************

         * @name: Error
         * @return:
         * @param: const std::wstring & message 错误消息字符串
         * @param: const char * file 一般传__FILE__值进去
         * @param: uint32_t line 一般传__LINE__值进去
         *************************************************************************************/
        Error(const std::wstring& message, const char* file, uint32_t line);

        Error(const std::string& message, const char* file, uint32_t line);

        Error(const char* message, const char* file, uint32_t line);

        /**************************************************************************************

         * @name: Error
         * @return:
         * @param: int SDLErrorCode
         * @param: const char * file
         * @param: uint32_t line
         *************************************************************************************/
        Error(int SDLErrorCode, const char* file, uint32_t line);

        /**************************************************************************************
         Notifies this instance.
         * @name: Notify
         * @return: void
         *************************************************************************************/
        virtual void Notify() const;


        /**************************************************************************************
         通知用户出错了,提供一个二选一对话框给用户选择
         * @name: Prompt
         * @return: void
         *************************************************************************************/
        virtual void Prompt() const;

    protected:
        /**************************************************************************************
         组装错误消息,返回错误消息字符串
         * @name: AssembleOutput
         * @return: std::wstring
         *************************************************************************************/
        virtual const std::wstring& AssembleOutput() const;

        enum EXCEPTION_TYPE
        {
            EXCEPTION_MESSAGE,  ///< 返回错误消息
            EXCEPTION_REGULAR,  ///< 返回错误消息,出错的代码文件和行
            EXCEPTION_SDL       ///< 返回SDL错误消息,出错的代码文件和行以及渲染器给的错误代码
        };

        /// <summary>
        ///错误类型
        /// </summary>
        EXCEPTION_TYPE type_;

        /// <summary>
        /// 对话框窗口标题
        /// </summary>
        std::wstring title_;

        /// <summary>
        /// 错误消息字符串
        /// </summary>
        std::wstring message_;

        /// <summary>
        /// 发生错误的代码文件名
        /// </summary>
        std::string file_;

        /// <summary>
        /// 发生错误的代码行
        /// </summary>
        uint32_t line_;
    };
}