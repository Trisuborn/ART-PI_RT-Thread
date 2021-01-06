
#include "main.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

void SystemClock_Config(void);

static void periphal_init(void)
{
	/* Enable I-Cache---------------------------------------------------------*/
	SCB_EnableICache();
	/* Enable D-Cache---------------------------------------------------------*/
	SCB_EnableDCache();
    HAL_Init();
	SystemClock_Config();
	
    
	
	GPIO_InitTypeDef led;
	
	__HAL_RCC_GPIOI_CLK_ENABLE();
	
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin  = GPIO_PIN_8;
	led.Pull = GPIO_PULLUP;
	led.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init( GPIOI, &led );
    
}


#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)

int main()
{
    //periphal_init();
	
	rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
	
}

