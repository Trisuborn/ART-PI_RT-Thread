
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
#include "pro_conf.h"
#include "lcd_st7735.h"


#define LOG_TAG        "LCD_ST7735"

#if (USER_USE_RTTHREAD == 1)
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_spi.h"
#include "drv_log.h"

static struct rt_spi_device* st7735 = NULL;
static struct rt_spi_configuration st7735_cfg;
static struct rt_device_pwm* lcd_blk_pwm = NULL;

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
static void lcd_st7735_hw_init(lcd_interface_t lcd_if);
static HAL_StatusTypeDef lcd_st7735_read_id(uint32_t* id);
static void lcd_st7735_set_region(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res);
static void lcd_st7735_send_pixel_data(uint16_t pix_dat);
static void lcd_st7735_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat);
static void lcd_st7735_disp_on(void);
static void lcd_st7735_disp_off(void);
static void lcd_st7735_hw_reset(void);
static void lcd_st7735_sw_reset(void);

/************************************************
 * @brief Different interface init functions
*************************************************/
static HAL_StatusTypeDef lcd_st7735_spi_single_line_init(void);

/************************************************
 * @brief Miscellaneous functions
*************************************************/
static void spi_alert_speed(uint32_t spi_baudrate);

/************************************************
 * @brief lcd structure
*************************************************/
static struct lcd_st77xx_ops lcd_st7735_ops;
struct lcd_st77xx_propertis lcd_st7735 ={
    .id = 0,
    .x_res = LCD_ST7735_X,
    .y_res = LCD_ST7735_Y,
    .ops = &lcd_st7735_ops,
    .lcd_if_t = ST77XX_SPI_SINGLE_LINE
};

static SPI_HandleTypeDef h_st7735_spi ={
    .Instance = LCD_ST7735_SPI
};

const char* name = "lcd_st7735_spi";

/************************************************
 * @brief functions' realized
*************************************************/

/************************************************
 * @brief lcd_st7735_trans_byte
 *
 * @param byte
*************************************************/
static void lcd_st7735_trans_byte(uint8_t byte)
{
    MODIFY_REG(h_st7735_spi.Instance->CR2, SPI_CR2_TSIZE, 1);
    SET_BIT(h_st7735_spi.Instance->CR1, SPI_CR1_CSTART);
    while ( !(h_st7735_spi.Instance->SR & (SPI_FLAG_TXP)) );
    *((__IO uint8_t*) & h_st7735_spi.Instance->TXDR) = byte;
    while ( !(h_st7735_spi.Instance->SR & (SPI_FLAG_EOT)) );
    h_st7735_spi.Instance->IFCR |= (SPI_IFCR_EOTC | SPI_IFCR_TXTFC);
}

/************************************************
 * @brief lcd_st7735_read_reg
 *
 * @param reg_addr
 * @param pbuf
 * @return HAL_StatusTypeDef
*************************************************/
static HAL_StatusTypeDef lcd_st7735_read_reg(uint8_t reg_addr, uint8_t* pbuf)
{
    switch ( lcd_st7735.lcd_if_t ) {
    case ST77XX_SPI_SINGLE_LINE: {
        spi_alert_speed(SPI_BAUDRATEPRESCALER_16);
        LCD_ST7735_CS_CLR;
        LCD_ST7735_DC_CLR;
        HAL_SPI_Transmit(&h_st7735_spi, &reg_addr, 1, 0xFFFFFFFF);
        LCD_ST7735_DC_SET;
        HAL_SPI_Receive(&h_st7735_spi, pbuf, 1, 0xFFFFFFFF);
        LCD_ST7735_CS_SET;
        break;
    }
    default:
        break;
    }

    return HAL_OK;
}

static void lcd_st7735_send_cmd(uint8_t cmd)
{
    LCD_ST7735_DC_CLR;
    LCD_ST7735_CS_CLR;
    lcd_st7735_trans_byte(cmd);
    LCD_ST7735_CS_SET;
}

static void lcd_st7735_send_dat(uint8_t dat)
{
    LCD_ST7735_DC_SET;
    LCD_ST7735_CS_CLR;
    lcd_st7735_trans_byte(dat);
    LCD_ST7735_CS_SET;
}

static HAL_StatusTypeDef lcd_st7735_read_id(uint32_t* id)
{
    uint8_t RDID1 = 0xDA;
    uint8_t RDID2 = 0xDB;
    uint8_t RDID3 = 0xDC;

    if ( (lcd_st7735.lcd_if_t == ST77XX_SPI_DOUBEL_LINE) || (lcd_st7735.lcd_if_t == ST77XX_SPI_SINGLE_LINE) )
        spi_alert_speed(SPI_BAUDRATEPRESCALER_16);

    lcd_st7735_read_reg(RDID1, &RDID1);
    lcd_st7735_read_reg(RDID2, &RDID2);
    lcd_st7735_read_reg(RDID3, &RDID3);

    *id = RDID1;
    *id = (*id << 8) | RDID2;
    *id = (*id << 8) | RDID3;

    LOG_I("ST7735 RDDID : %x", *id);

    if ( (lcd_st7735.lcd_if_t == ST77XX_SPI_DOUBEL_LINE) || (lcd_st7735.lcd_if_t == ST77XX_SPI_SINGLE_LINE) ) {
        spi_alert_speed(SPI_BAUDRATEPRESCALER_2);
        __HAL_SPI_ENABLE(&h_st7735_spi);
        SPI_1LINE_TX(&h_st7735_spi);
    }

    return HAL_OK;
}

