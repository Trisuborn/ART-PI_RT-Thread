/************************************************
 * @file lcd_st7735.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-12-08
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#ifndef LCD_ST7735_H
#define LCD_ST7735_H

/* includes */
#include "../lcd_st77xx_typedef.h"
#include "stm32h7xx_hal.h"


/* definations */
#define LCD_ST7735_X            160
#define LCD_ST7735_Y            80
#define LCD_ST7735_SPI          SPI4

#define LCD_ST7735_MOSI         GPIO_PIN_14		// PE14
#define LCD_ST7735_MISO         NULL
#define LCD_ST7735_CLK          GPIO_PIN_12		// PE12
#define LCD_ST7735_SPI_PIN      LCD_ST7735_MOSI | LCD_ST7735_MISO | LCD_ST7735_CLK

#define LCD_ST7735_CS           GPIO_PIN_11     // PE11
#define LCD_ST7735_RST          GPIO_PIN_14     // PB14
#define LCD_ST7735_DC           GPIO_PIN_13      // PE13
#define LCD_ST7735_BLK          GPIO_PIN_10      // PE10

#define	LCD_ST7735_CS_SET  	    GPIOE->BSRR = (uint32_t)LCD_ST7735_CS
#define	LCD_ST7735_CS_CLR  	    GPIOE->BSRR = (uint32_t)LCD_ST7735_CS <<16

#define	LCD_ST7735_RST_SET      GPIOE->BSRR |= (uint32_t)LCD_ST7735_RST
#define	LCD_ST7735_RST_CLR  	GPIOE->BSRR |= (uint32_t)LCD_ST7735_RST <<16

#define	LCD_ST7735_DC_SET  	    GPIOE->BSRR = (uint32_t)LCD_ST7735_DC
#define	LCD_ST7735_DC_CLR  	    GPIOE->BSRR = (uint32_t)LCD_ST7735_DC <<16

#define	LCD_ST7735_BLK_SET  	GPIOE->BSRR = (uint32_t)LCD_ST7735_BLK
#define	LCD_ST7735_BLK_CLR  	GPIOE->BSRR = (uint32_t)LCD_ST7735_BLK <<16


extern struct lcd_st77xx_propertis lcd_st7735;

// void st7735_init(void);
void st7735_ops_register(void);

#endif
