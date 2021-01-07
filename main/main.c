
#include "main.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#include "spi.h"
#include "drv_spi.h"
#include "drv_qspi.h"
#include "spi_flash_sfud.h"

#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)

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
    led.Pin = GPIO_PIN_8;
    led.Pull = GPIO_PULLUP;
    led.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOI, &led);

}

static void device_init(void)
{
    /* 普通SPI1 */
    struct rt_spi_configuration cfg = {
        .data_width = 8,
        .mode = RT_SPI_MODE_3 | RT_SPI_MSB,
        .max_hz = (RT_SFUD_SPI_MAX_HZ / 2)
    };
    __HAL_RCC_GPIOA_CLK_ENABLE();
    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);
    // rt_hw_spi_device_attach("spi1", "spi10", GPIOD, GPIO_PIN_6);
    if (RT_NULL == rt_sfud_flash_probe_ex("W25QXX", "spi10", &cfg, RT_NULL)) {
        rt_kprintf("SPI Flash have't been probe.\n");
    };

    /* QUAD SPI BANK1 */
    

}


int main()
{
    //periphal_init();
    device_init();

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

}