static void lcd_st7735_set_region(uint16_t x1_res, uint16_t y1_res, uint16_t x2_res, uint16_t y2_res)
{
    lcd_st7735_send_cmd(0x2a);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(x1_res + 1);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(x2_res + 1);

    lcd_st7735_send_cmd(0x2b);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(y1_res + 26);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(y2_res + 26);

    lcd_st7735_send_cmd(0x2c);
}

static void lcd_st7735_send_pixel_data(uint16_t pix_dat)
{
    lcd_st7735_send_dat(pix_dat >> 8);
    lcd_st7735_send_dat(pix_dat & 0xff);
}

static void lcd_st7735_draw_pixel(uint16_t x_res, uint16_t y_res, uint16_t pix_dat)
{
    lcd_st7735_set_region(x_res, y_res, x_res, y_res);
    lcd_st7735_send_pixel_data(pix_dat);
}

static void lcd_st7735_disp_on(void)
{
    lcd_st7735_send_cmd(0x11);      //Sleep exit
    lcd_st7735_send_cmd(0x29);      //Display on
}

static void lcd_st7735_disp_off(void)
{
    lcd_st7735_send_cmd(0x28);      //Display off
}

static void lcd_st7735_hw_reset(void)
{
    LCD_ST7735_RST_CLR;
    HAL_Delay(10);
    LCD_ST7735_RST_SET;
    HAL_Delay(130);
}

static void lcd_st7735_sw_reset(void)
{
    lcd_st7735_send_cmd(0x01);
    HAL_Delay(130);
}


static HAL_StatusTypeDef lcd_st7735_spi_single_line_init(void)
{

#if USER_USE_RTTHREAD == 1
    rt_err_t err = -RT_ERROR;
#if !defined(RT_USING_PWM)
    rt_pin_mode(LCD_BLK, PIN_MODE_OUTPUT);
#else
    lcd_blk_pwm = (struct rt_device_pwm*)rt_device_find("pwm1");
    if ( lcd_blk_pwm == RT_NULL ) {
        LOG_E("lcd blk pwm device got error.\n");
        goto __other;
    }
    err = rt_pwm_set(lcd_blk_pwm, 2, 100, 50);
    if ( err != RT_EOK ) {
        LOG_E("lcd blk pwm set error.\n");
        goto __other;
    }
    rt_pwm_enable(lcd_blk_pwm, 2, 1);
__other:
#endif
    rt_pin_mode(LCD_DC, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_CS, PIN_MODE_OUTPUT);

    if ( rt_device_find(name) )
        goto __other2;

    err = rt_hw_spi_device_attach("spi4", name, GPIOE, LCD_ST7735_CS);
    if ( err == RT_EOK ) {
        st7735_cfg.data_width = 8;
        st7735_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;
        st7735_cfg.max_hz = 120 * 1000 * 1000;
        st7735 = (struct rt_spi_device*)rt_device_find(name);
        if ( st7735 != NULL ) {
            err = rt_spi_configure(st7735, &st7735_cfg);
        } else {
            LOG_E("lcd_st7735_spi device not found.");
        }
    }
__other2:
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
    h_st7735_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    h_st7735_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
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
    if ( HAL_SPI_Init(&h_st7735_spi) != HAL_OK ) {
        LOG_E("HAL SPI init error");
        return HAL_ERROR;
    }
    __HAL_SPI_ENABLE(&h_st7735_spi);
    SPI_1LINE_TX(&h_st7735_spi);

    return HAL_OK;
}

static void spi_alert_speed(uint32_t spi_baudrate)
{
    __HAL_SPI_DISABLE(&h_st7735_spi);
    h_st7735_spi.Init.BaudRatePrescaler = spi_baudrate;
    if ( HAL_SPI_Init(&h_st7735_spi) != HAL_OK ) {
        LOG_E("HAL SPI alert speed error");
        return;
    }
    __HAL_SPI_ENABLE(&h_st7735_spi);
}


