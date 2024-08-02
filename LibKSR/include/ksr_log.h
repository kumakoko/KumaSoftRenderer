#pragma once

namespace KSR
{
    class Log
    {
    public:
        static void Info(const char* string, ...);
        static void Warn(const char* string, ...);
        static void Error(const char* string, ...);
    };
}
