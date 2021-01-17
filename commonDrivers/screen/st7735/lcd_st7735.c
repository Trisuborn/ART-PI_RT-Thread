// /************************************************
//  * @file lcd_st7735.c
//  * @author Trisuborn (ttowfive@gmail.com)
//  * @brief
//  * @version 0.1
//  * @date 2021-01-11
//  *
//  * @copyright Copyright (c) 2021
//  *
// *************************************************/
// #include "lcd_st7735.h"
// #include "lcd_st77xx.h"
// #include "pro_conf.h"


// #if USER_USE_RTTHREAD == 1
// #include <rtthread.h>
// #include <rtdevice.h>
// #include "drv_gpio.h"
// #include "drv_spi.h"
// #include "rt_drv_pwm.h"

// #define LOG_TAG             "drv.spi4"
// #define LOG_LVL             LOG_LVL_DBG
// #include "drv_log.h"

// static struct rt_spi_device* st7735;
// static struct rt_spi_configuration st7735_cfg;
// static struct rt_device_pwm* lcd_blk_pwm;

// #define LCD_BLK     GET_PIN(E, 10)
// #define LCD_DC      GET_PIN(E, 13)
// #define LCD_CS      GET_PIN(E, 11)
// #endif

// static SPI_HandleTypeDef h_st7735_spi = {
//     .Instance = LCD_ST7735_SPI
// };


// void lcd_gpio(void)
// {
// #if USER_USE_RTTHREAD == 1
//     rt_err_t err;
// #if !defined(RT_USING_PWM)
//     rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
// #else
// //     lcd_blk_pwm = (struct rt_device_pwm*)rt_device_find("pwm1");
// //     if (lcd_blk_pwm == RT_NULL) {
// //         LOG_E("lcd blk pwm device got error.\n");
// //         goto __other;
// //     }
// //     err = rt_pwm_set(&lcd_blk_pwm, 2, 100, 50);
// //     if (err != RT_EOK) {
// //         LOG_E("lcd blk pwm set error.\n");
// //         goto __other;
// //     }
// //     rt_pwm_enable(&lcd_blk_pwm, 2, 1);
// // __other:
// #endif
//     rt_pin_mode(LCD_DC, PIN_MODE_OUTPUT);
//     rt_pin_mode(LCD_CS, PIN_MODE_OUTPUT);

//     rt_hw_spi_device_attach("spi4", "spi40_st7735", GPIOE, LCD_ST7735_CS);

//     st7735 = (struct rt_spi_device*)rt_device_find("spi40_st7735");

//     st7735_cfg.data_width = 8;
//     st7735_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;
//     st7735_cfg.max_hz = 120 * 1000 * 1000;

//     rt_spi_configure(st7735, &st7735_cfg);
// #else
//     GPIO_InitTypeDef lcd_st7735_io_s;

//     __HAL_RCC_SPI4_CLK_ENABLE();
//     __HAL_RCC_GPIOE_CLK_ENABLE();

//     lcd_st7735_io_s.Pin = LCD_ST7735_DC | LCD_ST7735_BLK | LCD_ST7735_CS;
//     lcd_st7735_io_s.Mode = GPIO_MODE_OUTPUT_PP;
//     lcd_st7735_io_s.Pull = GPIO_PULLUP;
//     lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);

//     /* SPI */
//     /* PA5     ------> SPI1_SCK */
//     lcd_st7735_io_s.Pin = LCD_ST7735_MOSI | LCD_ST7735_CLK;
//     lcd_st7735_io_s.Mode = GPIO_MODE_AF_PP;
//     lcd_st7735_io_s.Pull = GPIO_PULLUP;
//     lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     lcd_st7735_io_s.Alternate = GPIO_AF5_SPI4;
//     HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);
// #endif