static void lcd_st7735_hw_init(lcd_interface_t lcd_if)
{
    HAL_StatusTypeDef err;

    lcd_st7735.lcd_if_t = lcd_if;

    /* Hardware initialization */
    switch ( lcd_st7735.lcd_if_t ) {
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

    if ( err != HAL_OK ) {
        LOG_E("lcd_st7735_hw_init error.");
    } else {
        LOG_I("lcd_st7735_hw_init Ok.");
    }

    // lcd_st7735_sw_reset();
    /* send basical commands to st7735 */
    lcd_st7735_send_cmd(0x11);//Sleep exit

    //ST7735R Frame Rate
    lcd_st7735_send_cmd(0xB1);
    lcd_st7735_send_dat(0x01);
    lcd_st7735_send_dat(0x2C);
    lcd_st7735_send_dat(0x2D);

    lcd_st7735_send_cmd(0xB2);
    lcd_st7735_send_dat(0x01);
    lcd_st7735_send_dat(0x2C);
    lcd_st7735_send_dat(0x2D);

    lcd_st7735_send_cmd(0xB3);
    lcd_st7735_send_dat(0x01);
    lcd_st7735_send_dat(0x2C);
    lcd_st7735_send_dat(0x2D);
    lcd_st7735_send_dat(0x01);
    lcd_st7735_send_dat(0x2C);
    lcd_st7735_send_dat(0x2D);

    lcd_st7735_send_cmd(0xB4); //Column inversion
    lcd_st7735_send_dat(0x07);

    //ST7735R Power Sequenc
    lcd_st7735_send_cmd(0xC0);
    lcd_st7735_send_dat(0xA2);
    lcd_st7735_send_dat(0x02);
    lcd_st7735_send_dat(0x84);
    lcd_st7735_send_dat(0xC1);
    lcd_st7735_send_dat(0xC5);

    lcd_st7735_send_cmd(0xC2);
    lcd_st7735_send_dat(0x0A);
    lcd_st7735_send_dat(0x00);

    lcd_st7735_send_cmd(0xC3);
    lcd_st7735_send_dat(0x8A);
    lcd_st7735_send_dat(0x2A);

    lcd_st7735_send_cmd(0xC4);
    lcd_st7735_send_dat(0x8A);
    lcd_st7735_send_dat(0xEE);

    lcd_st7735_send_cmd(0xC5); //VCOM
    lcd_st7735_send_dat(0x0E);

    lcd_st7735_send_cmd(0x36);
    lcd_st7735_send_dat(0xA8);

    lcd_st7735_send_cmd(0x21);

    //ST7735R Gamma Sequenc
    lcd_st7735_send_cmd(0xe0);
    lcd_st7735_send_dat(0x0f);
    lcd_st7735_send_dat(0x1a);
    lcd_st7735_send_dat(0x0f);
    lcd_st7735_send_dat(0x18);
    lcd_st7735_send_dat(0x2f);
    lcd_st7735_send_dat(0x28);
    lcd_st7735_send_dat(0x20);
    lcd_st7735_send_dat(0x22);
    lcd_st7735_send_dat(0x1f);
    lcd_st7735_send_dat(0x1b);
    lcd_st7735_send_dat(0x23);
    lcd_st7735_send_dat(0x37);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(0x07);
    lcd_st7735_send_dat(0x02);
    lcd_st7735_send_dat(0x10);

    lcd_st7735_send_cmd(0xe1);
    lcd_st7735_send_dat(0x0f);
    lcd_st7735_send_dat(0x1b);
    lcd_st7735_send_dat(0x0f);
    lcd_st7735_send_dat(0x17);
    lcd_st7735_send_dat(0x33);
    lcd_st7735_send_dat(0x2c);
    lcd_st7735_send_dat(0x29);
    lcd_st7735_send_dat(0x2e);
    lcd_st7735_send_dat(0x30);
    lcd_st7735_send_dat(0x30);
    lcd_st7735_send_dat(0x39);
    lcd_st7735_send_dat(0x3f);
    lcd_st7735_send_dat(0x00);
    lcd_st7735_send_dat(0x07);
    lcd_st7735_send_dat(0x03);
    lcd_st7735_send_dat(0x10);

    lcd_st7735_send_cmd(0xF0); //Enable test command
    lcd_st7735_send_dat(0x01);

    lcd_st7735_send_cmd(0xF6); //Disable ram power save mode
    lcd_st7735_send_dat(0x00);

    lcd_st7735_send_cmd(0x3A); //65k mode
    lcd_st7735_send_dat(0x05);

    lcd_st7735_disp_on();

}


void st7735_init(void)
{
    lcd_st7735.ops->hw_init(ST77XX_SPI_SINGLE_LINE);
    lcd_st7735.ops->read_id(NULL);
}


static uint8_t flag = 0;
void st7735_ops_register(void)
{
    lcd_st7735.ops->init            = st7735_init;
    lcd_st7735.ops->hw_init         = lcd_st7735_hw_init;
    lcd_st7735.ops->read_id         = lcd_st7735_read_id;
    lcd_st7735.ops->disp_on         = lcd_st7735_disp_on;
    lcd_st7735.ops->disp_off        = lcd_st7735_disp_off;
    lcd_st7735.ops->draw_pixel      = lcd_st7735_draw_pixel;
    lcd_st7735.ops->hw_reset        = lcd_st7735_hw_reset;
    lcd_st7735.ops->sw_reset        = lcd_st7735_sw_reset;
    lcd_st7735.ops->send_pixel_data = lcd_st7735_send_pixel_data;
    lcd_st7735.ops->set_region      = lcd_st7735_set_region;
    flag = 1;
}
