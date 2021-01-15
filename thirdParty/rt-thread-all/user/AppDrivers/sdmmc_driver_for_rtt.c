

#include <pro_conf.h>

#if USER_USE_RTTHREAD == 1

#include <board.h>
#include <rtdevice.h>
#include <rthw.h>
#include <dfs_posix.h>
#include <finsh.h>
#include "drv_sdio.h"
#include "sdmmc_driver_for_rtt.h"


int dfs_sdmmc_mnt_init(void)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    if (dfs_mount("sd0", "/", "elm", 0, 0) != 0) {
        rt_kprintf("SDMMC's file system mount failed!\n");
    } else {
        rt_kprintf("SDMMC's file system mount success!\n");
    }

    return 0;
}
INIT_ENV_EXPORT(dfs_sdmmc_mnt_init);

#endif