//     h_st7735_spi.Instance = SPI4;
//     h_st7735_spi.Init.Mode = SPI_MODE_MASTER;
//     h_st7735_spi.Init.Direction = SPI_DIRECTION_1LINE;
//     h_st7735_spi.Init.DataSize = SPI_DATASIZE_8BIT;
//     h_st7735_spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
//     h_st7735_spi.Init.CLKPhase = SPI_PHASE_2EDGE;
//     h_st7735_spi.Init.NSS = SPI_NSS_SOFT;
//     h_st7735_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//     h_st7735_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
//     h_st7735_spi.Init.TIMode = SPI_TIMODE_DISABLE;
//     h_st7735_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//     h_st7735_spi.Init.CRCPolynomial = 0x0;
//     h_st7735_spi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
//     h_st7735_spi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//     h_st7735_spi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_08DATA;
//     h_st7735_spi.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//     h_st7735_spi.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//     h_st7735_spi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//     h_st7735_spi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//     h_st7735_spi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//     h_st7735_spi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
//     h_st7735_spi.Init.IOSwap = SPI_IO_SWAP_DISABLE;
//     HAL_SPI_Init(&h_st7735_spi);
//     __HAL_SPI_ENABLE(&h_st7735_spi);
//     SPI_1LINE_TX(&h_st7735_spi);
// }

// void lcd_st7735_trans_byte(uint8_t byte)
// {
//     MODIFY_REG(h_st7735_spi.Instance->CR2, SPI_CR2_TSIZE, 1);
//     SET_BIT(h_st7735_spi.Instance->CR1, SPI_CR1_CSTART);
//     while (!(h_st7735_spi.Instance->SR & (SPI_FLAG_TXP)));
//     *((__IO uint8_t*) & h_st7735_spi.Instance->TXDR) = byte;
//     while (!(h_st7735_spi.Instance->SR & (SPI_FLAG_EOT)));
//     h_st7735_spi.Instance->IFCR |= (SPI_IFCR_EOTC | SPI_IFCR_TXTFC);

//     /* (SPI_FLAG_RXWNE | SPI_FLAG_FRLVL) */

// }

// void lcd_st7735_hw_reset(void)
// {
//     LCD_ST7735_RST_CLR;
//     DELAY(10);
//     LCD_ST7735_RST_SET;
//     DELAY(10);
// }

// #if USER_USE_RTTHREAD == 1
// INIT_APP_EXPORT(lcd_st7735_init);
// #endif
// int lcd_st7735_init(void)
// {
//     lcd_gpio();
//     // lcd_st7735_hw_reset();

//     lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit

//   //ST7735R Frame Rate
//     lcd_st7735_send(0xB1, ST7735_CMD);
//     lcd_st7735_send(0x01, ST7735_DAT);
//     lcd_st7735_send(0x2C, ST7735_DAT);
//     lcd_st7735_send(0x2D, ST7735_DAT);

//     lcd_st7735_send(0xB2, ST7735_CMD);
//     lcd_st7735_send(0x01, ST7735_DAT);
//     lcd_st7735_send(0x2C, ST7735_DAT);
//     lcd_st7735_send(0x2D, ST7735_DAT);

//     lcd_st7735_send(0xB3, ST7735_CMD);
//     lcd_st7735_send(0x01, ST7735_DAT);
//     lcd_st7735_send(0x2C, ST7735_DAT);
//     lcd_st7735_send(0x2D, ST7735_DAT);
//     lcd_st7735_send(0x01, ST7735_DAT);
//     lcd_st7735_send(0x2C, ST7735_DAT);
//     lcd_st7735_send(0x2D, ST7735_DAT);

//     lcd_st7735_send(0xB4, ST7735_CMD); //Column inversion
//     lcd_st7735_send(0x07, ST7735_DAT);

//     //ST7735R Power Sequence
//     lcd_st7735_send(0xC0, ST7735_CMD);
//     lcd_st7735_send(0xA2, ST7735_DAT);
//     lcd_st7735_send(0x02, ST7735_DAT);
//     lcd_st7735_send(0x84, ST7735_DAT);
//     lcd_st7735_send(0xC1, ST7735_CMD);
//     lcd_st7735_send(0xC5, ST7735_DAT);

//     lcd_st7735_send(0xC2, ST7735_CMD);
//     lcd_st7735_send(0x0A, ST7735_DAT);
//     lcd_st7735_send(0x00, ST7735_DAT);

