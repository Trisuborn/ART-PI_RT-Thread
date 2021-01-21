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
#define OV7725_ADDRESS 0x42u
#define OV5640_ADDRESS 0x78u

typedef enum {
    PIXFORMAT_INVALID = 0,
    PIXFORMAT_RGB565,       // 2BPP/RGB565
    PIXFORMAT_JPEG,         // JPEG/COMPRESSED
    PIXFORMAT_YUV422,
    PIXFORMAT_GRAYSCALE,
    PIXFORMAT_BAYER,
} cam_pixformat_t;

typedef enum {
    FRAMESIZE_INVALID = 0,
    // C/SIF Resolutions
    FRAMESIZE_QQCIF,    // 88x72 x
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_CIF,      // 352x288 x
    FRAMESIZE_QQSIF,    // 88x60 x
    FRAMESIZE_QSIF,     // 176x120
    FRAMESIZE_SIF,      // 352x240 x
    // VGA Resolutions
    FRAMESIZE_QQQQVGA,  // 40x30 x
    FRAMESIZE_QQQVGA,   // 80x60 x
    FRAMESIZE_QQVGA,    // 160x120 x
    FRAMESIZE_QVGA,     // 320x240 *
    FRAMESIZE_VGA,      // 640x480 *
    FRAMESIZE_HQQQVGA,  // 60x40
    FRAMESIZE_HQQVGA,   // 120x80 x
    FRAMESIZE_HQVGA,    // 240x160
    FRAMESIZE_HVGA,     // 480x320
    // FFT Resolutions
    FRAMESIZE_64X32,    // 64x32 x
    FRAMESIZE_64X64,    // 64x64 x
    FRAMESIZE_128X64,   // 128x64 x
    FRAMESIZE_128X128,  // 128x128
    // Other
    FRAMESIZE_LCD,      // 128x160
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_WVGA,     // 720x480 x
    FRAMESIZE_WVGA2,    // 752x480 x
    FRAMESIZE_SVGA,     // 800x600 x
    FRAMESIZE_XGA,      // 1024x768 x
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
    FRAMESIZE_720P,     // 1280x720 *
    FRAMESIZE_1080P,    // 1920x1080 *
    FRAMESIZE_960P,     // 1280x960 *
    FRAMESIZE_5MPP,     // 2592x1944 *
} cam_framesize_t;

struct camera_i2c_ops {
    void(*init)(void);
    uint8_t(*read_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* recv_buf, size_t r_size);
    uint8_t(*writ_reg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t* send_buf, size_t w_size);
};

struct camera_ops {
    void(*init)(void);
};

struct camera_ov_t {
    uint8_t* name;
    struct camera_i2c_ops* i2c_ops;     // i2c operational handler
    struct camera_ops* ops;             // operational handler

    uint16_t dev_addr;              // camera i2c address
    uint16_t mid;                   // manufacture id
    uint16_t pid;                   // product id
    uint8_t  flag;                  // i2c already config flag
};

#endif /* __CAMERA_TYPEDEF_H__ */
