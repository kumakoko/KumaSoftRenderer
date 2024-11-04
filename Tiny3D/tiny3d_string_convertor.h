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

#include <string>


class StringConvertor
{
private:
    /// <summary>
    /// The k temporary buffer length
    /// </summary>
    static const size_t kTmpBufferLen = 16;

    /// <summary>
    /// The s_char_buffer_
    /// </summary>
    static char s_char_buffer_[kTmpBufferLen];

    /// <summary>
    /// The s_wchar_buffer_
    /// </summary>
    static char s_wchar_buffer_[kTmpBufferLen];

    /// <summary>
    /// ANSI字符集,将会根据当前系统的代码页去初始化对应的值
    /// </summary>
    static std::string s_ansi_char_set;

private:
    /**************************************************************************************

     * @name: CodeConvert
     * @return: int
     * @param: const char * from_charset
     * @param: const char * to_charset
     * @param: const char * inbuf
     * @param: size_t inlen
     * @param: char * outbuf
     * @param: size_t * outlen
     *************************************************************************************/
    static int CodeConvert(const char* from_charset, const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t* outlen);
public:
    /**************************************************************************************
     把UTF8编码的字符串作为源数据,返回的得到的utf16字符串首指针
     * @name: UTF8toUTF16LE
     * @return: int
     * @param: const char * utf8_str 要转换的源数据字符串,utf8编码
     * @param: wchar_t * * utf16_str
     *************************************************************************************/
    static int UTF8toUTF16LE(const char* utf8_str, wchar_t** utf16_str);

    /**************************************************************************************

     * @name: UTF8toUTF16LE
     * @return: int
     * @param: const char * utf8_str
     * @param: std::wstring & utf16_str
     *************************************************************************************/
    static int UTF8toUTF16LE(const char* utf8_str, std::wstring& utf16_str);

    /**************************************************************************************

     * @name: UTF8toUTF16LE
     * @return: std::wstring
     * @param: const char * utf8_str
     *************************************************************************************/
    static std::wstring UTF8toUTF16LE(const char* utf8_str);

    /**************************************************************************************

     * @name: UTF16LEtoUTF8
     * @return: int
     * @param: const wchar_t * utf16_str
     * @param: char * * utf8_str
     *************************************************************************************/
    static int UTF16LEtoUTF8(const wchar_t* utf16_str, char** utf8_str);

    /**************************************************************************************

     * @name: UTF16LEtoUTF8
     * @return: int
     * @param: const wchar_t * utf16_str
     * @param: std::string & utf8_str
     *************************************************************************************/
    static int UTF16LEtoUTF8(const wchar_t* utf16_str, std::string& utf8_str);

    /**************************************************************************************

     * @name: UTF16LEtoUTF8
     * @return: std::string
     * @param: const wchar_t * utf16_str
     *************************************************************************************/
    static std::string UTF16LEtoUTF8(const wchar_t* utf16_str);

    /**************************************************************************************

     * @name: UTF8toGBK
     * @return: int
     * @param: const char * utf8_str
     * @param: char * * gbk_str
     *************************************************************************************/
    static int UTF8toGBK(const char* utf8_str, char** gbk_str);

    /**************************************************************************************

     * @name: UTF8toGBK
     * @return: int
     * @param: const char * utf8_str
     * @param: std::string & gbk_str
     *************************************************************************************/
    static int UTF8toGBK(const char* utf8_str, std::string& gbk_str);

    /**************************************************************************************

     * @name: GBKtoUTF8
     * @return: int
     * @param: const char * gbk_str
     * @param: char * * utf8_str
     *************************************************************************************/
    static int GBKtoUTF8(const char* gbk_str, char** utf8_str);

    /**************************************************************************************

     * @name: GBKtoUTF8
     * @return: int
     * @param: const char * gbk_str
     * @param: std::string & utf8_str
     *************************************************************************************/
    static int GBKtoUTF8(const char* gbk_str, std::string& utf8_str);

    /**************************************************************************************

     * @name: ANSItoUTF16LE
     * @return: int
     * @param: const char * ansi_str
     * @param: std::wstring & utf16_str
     *************************************************************************************/
    static int ANSItoUTF16LE(const char* ansi_str, std::wstring& utf16_str);

    /**************************************************************************************

     * @name: ANSItoUTF16LE
     * @return: std::wstring
     * @param: const char * ansi_str
     *************************************************************************************/
    static std::wstring ANSItoUTF16LE(const char* ansi_str);

    /**************************************************************************************

     * @name: UTF16LEtoANSI
     * @return: int
     * @param: const wchar_t * utf16_str
     * @param: std::string & ansi_str
     *************************************************************************************/
    static int UTF16LEtoANSI(const wchar_t* utf16_str, std::string& ansi_str);

    /**************************************************************************************

     * @name: InitCodePageInfo
     * @return: void
     *************************************************************************************/
    static void InitCodePageInfo();
};