//     lcd_st7735_send(0xC3, ST7735_CMD);
//     lcd_st7735_send(0x8A, ST7735_DAT);
//     lcd_st7735_send(0x2A, ST7735_DAT);
//     lcd_st7735_send(0xC4, ST7735_CMD);
//     lcd_st7735_send(0x8A, ST7735_DAT);
//     lcd_st7735_send(0xEE, ST7735_DAT);

//     lcd_st7735_send(0xC5, ST7735_CMD); //VCOM
//     lcd_st7735_send(0x0E, ST7735_DAT);

//     lcd_st7735_send(0x36, ST7735_CMD);
//     lcd_st7735_send(0xA8, ST7735_DAT);

//     lcd_st7735_send(0x21, ST7735_CMD);

//     //ST7735R Gamma Sequence
//     lcd_st7735_send(0xe0, ST7735_CMD);
//     lcd_st7735_send(0x0f, ST7735_DAT);
//     lcd_st7735_send(0x1a, ST7735_DAT);
//     lcd_st7735_send(0x0f, ST7735_DAT);
//     lcd_st7735_send(0x18, ST7735_DAT);
//     lcd_st7735_send(0x2f, ST7735_DAT);
//     lcd_st7735_send(0x28, ST7735_DAT);
//     lcd_st7735_send(0x20, ST7735_DAT);
//     lcd_st7735_send(0x22, ST7735_DAT);
//     lcd_st7735_send(0x1f, ST7735_DAT);
//     lcd_st7735_send(0x1b, ST7735_DAT);
//     lcd_st7735_send(0x23, ST7735_DAT);
//     lcd_st7735_send(0x37, ST7735_DAT);
//     lcd_st7735_send(0x00, ST7735_DAT);
//     lcd_st7735_send(0x07, ST7735_DAT);
//     lcd_st7735_send(0x02, ST7735_DAT);
//     lcd_st7735_send(0x10, ST7735_DAT);

//     lcd_st7735_send(0xe1, ST7735_CMD);
//     lcd_st7735_send(0x0f, ST7735_DAT);
//     lcd_st7735_send(0x1b, ST7735_DAT);
//     lcd_st7735_send(0x0f, ST7735_DAT);
//     lcd_st7735_send(0x17, ST7735_DAT);
//     lcd_st7735_send(0x33, ST7735_DAT);
//     lcd_st7735_send(0x2c, ST7735_DAT);
//     lcd_st7735_send(0x29, ST7735_DAT);
//     lcd_st7735_send(0x2e, ST7735_DAT);
//     lcd_st7735_send(0x30, ST7735_DAT);
//     lcd_st7735_send(0x30, ST7735_DAT);
//     lcd_st7735_send(0x39, ST7735_DAT);
//     lcd_st7735_send(0x3f, ST7735_DAT);
//     lcd_st7735_send(0x00, ST7735_DAT);
//     lcd_st7735_send(0x07, ST7735_DAT);
//     lcd_st7735_send(0x03, ST7735_DAT);
//     lcd_st7735_send(0x10, ST7735_DAT);

//     lcd_st7735_send(0xF0, ST7735_CMD); //Enable test command
//     lcd_st7735_send(0x01, ST7735_DAT);
//     lcd_st7735_send(0xF6, ST7735_CMD); //Disable ram power save mode
//     lcd_st7735_send(0x00, ST7735_DAT);

//     lcd_st7735_send(0x3A, ST7735_CMD); //65k mode
//     lcd_st7735_send(0x05, ST7735_DAT);

//     lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
//     lcd_st7735_displayOn();

//     lcd_st7735_clear_with(0xf800);
//     return 0;
// }

// void lcd_st7735_send_cmd(uint8_t cmd)
// {
//     LCD_ST7735_DC_CLR;
//     LCD_ST7735_CS_CLR;
//     lcd_st7735_trans_byte(cmd);
//     LCD_ST7735_CS_SET;
// }

// void lcd_st7735_send_dat(uint8_t dat)
// {
//     LCD_ST7735_DC_SET;
//     LCD_ST7735_CS_CLR;
//     lcd_st7735_trans_byte(dat);
//     LCD_ST7735_CS_SET;
// }

