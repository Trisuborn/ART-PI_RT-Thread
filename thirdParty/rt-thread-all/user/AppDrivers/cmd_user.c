
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
    dfs_sdmmc_mnt_init();
    dfs_spi_flash_mnt_init();

    if (rt_device_find("spi10") != NULL) {
        rt_kprintf("found spi10\n");
    }

    if (rt_device_find("W25Q64") != NULL) {
        rt_kprintf("found W25Q64\n");
    }
}
MSH_CMD_EXPORT(dfs_device_mnt, "mount dfs for Sd Card and SPI Flash.");
#endif

#ifdef RT_USING_PIN
#include "pin.h"
#include "drv_gpio.h"
static void pinctl(int argc, char **argv)
{
	
}
#endif

#ifdef BSP_USING_LCD
static void show_pic(int argc, char **argv)
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
