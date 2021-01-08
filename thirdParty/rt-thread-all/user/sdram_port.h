/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-04     zylx         The first version for STM32F4xx
 */

#ifndef __SDRAM_PORT_H__
#define __SDRAM_PORT_H__

#ifdef RT_USING_MEMHEAP_AS_HEAP
extern struct rt_memheap system_heap0;
#endif

/* parameters for sdram peripheral */
/* Bank1 or Bank2 */
#define SDRAM_TARGET_BANK               1
/* stm32h7 Bank1:0XC0000000  Bank2:0XD0000000 */
#define SDRAM_BANK_ADDR                 ((uint32_t)0XC0000000)
/* data width: 8, 16, 32 */
#define SDRAM_DATA_WIDTH                16
/* column bit numbers: 8, 9, 10, 11 */
#define SDRAM_COLUMN_BITS               9
/* row bit numbers: 11, 12, 13 */
#define SDRAM_ROW_BITS                  13
/* cas latency clock number: 1, 2, 3 */
#define SDRAM_CAS_LATENCY               2
/* read pipe delay: 0, 1, 2 */
#define SDRAM_RPIPE_DELAY               0
/* clock divid: 2, 3 */
#define SDCLOCK_PERIOD                  2

#define SDRAM_SIZE                      ((uint32_t)(32<<20))

/* Timing configuration for W9825G6KH-6 */
/* 160 MHz (480MHz/3) */
/* refresh rate counter */
// #define SDRAM_REFRESH_COUNT             ((uint32_t)1230)
// /* TMRD: 2 Clock cycles */
// #define LOADTOACTIVEDELAY               2
// /* TXSR: 8x10ns */
// #define EXITSELFREFRESHDELAY            13
// /* TRAS: 5x10ns */
// #define SELFREFRESHTIME                 8
// /* TRC:  6x10ns */
// #define ROWCYCLEDELAY                   10
// /* TWR:  2 Clock cycles */
// #define WRITERECOVERYTIME               2
// /* TRP:  2x10ns */
// #define RPDELAY                         4
// /* TRCD: 2x10ns */
// #define RCDDELAY                        4

/* 120 MHz (240MHz/2) */
/* refresh rate counter */
#define SDRAM_REFRESH_COUNT             ((uint32_t)917)
/* TMRD: 2 Clock cycles */
#define LOADTOACTIVEDELAY               2
/* TXSR: 8x10ns */
#define EXITSELFREFRESHDELAY            10
/* TRAS: 5x10ns */
#define SELFREFRESHTIME                 7
/* TRC:  6x10ns */
#define ROWCYCLEDELAY                   8
/* TWR:  2 Clock cycles */
#define WRITERECOVERYTIME               2
/* TRP:  2x10ns */
#define RPDELAY                         3
/* TRCD: 2x10ns */
#define RCDDELAY                        3

// /* 40 MHz (80MHz/2) */
/* refresh rate counter */
// #define SDRAM_REFRESH_COUNT             ((uint32_t)292)
// /* TMRD: 2 Clock cycles */
// #define LOADTOACTIVEDELAY               2
// /* TXSR: 8x10ns */
// #define EXITSELFREFRESHDELAY            5
// /* TRAS: 5x10ns */
// #define SELFREFRESHTIME                 2
// /* TRC:  6x10ns */
// #define ROWCYCLEDELAY                   3
// /* TWR:  2 Clock cycles */
// #define WRITERECOVERYTIME               2
// /* TRP:  2x10ns */
// #define RPDELAY                         1
// /* TRCD: 2x10ns */
// #define RCDDELAY                        1

/* memory mode register */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)




struct rt_memheap* rt_get_main_memheap(void);

#endif
