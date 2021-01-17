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


static uint8_t flag = 0;

struct screen_ops screen;


static void screen_register(void)
{
    screen.init = lcd_st7735.ops->init;
    screen.flush = lcd_st7735.ops->flush;
    screen.draw_pixel = lcd_st7735.ops->draw_pixel;
    flag = 1;
}

void scr_init(void)
{
    if ( !flag )
        screen_register();
    screen.init();
}

void scr_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat)
{
    screen.draw_pixel(x_res, y_res, pix_dat);
}

void scr_flush(uint16_t color)
{
    screen.flush(color);
}