// void lcd_st7735_send(uint8_t byte, ST7735_DC_OPT opt)
// {
//     if (opt == ST7735_CMD)
//         lcd_st7735_send_cmd(byte);
//     else if (opt == ST7735_DAT)
//         lcd_st7735_send_dat(byte);
//     else
//         DEBUG_PRINT("send opt error.\n");

// }

// void lcd_st7735_powerUp(void)
// {
//     lcd_st7735_send(0x11, ST7735_CMD);//Sleep exit
//     lcd_st7735_displayOn();
//     LCD_ST7735_BLK_CLR;
// }

// void lcd_st7735_powerDown(void)
// {
//     lcd_st7735_send(0x12, ST7735_CMD);//Sleep enter
//     lcd_st7735_displayOff();
//     LCD_ST7735_BLK_SET;
// }

// void lcd_st7735_displayOn(void)
// {
//     lcd_st7735_send(0x29, ST7735_CMD);//Display on
// }

// void lcd_st7735_displayOff(void)
// {
//     lcd_st7735_send(0x28, ST7735_CMD);//Display off
// }

// void lcd_st7735_set_region(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
// {
//     lcd_st7735_send_cmd(0x2a);
//     lcd_st7735_send_dat(0x00);
//     lcd_st7735_send_dat(xs + 1);
//     lcd_st7735_send_dat(0x00);
//     lcd_st7735_send_dat(xe + 1);

//     lcd_st7735_send_cmd(0x2b);
//     lcd_st7735_send_dat(0x00);
//     lcd_st7735_send_dat(ys + 26);
//     lcd_st7735_send_dat(0x00);
//     lcd_st7735_send_dat(ye + 26);

//     lcd_st7735_send_cmd(0x2c);
// }

// void lcd_st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
// {
//     lcd_st7735_set_region(x, y, x, y);
//     lcd_st7735_send_pixel_dat(color);
// }

// void lcd_st7735_send_pixel_dat(uint16_t color)
// {
//     lcd_st7735_send_dat(color >> 8);
//     lcd_st7735_send_dat(color & 0xff);
// }

// void lcd_st7735_clear_with(uint16_t color)
// {
//     uint16_t x, y;

//     lcd_st7735_set_region(0, 0, ST7735_W - 1, ST7735_H - 1);

//     for (y = 0; y < ST7735_H; y++) {
//         for (x = 0; x < ST7735_W; x++) {
//             lcd_st7735_send_pixel_dat(color);
//         }
//     }

// }


// struct test {
//     uint16_t reg;
//     uint16_t ram;
// };

// static void st7735_read_id(void)
// {
//     uint8_t RDID1 = 0xDA;

//     LCD_ST7735_CS_CLR;
//     LCD_ST7735_DC_CLR;
//     HAL_SPI_Transmit(&h_st7735_spi, &RDID1, 1, 0xFFFFFFFF);
//     rt_hw_us_delay(10);
//     LCD_ST7735_DC_SET;
//     HAL_SPI_Receive(&h_st7735_spi, &RDID1, 1, 0xFFFFFFFF);
//     LCD_ST7735_CS_SET;
//     rt_kprintf("RDID1 : %x\n", RDID1);

//     struct test* tp = (struct test*)malloc(sizeof(struct test));

//     rt_kprintf("reg : 0x%08x\n", &tp->reg);
//     rt_kprintf("ram : 0x%08x\n", &tp->ram);



//     if (tp)
//         free(tp);

// }
// MSH_CMD_EXPORT(st7735_read_id, st7735_read_id);


