/************************************************
 * @file lcd_st7735.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-11
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "lcd_st7735.h"
#include "pro_conf.h"

#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_spi.h"
#include "rt_drv_pwm.h"

#define LOG_TAG             "drv.spi4"
#define LOG_LVL             LOG_LVL_DBG
#include "drv_log.h"

static struct rt_spi_device* st7735;
static struct rt_spi_configuration st7735_cfg;
static struct rt_device_pwm* lcd_blk_pwm;

#define LCD_BLK     GET_PIN(E, 10)
#define LCD_DC      GET_PIN(E, 13)
#define LCD_CS      GET_PIN(E, 11)
#endif

static SPI_HandleTypeDef h_st7735_spi = {
    .Instance = LCD_ST7735_SPI
};


void lcd_gpio(void)
{
#if USER_USE_RTTHREAD == 1
    rt_err_t err;
#if !defined(RT_USING_PWM)
    rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
#else
//     lcd_blk_pwm = (struct rt_device_pwm*)rt_device_find("pwm1");
//     if (lcd_blk_pwm == RT_NULL) {
//         LOG_E("lcd blk pwm device got error.\n");
//         goto __other;
//     }
//     err = rt_pwm_set(&lcd_blk_pwm, 2, 100, 50);
//     if (err != RT_EOK) {
//         LOG_E("lcd blk pwm set error.\n");
//         goto __other;
//     }
//     rt_pwm_enable(&lcd_blk_pwm, 2, 1);
// __other:
#endif
    rt_pin_mode(LCD_DC, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_CS, PIN_MODE_OUTPUT);

    rt_hw_spi_device_attach("spi4", "spi40_st7735", GPIOE, LCD_144_ST7735_CS);

    st7735 = (struct rt_spi_device*)rt_device_find("spi40_st7735");

    st7735_cfg.data_width = 8;
    st7735_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;
    st7735_cfg.max_hz = 120 * 1000 * 1000;

    rt_spi_configure(st7735, &st7735_cfg);
#else
    GPIO_InitTypeDef lcd_st7735_io_s;

    __HAL_RCC_SPI4_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    lcd_st7735_io_s.Pin = LCD_144_ST7735_DC | LCD_144_ST7735_BLK | LCD_144_ST7735_CS;
    lcd_st7735_io_s.Mode = GPIO_MODE_OUTPUT_PP;
    lcd_st7735_io_s.Pull = GPIO_PULLUP;
    lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);

    /* SPI */
    /* PA5     ------> SPI1_SCK */
    lcd_st7735_io_s.Pin = LCD_144_ST7735_MOSI | LCD_144_ST7735_CLK;
    lcd_st7735_io_s.Mode = GPIO_MODE_AF_PP;
    lcd_st7735_io_s.Pull = GPIO_PULLUP;
    lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    lcd_st7735_io_s.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);
#endif

    h_st7735_spi.Instance = SPI4;
    h_st7735_spi.Init.Mode = SPI_MODE_MASTER;
    h_st7735_spi.Init.Direction = SPI_DIRECTION_1LINE;
    h_st7735_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    h_st7735_spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    h_st7735_spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    h_st7735_spi.Init.NSS = SPI_NSS_SOFT;
    h_st7735_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    h_st7735_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    h_st7735_spi.Init.TIMode = SPI_TIMODE_DISABLE;
    h_st7735_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_st7735_spi.Init.CRCPolynomial = 0x0;
    h_st7735_spi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    h_st7735_spi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    h_st7735_spi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_08DATA;
    h_st7735_spi.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    h_st7735_spi.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    h_st7735_spi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    h_st7735_spi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    h_st7735_spi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    h_st7735_spi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    h_st7735_spi.Init.IOSwap = SPI_IO_SWAP_DISABLE;

    HAL_SPI_Init(&h_st7735_spi);
    __HAL_SPI_ENABLE(&h_st7735_spi);
    SPI_1LINE_TX(&h_st7735_spi);

}

