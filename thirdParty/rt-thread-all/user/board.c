/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-26     zylx         first version
 */

#include "board.h"

/**
 * Function    ota_app_vtor_reconfig
 * Description Set Vector Table base location to the start addr of app(RT_APP_PART_ADDR).
*/
static int ota_app_vtor_reconfig(void)
{
#define RT_APP_PART_ADDR 0x08020000
#define NVIC_VTOR_MASK   0x3FFFFF80
/* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = RT_APP_PART_ADDR & NVIC_VTOR_MASK;

    return 0;
}
// INIT_BOARD_EXPORT(ota_app_vtor_reconfig);


/* memheap initialization */
struct rt_memheap axi_sram0 = { 0 };
struct rt_memheap ahb_sram1 = { 0 };
struct rt_memheap ahb_sram2 = { 0 };
struct rt_memheap ahb_sram3 = { 0 };
struct rt_memheap ahb_sram4 = { 0 };
struct rt_memheap bkup_sram = { 0 };
static int stm32_ex_sram_init()
{
#if defined(RT_USING_MEMHEAP) && defined(SOC_SERIES_STM32H7)

    /* Check sram whether it can be used or not */
    uint32_t tmp = 0x0904;
    __IO uint32_t* sram_p;
    uint32_t offset = 0xFF;

    sram_p = (__IO uint32_t*)0x24000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&axi_sram0, "axi_sram0", (void*)(0x24000000), (512 * 1024));

    sram_p = (__IO uint32_t*)0x30000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&ahb_sram1, "ahb_sram1", (void*)(0x30000000), (128 * 1024));

    sram_p = (__IO uint32_t*)0x30020000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&ahb_sram2, "ahb_sram2", (void*)(0x30020000), (128 * 1024));

    sram_p = (__IO uint32_t*)0x30040000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&ahb_sram3, "ahb_sram3", (void*)(0x30040000), (32 * 1024));

    sram_p = (__IO uint32_t*)0x38000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&ahb_sram4, "ahb_sram4", (void*)(0x38000000), (64 * 1024));

    sram_p = (__IO uint32_t*)0x38800000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if (*sram_p == tmp)
        rt_memheap_init(&bkup_sram, "bkup_sram", (void*)(0x38800000), (4 * 1024));
#endif
    return 0;
}
INIT_PREV_EXPORT(stm32_ex_sram_init);