/************************************************
 * @file lcd_st7735.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief lcd st7735
 * @version 0.1
 * @date 2021-01-17
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "lcd_st7735.h"
#include "pro_conf.h"

#define LOG_TAG        "LCD_ST7735"

#if (USER_USE_RTTHREAD == 1)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_spi.h"
#include "drv_log.h"

static struct rt_spi_device* st7735;
static struct rt_spi_configuration st7735_cfg;
static struct rt_device_pwm* lcd_blk_pwm;

#define LCD_BLK     GET_PIN(E, 10)
#define LCD_DC      GET_PIN(E, 13)
#define LCD_CS      GET_PIN(E, 11)
#else
#define LOG_E(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_D(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...)     printf(fmt, ##__VA_ARGS__)
#endif

/************************************************
 * @brief basical function
*************************************************/
static void lcd_st7735_hw_init(lcd_interface_t lcd_if_t);
static HAL_StatusTypeDef lcd_st7735_read_id(uint32_t* id);
static void lcd_st7735_set_region(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res);
static void lcd_st7735_send_pixel_data(uint16_t pix_dat);
static void lcd_st7735_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat);
static void lcd_st7735_flush(uint8_t* pbuf);
static void lcd_st7735_disp_on(void);
static void lcd_st7735_disp_off(void);
static void lcd_st7735_hw_reset(void);
static void lcd_st7735_sw_reset(void);

/************************************************
 * @brief Different interface init
*************************************************/
static HAL_StatusTypeDef lcd_st7735_spi_single_line_init(void);

/************************************************
 * @brief lcd structure
*************************************************/
static struct lcd_st77xx_ops lcd_st7735_ops = {
    .init = lcd_st7735_hw_init,
    .read_id = lcd_st7735_read_id,
    .set_region = lcd_st7735_set_region,
    .send_pixel_data = lcd_st7735_send_pixel_data,
    .draw_pixel = lcd_st7735_draw_pixel,
    .flush = lcd_st7735_flush,
    .disp_off = lcd_st7735_disp_on,
    .disp_off = lcd_st7735_disp_off,
    .hw_reset = lcd_st7735_hw_reset,
    .sw_reset = lcd_st7735_sw_reset
};

/************************************************
 * @brief functions' realized
*************************************************/
struct lcd_st77xx_propertis lcd_st7735 = {
    .id = 0,
    .x_res = LCD_ST7735_X,
    .x_res = LCD_ST7735_Y,
    .ops = &lcd_st7735_ops
};

static SPI_HandleTypeDef h_st7735_spi = {
    .Instance = LCD_ST7735_SPI
};

static HAL_StatusTypeDef lcd_st7735_read_reg(struct lcd_st77xx_propertis* lsp, uint8_t reg_addr, uint8_t* pbuf)
{
    
    // switch ()
    // {
    // case /* constant-expression */:
    //     /* code */
    //     break;
    
    // default:
    //     break;
    // }
    LCD_ST7735_CS_CLR;
    LCD_ST7735_DC_CLR;
    HAL_SPI_Transmit(&h_st7735_spi, &reg_addr, 1, 0xFFFFFFFF);
    LCD_ST7735_DC_SET;
    HAL_SPI_Receive(&h_st7735_spi, pbuf, 1, 0xFFFFFFFF);
    LCD_ST7735_CS_SET;

    return HAL_OK;
}

static HAL_StatusTypeDef lcd_st7735_read_id(uint32_t* id)
{
    uint8_t RDID1 = 0xDA;
    uint8_t RDID2 = 0xDB;
    uint8_t RDID3 = 0xDC;

    lcd_st7735_read_reg(RDID1, &RDID1);
    lcd_st7735_read_reg(RDID2, &RDID2);
    lcd_st7735_read_reg(RDID3, &RDID3);

    rt_kprintf("RDID1 : %x\n", RDID1);
    rt_kprintf("RDID2 : %x\n", RDID2);
    rt_kprintf("RDID3 : %x\n", RDID3);

    *id = RDID1;
    *id = (*id << 8) | RDID2;
    *id = (*id << 8) | RDID3;

    rt_kprintf("RDDID : %x\n", *id);
}

static void lcd_st7735_set_region(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res)
{

}
static void lcd_st7735_send_pixel_data(uint16_t pix_dat)
{

}
static void lcd_st7735_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat)
{

}
static void lcd_st7735_flush(uint8_t* pbuf)
{

}
static void lcd_st7735_disp_on(void)
{

}
static void lcd_st7735_disp_off(void)
{

}
static void lcd_st7735_hw_reset(void)
{

}
static void lcd_st7735_sw_reset(void)
{

}



