// 使用内嵌汇编代码优化的各种函数模块
#pragma once

#include <cstdint>

namespace KSR
{
    // 这个函数的作用是将指定的32位数据 data 重复填充到 dest 指向的内存区域，填充的次数为 count 次。
    inline void Mem_Set_WORD(void* dest, std::uint16_t data, std::int32_t count)
    {
        _asm
        {
            mov edi, dest  // 将目标内存地址 dest 存入 edi 寄存器，edi 是目标内存地址指针
            mov ecx, count // 将需要填充的16位字的数量 count 存入 ecx 寄存器，ecx 是计数器寄存器。
            mov ax, data   // 将16位数据 data 存入 ax 寄存器，ax 是源数据寄存器。
            rep stosw      // 这个指令组合是 repeat store string word 的缩写，意思是重复存储字（16位）。
                           // 它将 ax 寄存器中的数据填充到由 edi 寄存器指向的内存地址，填充的次数由 ecx 寄存器中的值决定。
        }
    }

    // 这个函数的作用是将指定的32位数据 data 重复填充到 dest 指向的内存区域，填充的次数为 count 次。
    inline void Mem_Set_QUAD(void* dest, std::uint32_t data, std::int32_t count)
    {
        _asm
        {
            mov edi, dest  // 将目标内存地址 dest 存入 edi 寄存器，edi 是目标内存地址指针
            mov ecx, count // 将需要填充的32位字的数量 count 存入 ecx 寄存器，ecx 是计数器寄存器。
            mov eax, data  // 将32位数据 data 存入 eax 寄存器，eax 是源数据寄存器。
            rep stosd      // 这个指令组合是 repeat store string doubleword 的缩写，意思是重复存储双字（32位）。
                           // 它将 eax 寄存器中的数据填充到由 edi 寄存器指向的内存地址，填充的次数由 ecx 寄存器中的值决定。
        }
    }
}