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

#include "w25qxx_driver_for_rtt.h"

#ifdef BSP_USING_QSPI_FLASH
#include "drv_qspi.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"
#endif


#define LOG_TAG       "W25QXX"
#include <drv_log.h>


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
    char write_status2_buf[2] ={ 0 };

    /* 0x31 write status register2 */
    write_status2_buf[0] = 0x31;

    status = w25qxx_read_status_register2(device);
    if ( !(status & 0x02) ) {
        status |= 1 << 1;
        w25qxx_write_enable(device);
        write_status2_buf[1] = status;
        rt_qspi_send(device, &write_status2_buf, 2);
        rt_qspi_send(device, &instruction, 1);
        LOG_I("flash already enter qspi mode\n");
        rt_thread_mdelay(10);
    }
}


static int rt_hw_spi_flash_with_sfud_init(void)
{
    /* 普通SPI1 */
    struct rt_spi_configuration spi_cfg ={
        .data_width = 8,
        .mode = RT_SPI_MODE_3 | RT_SPI_MSB,
        .max_hz = (RT_SFUD_SPI_MAX_HZ / 2)
    };
    // rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);
    rt_hw_spi_device_attach(SF0_BUS, SF0_DEV, GPIOD, GPIO_PIN_6);
    if ( RT_NULL == rt_sfud_flash_probe_ex(SF0, SF0_DEV, &spi_cfg, RT_NULL) ) {
        LOG_W("SPI Flash have't been probe in %s.", SF0_BUS);
    } else
        LOG_I("SPI Flash 's file system mounted success!");


    spi_cfg.max_hz = (RT_SFUD_SPI_MAX_HZ / 4);
    struct rt_qspi_configuration qspi_cfg ={
        .ddr_mode = 0,
        .medium_size = 0x800000,
        .qspi_dl_width = 4,
        .parent = spi_cfg
    };
    /* QUAD SPI BANK1 */
    stm32_qspi_bus_attach_device(QSF0_BUS, QSF0_DEV, RT_NULL, 4, w25qxx_enter_qspi_mode, RT_NULL);
    if ( RT_NULL == rt_sfud_flash_probe_ex(QSF0, QSF0_DEV, RT_NULL, &qspi_cfg) ) {
        LOG_W("QSPI Flash have't been probe in %s.", QSF0_BUS);
        return RT_ERROR;
    }

    LOG_I("QSPI Flash 's file system mounted success!");

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);


#if defined(RT_USING_DFS_ELMFAT)
#include <dfs_fs.h>

int dfs_spi_flash_mnt_init(void)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    /* 确定挂载点是否存在 */
    DIR* dir = NULL;
    int res = -1;
    struct rt_serial_device* console_serial;

    /* 获取控制台串口 */
    console_serial = (struct rt_serial_device*)rt_device_find(RT_CONSOLE_DEVICE_NAME);
    if ( !console_serial ) {
        LOG_E("console's serial can't to got.");
    }

    dir = opendir(SF0_MP);
    if ( dir == NULL ) {
        rt_kprintf("No %s mounted point. ready to create... ", SF0);
        /* 创建挂载点 */
        res = mkdir(SF0_MP, 0x777);
        if ( res != 0 )
            rt_kprintf("\nCreate mounted point for %s failed.\n", SF0_MP);
        else {
            rt_kprintf("Ok \n");
            /* 挂载设备 */
            res = dfs_mount(SF0, SF0_MP, "elm", 0, NULL);
            if ( res != 0 ) {
                rt_kprintf("%s mounted failed.\n", SF0_MP);
            }
        }
    } else {
        res = dfs_mount(SF0, SF0_MP, "elm", 0, NULL);
        if ( res != 0 ) {
            rt_kprintf("%s mounted failed.\n", SF0_MP);
        }
    }
    closedir(dir);

    dir = opendir(QSF0_MP);
    if ( dir == NULL ) {
        rt_kprintf("No %s mounted point. ready to create... ", QSF0);
        /* 创建挂载点 */
        res = mkdir(QSF0_MP, 0x777);
        if ( res != 0 )
            rt_kprintf("\nCreate mounted point for %s failed.\n", QSF0_MP);
        else {
            rt_kprintf("Ok \n");
            /* 挂载设备 */
            res = dfs_mount(QSF0, QSF0_MP, "elm", 0, NULL);
            if ( res != 0 ) {
                rt_kprintf("%s mounted failed.\n", QSF0_MP);
            }
        }
    } else {
        res = dfs_mount(QSF0, QSF0_MP, "elm", 0, NULL);
        if ( res != 0 ) {
            rt_kprintf("%s mounted failed.\n", QSF0_MP);
        }
    }
    closedir(dir);

    return 0;
}
INIT_ENV_EXPORT(dfs_spi_flash_mnt_init);

#endif /* defined(RT_USING_DFS_ELMFAT) && !defined(BSP_USING_SDCARD) */
#endif /* BSP_USING_QSPI_FLASH */
