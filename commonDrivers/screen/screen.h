/************************************************
 * @file screen.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#ifndef __SCREEN_H__
#define __SCREEN_H__

/* includes */
#include "stm32h7xx_hal.h"
#include "lcd_st7735.h"

/* definations */
struct screen_ops {
    void (*init)(void);
    void (*reg)(void);
    void (*draw_pixel)(uint16_t x_res, uint16_t y_res, uint16_t pix_dat);
    void(*set_region)(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res);
    void(*send_pixel_data)(uint16_t pix_dat);
};

extern struct screen_ops screen;


/* functions prototype */
void scr_init(void);
void scr_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat);
void scr_flush(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res, uint16_t color);

#endif
