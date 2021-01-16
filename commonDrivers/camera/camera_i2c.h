/************************************************
 * @file camera_i2c.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-16
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#ifndef __CAMERA_I2C_H__
#define __CAMERA_I2C_H__

#include "stm32h7xx_hal.h"


struct camera_i2c_ops {
    uint8_t(*read_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* recv_buf, size_t r_size);
    uint8_t(*writ_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* send_buf, size_t w_size);
    int (*xclk_config)(uint8_t xclk_source);
    void(*init)(void);
};
extern struct camera_i2c_ops camera_i2c;





#endif /* __CAMERA_I2C_H__ */