void lcd_st7735_trans_byte(uint8_t byte)
{
    MODIFY_REG(h_st7735_spi.Instance->CR2, SPI_CR2_TSIZE, 1);
    SET_BIT(h_st7735_spi.Instance->CR1, SPI_CR1_CSTART);
    while (!(h_st7735_spi.Instance->SR & (SPI_FLAG_TXP)));
    *((__IO uint8_t*) & h_st7735_spi.Instance->TXDR) = byte;
    while (!(h_st7735_spi.Instance->SR & (SPI_FLAG_EOT)));
    h_st7735_spi.Instance->IFCR |= (SPI_IFCR_EOTC | SPI_IFCR_TXTFC);
}

void lcd_st7735_hw_reset(void)
{
    LCD_144_ST7735_RST_CLR;
    DELAY(10);
    LCD_144_ST7735_RST_SET;
    DELAY(10);
}

#if USER_USE_RTTHREAD == 1
INIT_APP_EXPORT(lcd_st7735_init);
#endif
int lcd_st7735_init(void)
{
    lcd_gpio();
    // lcd_st7735_hw_reset();

    lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit

  //ST7735R Frame Rate
    lcd_st7735_send(0xB1, ST7735_CMD);
    lcd_st7735_send(0x01, ST7735_DAT);
    lcd_st7735_send(0x2C, ST7735_DAT);
    lcd_st7735_send(0x2D, ST7735_DAT);

    lcd_st7735_send(0xB2, ST7735_CMD);
    lcd_st7735_send(0x01, ST7735_DAT);
    lcd_st7735_send(0x2C, ST7735_DAT);
    lcd_st7735_send(0x2D, ST7735_DAT);

    lcd_st7735_send(0xB3, ST7735_CMD);
    lcd_st7735_send(0x01, ST7735_DAT);
    lcd_st7735_send(0x2C, ST7735_DAT);
    lcd_st7735_send(0x2D, ST7735_DAT);
    lcd_st7735_send(0x01, ST7735_DAT);
    lcd_st7735_send(0x2C, ST7735_DAT);
    lcd_st7735_send(0x2D, ST7735_DAT);

    lcd_st7735_send(0xB4, ST7735_CMD); //Column inversion
    lcd_st7735_send(0x07, ST7735_DAT);

    //ST7735R Power Sequence
    lcd_st7735_send(0xC0, ST7735_CMD);
    lcd_st7735_send(0xA2, ST7735_DAT);
    lcd_st7735_send(0x02, ST7735_DAT);
    lcd_st7735_send(0x84, ST7735_DAT);
    lcd_st7735_send(0xC1, ST7735_CMD);
    lcd_st7735_send(0xC5, ST7735_DAT);

    lcd_st7735_send(0xC2, ST7735_CMD);
    lcd_st7735_send(0x0A, ST7735_DAT);
    lcd_st7735_send(0x00, ST7735_DAT);

    lcd_st7735_send(0xC3, ST7735_CMD);
    lcd_st7735_send(0x8A, ST7735_DAT);
    lcd_st7735_send(0x2A, ST7735_DAT);
    lcd_st7735_send(0xC4, ST7735_CMD);
    lcd_st7735_send(0x8A, ST7735_DAT);
    lcd_st7735_send(0xEE, ST7735_DAT);

    lcd_st7735_send(0xC5, ST7735_CMD); //VCOM
    lcd_st7735_send(0x0E, ST7735_DAT);

    lcd_st7735_send(0x36, ST7735_CMD);
    lcd_st7735_send(0xA8, ST7735_DAT);

    lcd_st7735_send(0x21, ST7735_CMD);

    //ST7735R Gamma Sequence
    lcd_st7735_send(0xe0, ST7735_CMD);
    lcd_st7735_send(0x0f, ST7735_DAT);
    lcd_st7735_send(0x1a, ST7735_DAT);
    lcd_st7735_send(0x0f, ST7735_DAT);
    lcd_st7735_send(0x18, ST7735_DAT);
    lcd_st7735_send(0x2f, ST7735_DAT);
    lcd_st7735_send(0x28, ST7735_DAT);
    lcd_st7735_send(0x20, ST7735_DAT);
    lcd_st7735_send(0x22, ST7735_DAT);
    lcd_st7735_send(0x1f, ST7735_DAT);
    lcd_st7735_send(0x1b, ST7735_DAT);
    lcd_st7735_send(0x23, ST7735_DAT);
    lcd_st7735_send(0x37, ST7735_DAT);
    lcd_st7735_send(0x00, ST7735_DAT);
    lcd_st7735_send(0x07, ST7735_DAT);
    lcd_st7735_send(0x02, ST7735_DAT);
    lcd_st7735_send(0x10, ST7735_DAT);

    lcd_st7735_send(0xe1, ST7735_CMD);
    lcd_st7735_send(0x0f, ST7735_DAT);
    lcd_st7735_send(0x1b, ST7735_DAT);
    lcd_st7735_send(0x0f, ST7735_DAT);
    lcd_st7735_send(0x17, ST7735_DAT);
    lcd_st7735_send(0x33, ST7735_DAT);
    lcd_st7735_send(0x2c, ST7735_DAT);
    lcd_st7735_send(0x29, ST7735_DAT);
    lcd_st7735_send(0x2e, ST7735_DAT);
    lcd_st7735_send(0x30, ST7735_DAT);
    lcd_st7735_send(0x30, ST7735_DAT);
    lcd_st7735_send(0x39, ST7735_DAT);
    lcd_st7735_send(0x3f, ST7735_DAT);
    lcd_st7735_send(0x00, ST7735_DAT);
    lcd_st7735_send(0x07, ST7735_DAT);
    lcd_st7735_send(0x03, ST7735_DAT);
    lcd_st7735_send(0x10, ST7735_DAT);

    lcd_st7735_send(0xF0, ST7735_CMD); //Enable test command
    lcd_st7735_send(0x01, ST7735_DAT);
    lcd_st7735_send(0xF6, ST7735_CMD); //Disable ram power save mode
    lcd_st7735_send(0x00, ST7735_DAT);

    lcd_st7735_send(0x3A, ST7735_CMD); //65k mode
    lcd_st7735_send(0x05, ST7735_DAT);

    lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
    lcd_st7735_displayOn();

    lcd_st7735_clear_with(0xf800);
    return 0;
}

