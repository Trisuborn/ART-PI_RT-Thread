
#include "main.h"
#include "sys_conf.h"
#include "pro_conf.h"

#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define LED0_PIN    GET_PIN(I, 8)
#define LED1_PIN    GET_PIN(E, 3)
#define LCD_BLK     GET_PIN(D, 4)
#endif

static blk_on(void)
{
	rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
	rt_pin_write(LCD_BLK, PIN_HIGH);
}
MSH_CMD_EXPORT(blk_on, blk_on);

static blk_off(void)
{
	rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
	rt_pin_write(LCD_BLK, PIN_LOW);
}
MSH_CMD_EXPORT(blk_off, blk_off);



int main()
{
#if USER_USE_RTTHREAD == (1u)
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (1) {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

#else
    HAL_Init();
    SystemClock_Config();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    GPIO_InitTypeDef led;

    led.Mode = GPIO_MODE_OUTPUT_PP;
    led.Pin = GPIO_PIN_8;
    led.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOI, &led);

    bsp_InitExtSDRAM();

    for

        while (1) {
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
            HAL_Delay(500);
        }

#endif

    return 0;
}
















#include <board.h>
#include <lcd_port.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include <drv_log.h>

// static LTDC_HandleTypeDef LtdcHandle = { 0 };

// rt_err_t stm32_lcd_init()
// {
//     LTDC_LayerCfgTypeDef pLayerCfg = { 0 };
// 	
// 	rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
// 	rt_pin_write(LCD_BLK, PIN_HIGH);
// 
//     /* LTDC Initialization -------------------------------------------------------*/
// 
//     /* Polarity configuration */
//     /* Initialize the horizontal synchronization polarity as active low */
//     LtdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
//     /* Initialize the vertical synchronization polarity as active low */
//     LtdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
//     /* Initialize the data enable polarity as active low */
//     LtdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
//     /* Initialize the pixel clock polarity as input pixel clock */
//     LtdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
// 
//     /* Timing configuration */
//     /* Horizontal synchronization width = Hsync - 1 */
//     LtdcHandle.Init.HorizontalSync = LCD_HSYNC_WIDTH - 1;
//     /* Vertical synchronization height = Vsync - 1 */
//     LtdcHandle.Init.VerticalSync = LCD_VSYNC_HEIGHT - 1;
//     /* Accumulated horizontal back porch = Hsync + HBP - 1 */
//     LtdcHandle.Init.AccumulatedHBP = LCD_HSYNC_WIDTH + LCD_HBP - 1;
//     /* Accumulated vertical back porch = Vsync + VBP - 1 */
//     LtdcHandle.Init.AccumulatedVBP = LCD_VSYNC_HEIGHT + LCD_VBP - 1;
//     /* Accumulated active width = Hsync + HBP + Active Width - 1 */
//     LtdcHandle.Init.AccumulatedActiveW = LCD_HSYNC_WIDTH + LCD_HBP + 480 - 1;
//     /* Accumulated active height = Vsync + VBP + Active Heigh - 1 */
//     LtdcHandle.Init.AccumulatedActiveH = LCD_VSYNC_HEIGHT + LCD_VBP + 272 - 1;
//     /* Total height = Vsync + VBP + Active Heigh + VFP - 1 */
//     LtdcHandle.Init.TotalHeigh = LtdcHandle.Init.AccumulatedActiveH + LCD_VFP;
//     /* Total width = Hsync + HBP + Active Width + HFP - 1 */
//     LtdcHandle.Init.TotalWidth = LtdcHandle.Init.AccumulatedActiveW + LCD_HFP;
// 
//     /* Configure R,G,B component values for LCD background color */
//     LtdcHandle.Init.Backcolor.Blue = 0;
//     LtdcHandle.Init.Backcolor.Green = 0;
//     LtdcHandle.Init.Backcolor.Red = 0xFF;
// 
//     LtdcHandle.Instance = LTDC;
// 
//     /* Layer1 Configuration ------------------------------------------------------*/
// 
//     /* Windowing configuration */
//     pLayerCfg.WindowX0 = 0;
//     pLayerCfg.WindowX1 = 480;
//     pLayerCfg.WindowY0 = 0;
//     pLayerCfg.WindowY1 = 272;
// 
//     /* Pixel Format configuration*/
// 
//     //pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
// 
//     pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
// 
// 	//extern struct rt_memheap system_heap0;
// 	//static uint16_t *buf;
// 	//
// 	//buf = (uint16_t*)rt_memheap_alloc(&system_heap0, (480*272*2));
// 
//     /* Start Address configuration : frame buffer is located at FLASH memory */
//     pLayerCfg.FBStartAdress = 0;//(uint32_t)buf;
// 
//     /* Alpha constant (255 totally opaque) */
//     pLayerCfg.Alpha = 255;
// 
//     /* Default Color configuration (configure A,R,G,B component values) */
//     pLayerCfg.Alpha0 = 0;
//     pLayerCfg.Backcolor.Blue = 0;
//     pLayerCfg.Backcolor.Green = 0;
//     pLayerCfg.Backcolor.Red = 0xF8;
// 
//     /* Configure blending factors */
//     /* Constant Alpha value:  pLayerCfg.Alpha / 255
//        C: Current Layer Color
//        Cs: Background color
//        BC = Constant Alpha x C + (1 - Constant Alpha ) x Cs */
//     /* BlendingFactor1: Pixel Alpha x Constant Alpha */
//     pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
//     /* BlendingFactor2: 1 - (Pixel Alpha x Constant Alpha) */
//     pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
// 
//     /* Configure the number of lines and number of pixels per line */
//     pLayerCfg.ImageWidth = 480;
//     pLayerCfg.ImageHeight = 272;
// 
//     /* Configure the LTDC */
//     if (HAL_LTDC_Init(&LtdcHandle) != HAL_OK)     {
//         rt_kprintf("LTDC init failed");
//         return -RT_ERROR;
//     }
// 
//     /* Configure the Background Layer*/
//     //if (HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg, 0) != HAL_OK)     {
//     //    rt_kprintf("LTDC layer init failed");
//     //    return -RT_ERROR;
//     //}     
// 	//else     {
//     //    /* enable LTDC interrupt */
//     //    HAL_NVIC_SetPriority(LTDC_IRQn, 1, 0);
//     //    HAL_NVIC_EnableIRQ(LTDC_IRQn);
// 	//	
//     //    rt_kprintf("LTDC init success");
//     //    return RT_EOK;
//     //}
// 	
// 	
// 	
// }
// MSH_CMD_EXPORT(stm32_lcd_init, stm32_lcd_init);
// 
// void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
// {
// 
//     GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//     if (ltdcHandle->Instance == LTDC) {
//       /* LTDC clock enable */
//         __HAL_RCC_LTDC_CLK_ENABLE();
// 
//         __HAL_RCC_GPIOK_CLK_ENABLE();
//         __HAL_RCC_GPIOJ_CLK_ENABLE();
//         __HAL_RCC_GPIOI_CLK_ENABLE();
// 
//         GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_3 | GPIO_PIN_2
//             | GPIO_PIN_7 | GPIO_PIN_0 | GPIO_PIN_1;
//         GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//         GPIO_InitStruct.Pull = GPIO_PULLUP;
//         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//         GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//         HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);
// 
// 
//         GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_0
//             | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
//             | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
//         GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//         GPIO_InitStruct.Pull = GPIO_PULLUP;
//         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//         GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//         HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
// 
//         GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
//         GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//         GPIO_InitStruct.Pull = GPIO_PULLUP;
//         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//         GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//         HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
// 
//     }
// }


