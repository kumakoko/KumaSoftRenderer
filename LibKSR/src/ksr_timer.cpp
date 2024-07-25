#include <thread>
#include "ksr_timer.h"

namespace KSR
{
    // 全局变量，用于计时
    std::chrono::steady_clock::time_point start_time;

    // 获取当前时间点（以毫秒为单位）
    std::chrono::milliseconds Get_Clock()
    {
        auto now_time_point = std::chrono::steady_clock::now();  // steady_clock::now()返回当前时间点。
        auto tm_since_epoch = now_time_point.time_since_epoch(); // time_since_epoch()返回当前时间点到纪元时间的时间间隔。
        // duration_cast<milliseconds>将时间间隔转换为以毫秒为单位的duration对象。
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
    }

    // 记录开始时间点
    std::chrono::milliseconds Start_Clock()
    {
        start_time = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(start_time.time_since_epoch());
    }

    // 等待指定时间
    std::chrono::milliseconds Wait_Clock(std::chrono::milliseconds count)
    {
        // 检查当前时间与开始时间的间隔是否小于指定的时间间隔count
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) < count)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 使当前线程休眠1毫秒，防止忙等待。
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
    }
}