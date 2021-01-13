/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include <stm32h7xx.h>

#ifdef __cplusplus
extern "C" {
#endif


// #define STM32_SRAM_SIZE           (512)
// #define STM32_SRAM_END            (0x24000000 + STM32_SRAM_SIZE * 1024)

#define STM32_SRAM_SIZE           (128)
#define STM32_SRAM_END            (0x20000000 + STM32_SRAM_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
//extern int Image$$AXI_SRAM$$ZI$$Limit;
//#define HEAP_BEGIN      (&Image$$AXI_SRAM$$ZI$$Limit)

extern int Image$$DTCM$$ZI$$Limit;
#define HEAP_BEGIN      (&Image$$DTCM$$ZI$$Limit)

#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      (&__bss_end)
#endif

#define HEAP_END        STM32_SRAM_END


#if defined(RT_USING_MEMHEAP) && defined(SOC_SERIES_STM32H7)
extern struct rt_memheap ahb_sram1;
extern struct rt_memheap ahb_sram2;
extern struct rt_memheap ahb_sram3;
extern struct rt_memheap ahb_sram4;
extern struct rt_memheap bkup_sram;
#endif


#ifdef __cplusplus
}
#endif

#endif
