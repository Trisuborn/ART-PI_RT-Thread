
#include "main.h"
#include "sys_conf.h"
#include "pro_conf.h"
#include "lcd_port.h"
#include "lcd_st7735.h"

#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)
#define LCD_BLK     GET_PIN(D, 4)

#endif


uint8_t* buf = RT_NULL;
static int sram_mc(void)
{
    uint32_t i = 0;
    uint32_t size = (64 * 1024);
    // while (size) {
    //     buf = (uint8_t*)rt_malloc(size);
    //     if (buf == RT_NULL) {
    //         rt_kprintf("rt_malloc malloc error (%d)\n", i++);
    //         size -= RT_ALIGN_SIZE;
    //     } else {
    //         break;
    //     }
    //     rt_thread_mdelay(10);
    // }

    // while (size) {
    //     buf = (uint8_t*)rt_malloc(4096);
    //     if (buf == RT_NULL) {
    //         rt_kprintf("rt_malloc malloc error (%d)\n", i++);
    //         break;
    //     } else {
    //         size -= 4096;
    //     }
    // }
    buf = (uint8_t*)rt_malloc(size);
    *buf = 0x0904;
    rt_free(buf);
}
MSH_CMD_EXPORT(sram_mc, sram_mc);


int main()
{
    
    rt_kprintf("AXI_SRAM_ZI_SIZE : %x\n", AXI_SRAM_ZI_SIZE);
#if USER_USE_RTTHREAD == (1u)

    uint32_t ts, te, t;

    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    uint32_t i = 0;
    while (1) {
        rt_thread_mdelay(14);
        //ts = rt_tick_get();
        lcd_st7735_clear_with(i);
        //te = rt_tick_get();
        //rt_kprintf("%d tick/f\n", te - ts);
        i += 0xF;
    }

#else
    HAL_Init();
    SystemClock_Config();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitTypeDef led;

    lcd_st7735_init();

    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Pin = GPIO_PIN_3;
    led.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &led);


    uint32_t i = 0;
    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        lcd_st7735_clear_with(i);
        i += 0xF;
        HAL_Delay(50);
    }

#endif

    return 0;
}

