
#include "main.h"
#include "sys_conf.h"

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
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    while (1) {
		rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
		rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
	 
	#else
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef led;
	
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = GPIO_PIN_3;
	led.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &led);
	
	while(1) {
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
		HAL_Delay(500);
	}
	
	#endif
	
}