void lcd_st7735_send_cmd(uint8_t cmd)
{
    LCD_144_ST7735_DC_CLR;
    LCD_144_ST7735_CS_CLR;
    lcd_st7735_trans_byte(cmd);
    LCD_144_ST7735_CS_SET;
}

void lcd_st7735_send_dat(uint8_t dat)
{
    LCD_144_ST7735_DC_SET;
    LCD_144_ST7735_CS_CLR;
    lcd_st7735_trans_byte(dat);
    LCD_144_ST7735_CS_SET;
}

void lcd_st7735_send(uint8_t byte, ST7735_DC_OPT opt)
{
    if (opt == ST7735_CMD)
        lcd_st7735_send_cmd(byte);
    else if (opt == ST7735_DAT)
        lcd_st7735_send_dat(byte);
    else
        DEBUG_PRINT("send opt error.\n");

}

void lcd_st7735_powerUp(void)
{
    lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
    lcd_st7735_displayOn();
    LCD_144_ST7735_BLK_CLR;
}

void lcd_st7735_powerDown(void)
{
    lcd_st7735_send(0x12, ST7735_CMD);//Sleep enter
    lcd_st7735_displayOff();
    LCD_144_ST7735_BLK_SET;
}

void lcd_st7735_displayOn(void)
{
    lcd_st7735_send(0x29, ST7735_CMD);//Display on
}

void lcd_st7735_displayOff(void)
{
    lcd_st7735_send(0x28, ST7735_CMD);//Display off
}

void lcd_st7735_set_region(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    lcd_st7735_send_cmd(0x2a);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(xs + 1);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(xe + 1);

    lcd_st7735_send_cmd(0x2b);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(ys + 26);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(ye + 26);

    lcd_st7735_send_cmd(0x2c);
}

void lcd_st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_st7735_set_region(x, y, x, y);
    lcd_st7735_send_pixel_dat(color);
}

void lcd_st7735_send_pixel_dat(uint16_t color)
{
    lcd_st7735_send_dat(color >> 8);
    lcd_st7735_send_dat(color & 0xff);
}

void lcd_st7735_clear_with(uint16_t color)
{
    uint16_t x, y;

    lcd_st7735_set_region(0, 0, ST7735_W - 1, ST7735_H - 1);

    for (y = 0; y < ST7735_H; y++) {
        for (x = 0; x < ST7735_W; x++) {
            lcd_st7735_send_pixel_dat(color);
        }
    }

}
