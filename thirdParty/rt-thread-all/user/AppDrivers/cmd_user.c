
#include "cmd_user.h"
#include <rtdevice.h>
#include <rthw.h>
#include <dfs_posix.h>


#ifdef RT_USING_FINSH
#include "finsh.h"

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
