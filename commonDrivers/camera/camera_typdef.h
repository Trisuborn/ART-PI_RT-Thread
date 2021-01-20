/************************************************
 * @file camera_typdef.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-20
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#ifndef __CAMERA_TYPEDEF_H__
#define __CAMERA_TYPEDEF_H__

/* includes */
#include "stm32h7xx_hal.h"

/* definations */
#define OV7670_ADDRESS 0x42u
#define OV2640_ADDRESS 0x30u
#define OV7725_ADDRESS 0x42u
#define OV5640_ADDRESS 0x78u


struct camera_i2c_ops {
    void(*init)(void);
    uint8_t (*read_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* recv_buf, size_t r_size);
    uint8_t (*writ_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* send_buf, size_t w_size);
};

struct camera_ops {
    void(*init)(void);
};

struct camera_ov_t {
    uint8_t *name;
    struct camera_i2c_ops *i2c_ops;     // i2c operational handler
    struct camera_ops *ops;             // operational handler

    uint16_t dev_addr;              // camera i2c address
    uint16_t mid;                   // manufacture id
    uint16_t pid;                   // product id
    uint8_t  flag;                  // i2c already config flag
};

#endif /* __CAMERA_TYPEDEF_H__ */
