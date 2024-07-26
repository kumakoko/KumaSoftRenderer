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

#include <chrono>

namespace KSR
{
    // 全局变量start_time，它的类型是steady_clock::time_point，用于记录开始时间点。
    // steady_clock是chrono库中的一种时钟类型，适用于测量时间间隔，因为它不会受到系
    // 统时间变化的影响。
    extern std::chrono::steady_clock::time_point start_time;

    /**************************************************************************************
    获取当前时间点（以毫秒为单位）
    @name: KSR::Get_Clock
    @return: std::chrono::milliseconds 返回自纪元以来的时间间隔，以毫秒为单位。
    *************************************************************************************/
    std::chrono::milliseconds Get_Clock();

    /**************************************************************************************
    记录当前时间点到全局变量start_time，并返回自纪元以来的时间间隔，以毫秒为单位。
    @name: KSR::Start_Clock
    @return: std::chrono::milliseconds 返回自纪元以来的时间间隔，以毫秒为单位。
    *************************************************************************************/
    std::chrono::milliseconds Start_Clock();

    /**************************************************************************************
    等待指定的时间间隔。
    @name: KSR::Wait_Clock
    @return: std::chrono::milliseconds
    @param: std::chrono::milliseconds count
    *************************************************************************************/
    std::chrono::milliseconds Wait_Clock(std::chrono::milliseconds count);
}