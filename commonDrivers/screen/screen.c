/************************************************
 * @file screen.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"
#include "screen.h"

#define LOG_TAG        "SCREEN"

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

/************************************************
 * @brief 对接屏幕基本操作API
 *************************************************/
struct screen_ops screen;

static void screen_register(void)
{
    st7735_ops_register();
    screen.reg              = st7735_ops_register;
    screen.init             = lcd_st7735.ops->init;
    screen.draw_pixel       = lcd_st7735.ops->draw_pixel;
    screen.set_region       = lcd_st7735.ops->set_region;
    screen.send_pixel_data  = lcd_st7735.ops->send_pixel_data;
}


static void val_swap(uint16_t* val1, uint16_t* val2)
{
    uint16_t tmp = *val1;
    *val1 = *val2;
    *val2 = tmp;
}

void scr_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat)
{
    screen.draw_pixel(x_res, y_res, pix_dat);
}

void scr_flush(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res, uint16_t color)
{
    if ( x1_res > x2_res )
        val_swap(&x1_res, &x2_res);

    if ( y1_res > y2_res )
        val_swap(&y1_res, &y2_res);

    screen.set_region(x1_res, y1_res, x2_res - 1, y2_res - 1);
    for ( uint16_t y = 0; y < y2_res; y++ ) {
        for ( uint16_t x = 0; x < x2_res; x++ ) {
            screen.send_pixel_data(color);
        }
    }
}

void scr_init(void)
{
    /* 注册屏幕 */
    screen_register();
    /* 屏幕初始化 */
    screen.init();
}
#if USER_USE_RTTHREAD == 1
INIT_COMPONENT_EXPORT(scr_init);
#endif



