
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
}
MSH_CMD_EXPORT(dfs_device_mnt, "mount dfs for Sd Card and SPI Flash.");
#endif

#ifdef RT_USING_PIN
#include "pin.h"
#include "drv_gpio.h"
#endif

#endif
