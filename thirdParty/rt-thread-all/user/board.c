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
__attribute__((unused)) static int ota_app_vtor_reconfig(void)
{
#define RT_APP_PART_ADDR 0x08020000
#define NVIC_VTOR_MASK   0x3FFFFF80
/* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = RT_APP_PART_ADDR & NVIC_VTOR_MASK;
    return 0;
}
// INIT_BOARD_EXPORT(ota_app_vtor_reconfig);


/* memheap initialization */
static struct rt_memheap axi_sram0 ={ 0 };
static struct rt_memheap ahb_sram123 ={ 0 };
// static struct rt_memheap ahb_sram1 = { 0 };
// static struct rt_memheap ahb_sram2 = { 0 };
// static struct rt_memheap ahb_sram3 = { 0 };
static struct rt_memheap ahb_sram4 ={ 0 };
static struct rt_memheap bkup_sram ={ 0 };
static int stm32_ex_sram_init()
{
#if defined(RT_USING_MEMHEAP) && defined(SOC_SERIES_STM32H7)

    /* Check sram whether it can be used or not */
    rt_err_t err = RT_ERROR;

    uint32_t tmp = 0x0904;
    __IO uint32_t* sram_p;
    uint32_t offset = 0xFF;

    sram_p = (__IO uint32_t*)0x24000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if ( *sram_p == tmp ) {
        /* 因为axi有SDMMC占用的ZI段，所以axi的实际大小为：axi sram size - ZI size */
        err = rt_memheap_init(&axi_sram0, "axi_sram0", (void*)(AXI_SRAM_ZI_END), AXI_SRAM_FREE_SIZE);
        if ( err != RT_EOK ) {
            rt_kprintf("axi_sram0 memheap init error.\n");
        }
    }

    sram_p = (__IO uint32_t*)0x30000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if ( *sram_p == tmp ) {
        err = rt_memheap_init(&ahb_sram123, "ahb_sram123", (void*)(0x30000000), (128 * 2 + 32) * 1024);
        if ( err != RT_EOK ) {
            rt_kprintf("ahb_sram123 memheap init error.\n");
        }
    }

    // sram_p = (__IO uint32_t*)0x30000000 + offset;
    // *sram_p = tmp;
    // HAL_Delay(5);
    // if (*sram_p == tmp) {
    //     err = rt_memheap_init(&ahb_sram1, "ahb_sram1", (void*)(0x30000000), (128 * 1024));
    //     if (err != RT_EOK) {
    //         rt_kprintf("ahb_sram1 memheap init error.\n");
    //     }
    // }

    // sram_p = (__IO uint32_t*)0x30020000 + offset;
    // *sram_p = tmp;
    // HAL_Delay(5);
    // if (*sram_p == tmp) {
    //     err = rt_memheap_init(&ahb_sram2, "ahb_sram2", (void*)(0x30020000), (128 * 1024));
    //     if (err != RT_EOK) {
    //         rt_kprintf("ahb_sram2 memheap init error.\n");
    //     }
    // }

    // sram_p = (__IO uint32_t*)0x30040000 + offset;
    // *sram_p = tmp;
    // HAL_Delay(5);
    // if (*sram_p == tmp) {
    //     err = rt_memheap_init(&ahb_sram3, "ahb_sram3", (void*)(0x30040000), (32 * 1024));
    //     if (err != RT_EOK) {
    //         rt_kprintf("ahb_sram3 memheap init error.\n");
    //     }
    // }

    sram_p = (__IO uint32_t*)0x38000000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if ( *sram_p == tmp ) {
        err = rt_memheap_init(&ahb_sram4, "ahb_sram4", (void*)(0x38000000), (64 * 1024));
        if ( err != RT_EOK ) {
            rt_kprintf("ahb_sram4 memheap init error.\n");
        }
    }

    sram_p = (__IO uint32_t*)0x38800000 + offset;
    *sram_p = tmp;
    HAL_Delay(5);
    if ( *sram_p == tmp ) {
        err = rt_memheap_init(&bkup_sram, "bkup_sram", (void*)(0x38800000), (4 * 1024));
        if ( err != RT_EOK ) {
            rt_kprintf("bkup_sram memheap init error.\n");
        }
    }

#endif
    return 0;
}
INIT_PREV_EXPORT(stm32_ex_sram_init);

#if defined(RT_USING_DFS) && defined(RT_USING_DFS_MNTTABLE)
#include "dfs.h"
#include "dfs_fs.h"
#include "dfs_posix.h"

#define SF0         "w25qxx_spi"
#define SF0_MP      "/SF0"
#define SF0_BUS     "spi1"
#define SF0_DEV     "spi10"

#define QSF0        "w25qxx_qspi"
#define QSF0_MP     "/QSF0"
#define QSF0_BUS    "qspi1"
#define QSF0_DEV    "qspi10"

const struct dfs_mount_tbl mount_table[] =
{
    { "sd0", "/", "elm", 0, 0 },
    { SF0, SF0_MP, "elm", 0, 0 },
    { QSF0, QSF0_MP, "elm", 0, 0 },
    { 0 }
};
#endif