static HAL_StatusTypeDef lcd_st7735_spi_single_line_init(void)
{
    GPIO_InitTypeDef lcd_st7735_io_s;

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
    rt_hw_spi_device_attach("spi4", "lcd_st7735_spi", GPIOE, LCD_ST7735_CS);

    st7735 = (struct rt_spi_device*)rt_device_find("lcd_st7735_spi");
    st7735_cfg.data_width = 8;
    st7735_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;
    st7735_cfg.max_hz = 120 * 1000 * 1000;
    rt_spi_configure(st7735, &st7735_cfg);
#else
    GPIO_InitTypeDef lcd_st7735_io_s;

    __HAL_RCC_SPI4_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    lcd_st7735_io_s.Pin = LCD_ST7735_DC | LCD_ST7735_BLK | LCD_ST7735_CS;
    lcd_st7735_io_s.Mode = GPIO_MODE_OUTPUT_PP;
    lcd_st7735_io_s.Pull = GPIO_PULLUP;
    lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);

    /* SPI */
    /* PA5     ------> SPI1_SCK */
    lcd_st7735_io_s.Pin = LCD_ST7735_MOSI | LCD_ST7735_CLK;
    lcd_st7735_io_s.Mode = GPIO_MODE_AF_PP;
    lcd_st7735_io_s.Pull = GPIO_PULLUP;
    lcd_st7735_io_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    lcd_st7735_io_s.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOE, &lcd_st7735_io_s);
#endif

    h_st7735_spi.Init.Mode = SPI_MODE_MASTER;
    h_st7735_spi.Init.Direction = SPI_DIRECTION_1LINE;
    h_st7735_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    h_st7735_spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    h_st7735_spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    h_st7735_spi.Init.NSS = SPI_NSS_SOFT;
    h_st7735_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
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
    if (HAL_SPI_Init(&h_st7735_spi) != HAL_OK) {
        return HAL_ERROR;
    }
    __HAL_SPI_ENABLE(&h_st7735_spi);
    SPI_1LINE_TX(&h_st7735_spi);

    return HAL_OK;
}

static void lcd_st7735_hw_init(lcd_interface_t lcd_if_t)
{
    HAL_StatusTypeDef err;

    lcd_st7735.lcd_if_t = lcd_if_t;

    /* Hardware initialization */
    switch (lcd_if_t) {
    case ST77XX_SPI_SINGLE_LINE:
        err = lcd_st7735_spi_single_line_init();
        break;
    case ST77XX_SPI_DOUBEL_LINE:
        break;
    case ST77XX_PARALLEL_8080_8BIT:
        break;
    case ST77XX_PARALLEL_8080_16BIT:
        break;

    default:
        break;
    }

    if (err != HAL_OK) {
        LOG_E("lcd_st7735_hw_init error.");
    } else {
        LOG_I("lcd_st7735_hw_init Ok.");
    }

    /* send basical commands to st7735 */

__err:
}


struct test {
    uint16_t reg;
    uint16_t ram;
};

static void st7735_read_id(void)
{
    uint8_t RDID1 = 0xDA;

    LCD_ST7735_CS_CLR;
    LCD_ST7735_DC_CLR;
    HAL_SPI_Transmit(&h_st7735_spi, &RDID1, 1, 0xFFFFFFFF);
    HAL_Delay(1);
    LCD_ST7735_DC_SET;
    HAL_SPI_Receive(&h_st7735_spi, &RDID1, 1, 0xFFFFFFFF);
    LCD_ST7735_CS_SET;
    rt_kprintf("RDID1 : %x\n", RDID1);

    struct test* tp = (struct test*)malloc(sizeof(struct test));

    rt_kprintf("reg : 0x%08x\n", &tp->reg);
    rt_kprintf("ram : 0x%08x\n", &tp->ram);


    if (tp)
        free(tp);

}
MSH_CMD_EXPORT(st7735_read_id, st7735_read_id);

void st7735_init(void)
{
    lcd_st7735_ops.init(ST77XX_SPI_SINGLE_LINE);
}
#if USER_USE_RTTHREAD == 1
INIT_APP_EXPORT(st7735_init);
#endif
