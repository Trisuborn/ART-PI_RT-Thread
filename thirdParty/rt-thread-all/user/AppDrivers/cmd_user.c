
#include "cmd_user.h"
#include <rtdevice.h>
#include <rthw.h>
#include <dfs_posix.h>


#ifdef RT_USING_FINSH
#include "finsh.h"




#ifdef RT_USING_DFS
#include "w25qxx_driver_for_rtt.h"
#include "sdmmc_driver_for_rtt.h"

static void dfs_device_mnt(void)
{
#ifdef RT_USING_SDIO
    //dfs_sdmmc_mnt_init();
#endif
#ifdef RT_USING_SFUD
    dfs_spi_flash_mnt_init();
#endif
}
MSH_CMD_EXPORT(dfs_device_mnt, mount dfs for Sd Cardand SPI Flash.);

static void spi1_mount(void)
{
    int res;
    res = dfs_mount("W25Q128", "/", "elm", 0, NULL);
    if (res != 0) {
        rt_kprintf("%s mounted failed.\n", SF0);
    }
}
MSH_CMD_EXPORT(spi1_mount, spi1_mount);
#endif

#ifdef RT_USING_PIN
#include "pin.h"
#include "drv_gpio.h"
static void pinctl(int argc, char** argv)
{

}
#endif

#ifdef BSP_USING_LCD
static void show_pic(int argc, char** argv)
{

}
MSH_CMD_EXPORT(show_pic, scr show pic from device)
#endif


static void tick(void)
{
    rt_kprintf("cur tick : %d\n", rt_tick_get());
}
MSH_CMD_EXPORT(tick, "Get cur tick of system.");

#endif
