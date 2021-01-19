/************************************************
 * @file main.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-16
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "main.h"
#include "sys_conf.h"
#include "pro_conf.h"
#include "screen.h"

#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)
#define LCD_BLK     GET_PIN(D, 4)
#endif


int main()
{

#if USER_USE_RTTHREAD == (1u)
    uint32_t ts, te, t;
    uint32_t i = 0;

    while ( 1 ) {
        // ts = rt_tick_get();
        scr_flush(0, 0, 160, 80, i);
        // te = rt_tick_get();
        // rt_kprintf("%d tick/f\n", te - ts);
        i += 0xF;
        rt_thread_mdelay(14);
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
    while ( 1 ) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        lcd_st7735_clear_with(i);
        i += 0xF;
        HAL_Delay(50);
    }

#endif

}

