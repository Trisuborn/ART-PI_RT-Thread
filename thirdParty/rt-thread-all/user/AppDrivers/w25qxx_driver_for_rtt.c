/************************************************
 * @file w25qxx_driver_for_rtt.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-07
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/

#include <pro_conf.h>

#if USER_USE_RTTHREAD == 1

#include <board.h>
#include <drv_qspi.h>
#include <rtdevice.h>
#include <rthw.h>
#include <finsh.h>
#include "drv_spi.h"
#include <dfs_posix.h>

#ifdef BSP_USING_QSPI_FLASH
#include "drv_qspi.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"

char w25qxx_read_status_register2(struct rt_qspi_device* device)
{
    /* 0x35 read status register2 */
    char instruction = 0x35, status;

    rt_qspi_send_then_recv(device, &instruction, 1, &status, 1);

    return status;
}

void w25qxx_write_enable(struct rt_qspi_device* device)
{
    /* 0x06 write enable */
    char instruction = 0x06;

    rt_qspi_send(device, &instruction, 1);
}

void w25qxx_enter_qspi_mode(struct rt_qspi_device* device)
{
    char status = 0;
    /* 0x38 enter qspi mode */
    char instruction = 0x38;
    char write_status2_buf[2] = { 0 };

    /* 0x31 write status register2 */
    write_status2_buf[0] = 0x31;

    status = w25qxx_read_status_register2(device);
    if (!(status & 0x02)) {
        status |= 1 << 1;
        w25qxx_write_enable(device);
        write_status2_buf[1] = status;
        rt_qspi_send(device, &write_status2_buf, 2);
        rt_qspi_send(device, &instruction, 1);
        rt_kprintf("flash already enter qspi mode\n");
        rt_thread_mdelay(10);
    }
}

static int rt_hw_spi_flash_with_sfud_init(void)
{
    /* 普通SPI1 */
    struct rt_spi_configuration cfg = {
        .data_width = 8,
        .mode = RT_SPI_MODE_3 | RT_SPI_MSB,
        .max_hz = (RT_SFUD_SPI_MAX_HZ / 1)
    };
    __HAL_RCC_GPIOA_CLK_ENABLE();
    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);
    // rt_hw_spi_device_attach("spi1", "spi10", GPIOD, GPIO_PIN_6);
    if (RT_NULL == rt_sfud_flash_probe_ex("W25Q128", "spi10", &cfg, RT_NULL)) {
        rt_kprintf("SPI Flash have't been probe.\n");
    };

    /* QUAD SPI BANK1 */
    stm32_qspi_bus_attach_device("qspi1", "qspi10", RT_NULL, 4, w25qxx_enter_qspi_mode, RT_NULL);
    if (RT_NULL == rt_sfud_flash_probe("W25Q64", "qspi10")) {
        return -RT_ERROR;
    }
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);

#if defined(RT_USING_DFS_ELMFAT) && !defined(RT_USING_SDIO)
#include <dfs_fs.h>

#define SPI_DEV  "W25Q128"
#define QSPI_DEV  "W25Q64"

int spi_flash_mnt_init(void)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    // /* 确定挂载点是否存在 */
    // DIR* dir = NULL;
    // int res = -1;

    // uint8_t mnt_flag[2] = { 0 };

    // dir = opendir("/W25Q128");
    // if (dir == NULL) {
    //     rt_kprintf("No W25Q128 mounted point. ready to create...\n");
    //     /* 创建挂载点 */
    //     res = mkdir("/W25Q128", 0x777);
    //     if (res != 0)
    //         rt_kprintf("Create mounted point failed.\n");
    //     else {
    //         /* 挂载设备 */
    //         res = dfs_mount(SPI_DEV, "/W25Q128", "elm", 0, NULL);
    //         if (res != 0) {
    //             rt_kprintf("DFS for /W25Q128 mounted failed.\n");
    //             mnt_flag[0] = 1;
    //         }
    //     }
    // }
    // closedir(dir);

    

    if (dfs_mount(SPI_DEV, "/", "elm", 0, 0) == 0) {
        rt_kprintf("01 file system initialization done!\n");
    } else {
        if (dfs_mkfs("elm", SPI_DEV) == 0) {
            if (dfs_mount(SPI_DEV, "/", "elm", 0, 0) == 0) {
                rt_kprintf("02 file system initialization done!\n");
            } else {
                rt_kprintf("02 file system initialization failed!\n");
            }
        }
    }
	
	if (dfs_mount(QSPI_DEV, "/", "elm", 0, 0) == 0) {
        rt_kprintf("01 file system initialization done!\n");
    } else {
        if (dfs_mkfs("elm", QSPI_DEV) == 0) {
            if (dfs_mount(QSPI_DEV, "/", "elm", 0, 0) == 0) {
                rt_kprintf("02 file system initialization done!\n");
            } else {
                rt_kprintf("02 file system initialization failed!\n");
            }
        }
    }

    return 0;
}
INIT_ENV_EXPORT(spi_flash_mnt_init);

#endif /* defined(RT_USING_DFS_ELMFAT) && !defined(BSP_USING_SDCARD) */
#endif /* BSP_USING_QSPI_FLASH */

#endif /* BSP_USING_QSPI_FLASH */
