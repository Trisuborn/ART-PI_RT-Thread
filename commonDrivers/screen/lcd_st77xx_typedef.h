/************************************************
 * @file lcd_st77xx.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief A header file for st7735 and st7789
 * @version 0.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#ifndef __LCD_ST77XX_H__
#define __LCD_ST77XX_H__

/* includes */
#include "stm32h7xx_hal.h"

/* definations */
/************************************************
 * @brief  LCD ST77XX 的通信方式
*************************************************/
typedef enum {
    ST77XX_SPI_SINGLE_LINE,        // SPI 单线
    ST77XX_SPI_DOUBEL_LINE,        // SPI 双线
    ST77XX_PARALLEL_8080_8BIT,     // 8080 8 bit 并行
    ST77XX_PARALLEL_8080_16BIT,    // 8080 16 bit 并行
} lcd_interface_t;

/************************************************
 * @brief  LCD ST77XX 的操作方法
*************************************************/
struct lcd_st77xx_ops {
    void(*init)(void);
    void(*hw_init)(lcd_interface_t lcd_if);
    HAL_StatusTypeDef(*read_id)(uint32_t* id);

    void(*set_region)(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res);
    void(*send_pixel_data)(uint16_t pix_dat);

    void(*draw_pixel)(uint16_t x_res, uint16_t y_res, uint16_t pix_dat);

    void(*disp_on)(void);
    void(*disp_off)(void);
    void(*hw_reset)(void);
    void(*sw_reset)(void);
};

/************************************************
 * @brief  LCD ST77XX 的属性
*************************************************/
struct lcd_st77xx_propertis {
    struct lcd_st77xx_ops* ops;
    lcd_interface_t lcd_if_t;
    uint32_t id;
    uint16_t x_res;
    uint16_t y_res;
};

#endif
