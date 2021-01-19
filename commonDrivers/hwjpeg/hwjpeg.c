/************************************************
 * @file hwjpeg.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief hardware jpeg decoder and encoder
 * @version 0.1
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "pro_conf.h"
#include "hwjpeg.h"

#define LOG_TAG        "HWJPEG"

#if (USER_USE_RTTHREAD == 1)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_log.h"
#else
#define LOG_E(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_D(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#endif


JPEG_HandleTypeDef hjpeg;


static hwjpeg_decode()
{
    // HAL_JPEG_Decode()
    

}


/* JPEG init function */
int hwjpeg_init(void)
{
    hjpeg.Instance = JPEG;
    if ( HAL_JPEG_Init(&hjpeg) != HAL_OK ) {
        LOG_E("hardware jpeg init error.");
        return -1;
    } else
        LOG_I("hardware jpeg init ok.");
    return 0;
}
#if (USER_USE_RTTHREAD == 1)
INIT_COMPONENT_EXPORT(hwjpeg_init);
#endif

void HAL_JPEG_MspInit(JPEG_HandleTypeDef* jpegHandle)
{
    if ( jpegHandle->Instance == JPEG ) {
      /* JPEG clock enable */
        __HAL_RCC_JPEG_CLK_ENABLE();
    }
}

void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef* jpegHandle)
{
    if ( jpegHandle->Instance == JPEG ) {
      /* Peripheral clock disable */
        __HAL_RCC_JPEG_CLK_DISABLE();
    }
}
