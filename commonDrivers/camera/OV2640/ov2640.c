// #include "ov2640.h"
// #include "ov2640_regs.h"

// #define OV2640_XCLK_FREQUENCY       (20000000)
// #define OV2640_NUM_ALLOWED_SIZES    (19)
// #define NUM_BRIGHTNESS_LEVELS       (5)
// #define NUM_CONTRAST_LEVELS         (5)
// #define NUM_SATURATION_LEVELS       (5)
// #define NUM_EFFECTS                 (9)
// #define REGLENGTH 8

// #define ov2640_delay HAL_Delay

// static const uint8_t allowed_sizes[OV2640_NUM_ALLOWED_SIZES] ={
//     FRAMESIZE_CIF,      // 352x288
//     FRAMESIZE_SIF,      // 352x240
//     FRAMESIZE_QQVGA,    // 160x120
//     FRAMESIZE_128X64,   // 128x64
//     FRAMESIZE_QVGA,     // 320x240
//     FRAMESIZE_VGA,      // 640x480
//     FRAMESIZE_HVGA,     // 480 * 320
//     FRAMESIZE_WVGA2,    // 752x480
//     FRAMESIZE_SVGA,     // 800x600
//     FRAMESIZE_XGA,      // 1024x768
//     FRAMESIZE_SXGA,     // 1280x1024
//     FRAMESIZE_UXGA,     // 1600x1200
// };

// //---------------------------------------
// static const uint8_t rgb565_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_RGB565 },
//     { 0xD7, 0x03 },
//     { 0xE1, 0x77 },
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// //-------------------------------------
// static const uint8_t jpeg_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_JPEG_EN | IMAGE_MODE_RGB565 }, // | IMAGE_MODE_HREF_VSYNC
//     { 0xd7, 0x03 },
//     { 0xe1, 0x77 },
//     //{QS,            0x0c},
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// static const uint8_t yuyv_regs[][2] ={
//     { BANK_SEL, BANK_SEL_DSP },
//     { REG_RESET, REG_RESET_DVP },
//     { IMAGE_MODE, IMAGE_MODE_YUV422 },
//     { 0xd7, 0x03 },
//     { 0x33, 0xa0 },
//     { 0xe5, 0x1f },
//     { 0xe1, 0x67 },
//     { REG_RESET, 0x00 },
//     { 0, 0 },
// };

// //--------------------------------------------------------------------
// static const uint8_t brightness_regs[NUM_BRIGHTNESS_LEVELS + 1][5] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
//     { 0x00, 0x04, 0x09, 0x00, 0x00 }, /* -2 */
//     { 0x00, 0x04, 0x09, 0x10, 0x00 }, /* -1 */
//     { 0x00, 0x04, 0x09, 0x20, 0x00 }, /*  0 */
//     { 0x00, 0x04, 0x09, 0x30, 0x00 }, /* +1 */
//     { 0x00, 0x04, 0x09, 0x40, 0x00 }, /* +2 */
// };

// //----------------------------------------------------------------
// static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS + 1][7] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA, BPDATA, BPDATA },
//     { 0x00, 0x04, 0x07, 0x20, 0x18, 0x34, 0x06 }, /* -2 */
//     { 0x00, 0x04, 0x07, 0x20, 0x1c, 0x2a, 0x06 }, /* -1 */
//     { 0x00, 0x04, 0x07, 0x20, 0x20, 0x20, 0x06 }, /*  0 */
//     { 0x00, 0x04, 0x07, 0x20, 0x24, 0x16, 0x06 }, /* +1 */
//     { 0x00, 0x04, 0x07, 0x20, 0x28, 0x0c, 0x06 }, /* +2 */
// };

// //--------------------------------------------------------------------
// static const uint8_t saturation_regs[NUM_SATURATION_LEVELS + 1][5] ={
//     { BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
//     { 0x00, 0x02, 0x03, 0x28, 0x28 }, /* -2 */
//     { 0x00, 0x02, 0x03, 0x38, 0x38 }, /* -1 */
//     { 0x00, 0x02, 0x03, 0x48, 0x48 }, /*  0 */
//     { 0x00, 0x02, 0x03, 0x58, 0x58 }, /* +1 */
//     { 0x00, 0x02, 0x03, 0x68, 0x68 }, /* +2 */
// };

// //--------------------------------------------------------
// static const uint8_t OV2640_EFFECTS_TBL[NUM_EFFECTS][3]={
//     //00-0 05-0  05-1
//     { 0x00, 0x80, 0x80 }, // Normal
//     { 0x18, 0xA0, 0x40 }, // Blueish (cool light)
//     { 0x18, 0x40, 0xC0 }, // Redish (warm)
//     { 0x18, 0x80, 0x80 }, // Black and white
//     { 0x18, 0x40, 0xA6 }, // Sepia
//     { 0x40, 0x80, 0x80 }, // Negative
//     { 0x18, 0x50, 0x50 }, // Greenish
//     { 0x58, 0x80, 0x80 }, // Black and white negative
//     { 0x00, 0x80, 0x80 }, // Normal
// };

// //-----------------------------------------------------
// static uint8_t OV2640_WR_Reg(uint8_t reg, uint8_t data)
// {
//     Camera_WriteReg(&hcamera, reg, &data);
//     return 0;
// }

// //---------------------------------------
// static uint8_t OV2640_RD_Reg(uint8_t reg)
// {
//     uint8_t data;
//     Camera_ReadReg(&hcamera, reg, &data);
//     return data;
// }

// //------------------------------------------------
// static void wrSensorRegs(const uint8_t(*regs)[2])
// {
//     for ( int i = 0; regs[i][0]; i++ ) {
//         Camera_WriteReg(&hcamera, regs[i][0], &regs[i][1]);
//     }
// }

// //----------------
// static int reset()
// {
//     ov2640_delay(100);
//   // Reset all registers
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     OV2640_WR_Reg(COM7, COM7_SRST);
//     // Delay 5 ms
//     ov2640_delay(5);
//     wrSensorRegs(ov2640_Slow_regs);
//     // 30 ms
//     ov2640_delay(30);

//     return 0;
// }

// //----------------------------------------
// static int set_special_effect(uint8_t sde)
// {
//     if ( sde >= NUM_EFFECTS ) return -1;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(BPADDR, 0x00);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][0]);
//     OV2640_WR_Reg(BPADDR, 0x05);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][1]);
//     OV2640_WR_Reg(BPDATA, OV2640_EFFECTS_TBL[sde][2]);

//     return 0;
// }

// //-----------------------------------
// static int set_exposure(int exposure)
// {
//     int ret = 0;
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     if ( exposure == -1 ) {
//         // get exposure
//         uint16_t exp = (uint16_t)(OV2640_RD_Reg(REG45) & 0x3f) << 10;
//         exp |= (uint16_t)OV2640_RD_Reg(AEC) << 2;
//         exp |= (uint16_t)OV2640_RD_Reg(REG04) & 0x03;
//         ret = (int)exp;
//     } else if ( exposure == -2 ) {
//         // disable auto exposure and gain
//         OV2640_WR_Reg(COM8, COM8_SET(0));
//     } else if ( exposure > 0 ) {
//         // set manual exposure
//         int max_exp = (dvp_cam_resolution[hcamera.framesize][0] <= 800) ? 672 : 1248;
//         if ( exposure > max_exp ) exposure = max_exp;
//         OV2640_WR_Reg(COM8, COM8_SET(0));
//         OV2640_WR_Reg(REG45, (uint8_t)((exposure >> 10) & 0x3f));
//         OV2640_WR_Reg(AEC, (uint8_t)((exposure >> 2) & 0xff));
//         OV2640_WR_Reg(REG04, (uint8_t)(exposure & 3));
//     } else {
//         // enable auto exposure and gain
//         OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
//     }

//     // Enable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     return ret;
// }

// //-------------------------------------------
// static void _set_framesize(uint8_t framesize)
// {
//     uint8_t cbar, qsreg, com7;

//     // save color bar status and qs register
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     cbar = OV2640_RD_Reg(COM7) & COM7_COLOR_BAR;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     qsreg = OV2640_RD_Reg(QS);

//     uint16_t w = dvp_cam_resolution[framesize][0];
//     uint16_t h = dvp_cam_resolution[framesize][1];
//     const uint8_t(*regs)[2];

//     if ( w <= dvp_cam_resolution[FRAMESIZE_SVGA][0] ) regs = OV2640_svga_regs;
//     else regs = OV2640_uxga_regs;

//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     // Write output width
//     OV2640_WR_Reg(ZMOW, (w >> 2) & 0xFF); // OUTW[7:0] (real/4)
//     OV2640_WR_Reg(ZMOH, (h >> 2) & 0xFF); // OUTH[7:0] (real/4)
//     OV2640_WR_Reg(ZMHH, ((h >> 8) & 0x04) | ((w >> 10) & 0x03)); // OUTH[8]/OUTW[9:8]

//     // Set CLKRC
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     //OV2640_WR_Reg(CLKRC, ov2640_sensor->night_mode);

//     // Write DSP input regsiters
//     wrSensorRegs(regs);

//     // restore color bar status
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     com7 = OV2640_RD_Reg(COM7) | cbar;
//     OV2640_WR_Reg(COM7, com7);

//     // restore qs register
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(QS, qsreg);

//     // Enable DSP
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
// }

// //---------------------------------------------
// static int set_pixformat(pixformat_t pixformat)
// {
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);

//     switch ( pixformat ) {
//     case PIXFORMAT_RGB565:
//         //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//         //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN));
//         wrSensorRegs(rgb565_regs);
//         break;
//     case PIXFORMAT_JPEG:
//         //OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//         //OV2640_WR_Reg(COM8, COM8_SET(COM8_BNDF_EN));
//         wrSensorRegs(jpeg_regs);
//         break;
//     case PIXFORMAT_YUV422:
//         wrSensorRegs(yuyv_regs);
//         break;
//     default:
//         // Enable DSP
//         OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//         OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//         return -1;
//     }
//     _set_framesize(hcamera.framesize);
//     // Enable DSP (enabled in '_set_framesize'
//     //OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     //OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     // Delay 30 ms
//     //ov2640_delay(30);

//     return 0;
// }

// //-----------------------------------------
// static int set_framesize(framesize_t framesize)
// {
//     int i = OV2640_NUM_ALLOWED_SIZES;
//     for ( i=0; i < OV2640_NUM_ALLOWED_SIZES; i++ ) {
//         if ( allowed_sizes[i] == framesize ) break;
//     }
//     if ( i >= OV2640_NUM_ALLOWED_SIZES ) {
//         //LOGW(TAG, "Frame size %d not allowed", framesize);
//         return -1;
//     }

//     hcamera.framesize = framesize;
//     _set_framesize(framesize);

//     //delay 30 ms
//     ov2640_delay(30);

//     return 0;
// }

// //-------------------------------------------
// int ov2640_check_framesize(uint8_t framesize)
// {
//     int i = OV2640_NUM_ALLOWED_SIZES;
//     for ( i=0; i < OV2640_NUM_ALLOWED_SIZES; i++ ) {
//         if ( allowed_sizes[i] == framesize ) break;
//     }
//     if ( i >= OV2640_NUM_ALLOWED_SIZES ) return -1;
//     return 0;
// }

// //--------------------------------
// static int set_contrast(int level)
// {
//     level += (NUM_CONTRAST_LEVELS / 2) + 1;
//     if ( level < 0 || level > NUM_CONTRAST_LEVELS ) {
//         return -1;
//     }

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write contrast registers
//     for ( int i=0; i < sizeof(contrast_regs[0]) / sizeof(contrast_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(contrast_regs[0][i], contrast_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------------
// static int set_brightness(int level)
// {
//     level += (NUM_BRIGHTNESS_LEVELS / 2) + 1;
//     if ( (level < 0) || (level > NUM_BRIGHTNESS_LEVELS) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write brightness registers
//     for ( int i=0; i < sizeof(brightness_regs[0]) / sizeof(brightness_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(brightness_regs[0][i], brightness_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------------
// static int set_saturation(int level)
// {
//     level += (NUM_SATURATION_LEVELS / 2) + 1;
//     if ( (level < 0) || (level > NUM_SATURATION_LEVELS) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Write saturation registers
//     for ( int i=0; i < sizeof(saturation_regs[0]) / sizeof(saturation_regs[0][0]); i++ ) {
//         OV2640_WR_Reg(saturation_regs[0][i], saturation_regs[level][i]);
//     }
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);

//     return 0;
// }

// //----------------------------
// static int set_quality(int qs)
// {
//     if ( (qs < 2) || (qs > 60) ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     // Write QS register
//     OV2640_WR_Reg(QS, qs);

//     return 0;
// }

// //---------------------------------
// static int set_colorbar(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(COM7);

//     if ( enable ) reg |= COM7_COLOR_BAR;
//     else reg &= ~COM7_COLOR_BAR;

//     return OV2640_WR_Reg(COM7, reg);
// }

// //--------------------------------
// static int set_hmirror(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(REG04);

//     if ( !enable ) { // Already mirrored.
//         reg |= REG04_HFLIP_IMG;
//     } else {
//         reg &= ~REG04_HFLIP_IMG;
//     }

//     return OV2640_WR_Reg(REG04, reg);
// }

// //------------------------------
// static int set_vflip(int enable)
// {
//     uint8_t reg;
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     reg = OV2640_RD_Reg(REG04);

//     if ( !enable ) { // Already flipped.
//         reg |= REG04_VFLIP_IMG | REG04_VREF_EN;
//     } else {
//         reg &= ~(REG04_VFLIP_IMG | REG04_VREF_EN);
//     }

//     return OV2640_WR_Reg(REG04, reg);
// }

// //---------------------------------------------------------
// static int read_id(uint16_t* manuf_id, uint16_t* device_id)
// {
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     *manuf_id = ((uint16_t)OV2640_RD_Reg(0x1C) << 8) | OV2640_RD_Reg(0x1D);
//     *device_id = ((uint16_t)OV2640_RD_Reg(0x0A) << 8) | OV2640_RD_Reg(0x0B);
//     return 0;
// }

// //------------------------------------
// static int read_reg(uint16_t reg_addr)
// {
//     return (int)OV2640_RD_Reg((uint8_t)reg_addr);
// }

// //--------------------------------------------------------
// static int write_reg(uint16_t reg_addr, uint16_t reg_data)
// {
//     return (int)OV2640_WR_Reg((uint8_t)reg_addr, (uint8_t)reg_data);
// }

// static const uint8_t OV2640_LIGHTMODE_TBL[5][3]=
// {
//     { 0x5e, 0x41, 0x54 }, //Auto, NOT used
//     { 0x5e, 0x41, 0x54 }, //Sunny
//     { 0x52, 0x41, 0x66 }, //Office
//     { 0x65, 0x41, 0x4f }, //Cloudy
//     { 0x42, 0x3f, 0x71 }, //Home
// };


// //-------------------------------------
// static int set_light_mode(uint8_t mode)
// {
//     if ( mode > 4 ) return -1;

//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     if ( mode == 0 ) {
//         OV2640_WR_Reg(0xc7, 0x00); // AWB on
//     } else {
//         OV2640_WR_Reg(0xc7, 0x40); // AWB off
//         OV2640_WR_Reg(0xcc, OV2640_LIGHTMODE_TBL[mode][0]);
//         OV2640_WR_Reg(0xcd, OV2640_LIGHTMODE_TBL[mode][1]);
//         OV2640_WR_Reg(0xce, OV2640_LIGHTMODE_TBL[mode][2]);
//     }
//     return 0;
// }

// //-----------------------------------
// static int set_night_mode(int enable)
// {
//     // Disable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_BYPAS);
//     // Set CLKRC
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_SENSOR);
//     OV2640_WR_Reg(CLKRC, (enable) ? 0 : CLKRC_DOUBLE);
//     // Enable DSP
//     OV2640_WR_Reg(BANK_SEL, BANK_SEL_DSP);
//     OV2640_WR_Reg(R_BYPASS, R_BYPASS_DSP_EN);
//     //delay 30 ms
//     ov2640_delay(30);
//     return 0;
// }

// //===============================
// int ov2640_init(framesize_t framesize)
// {
//     reset();
//     hcamera.framesize = framesize;
//     hcamera.pixformat = PIXFORMAT_RGB565;
//     //set_framesize(FRAMESIZE_QQVGA);
//     set_pixformat(hcamera.pixformat);
//     set_hmirror(0);
//     set_vflip(0);
//     return 0;
// }

/************************************************
 * @file ov2640.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-21
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"

#include "ov2640.h"
#include "ov2640_regs.h"
#include "camera_i2c.h"



struct camera_ov_t cam_ov2640 ={ 0 };

static uint8_t read_reg(uint8_t reg, uint8_t len, uint8_t* buf)
{
    return cam_ov2640.i2c_ops->read_reg(cam_ov2640.dev_addr, reg, buf, len);
}

/* i2c write reg */
static uint8_t write_reg(uint8_t reg, uint8_t data)
{
    return cam_ov2640.i2c_ops->writ_reg(cam_ov2640.dev_addr, reg, &data, 1);
}

/* change ov2640 to jpeg mode */
void ov2640_jpeg_mode(void)
{
    uint16_t i = 0;
    /* set yun422 mode */
    // for ( i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++ ) {
    //     write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]);
    // }
    for ( i = 0; ov2640_yuv422_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]);
    }

    /* set jpeg mode */
    // for ( i=0;i < (sizeof(ov2640_jpeg_reg_tbl) / 2);i++ ) {
    //     write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);
    // }
    for ( i = 0; ov2640_jpeg_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);
    }
}

/* change ov2640 to rgb565 mode */
void ov2640_rgb565_mode(void)
{
    uint16_t i=0;
    // for ( i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 2); i++ ) {
    //     write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
    // }
    for ( i = 0; i < ov2640_rgb565_reg_tbl[i][0]; i++ ) {
        write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]);
    }
}

/* set auto exposure */
void ov2640_set_auto_exposure(uint8_t level)
{
    uint8_t i = 0;
    uint8_t* p = (uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
    for ( i = 0; i < 4; i++ ) {
        write_reg(p[i * 2], p[i * 2 + 1]);
    }
}

/* set light mode
 * 0: auto
 * 1: sunny
 * 2: cloudy
 * 3: office
 * 4: home
 * */
void ov2640_set_light_mode(uint8_t mode)
{
    uint8_t regccval, regcdval, regceval;

    switch ( mode ) {
    case 0:
        write_reg(0xFF, 0x00);
        write_reg(0xC7, 0x10);
        return;

    case 2:
        regccval = 0x65;
        regcdval = 0x41;
        regceval = 0x4F;
        break;

    case 3:
        regccval = 0x52;
        regcdval = 0x41;
        regceval = 0x66;
        break;

    case 4:
        regccval = 0x42;
        regcdval = 0x3F;
        regceval = 0x71;
        break;

    default:
        regccval = 0x5E;
        regcdval = 0x41;
        regceval = 0x54;
        break;
    }

    write_reg(0xFF, 0x00);
    write_reg(0xC7, 0x40);
    write_reg(0xCC, regccval);
    write_reg(0xCD, regcdval);
    write_reg(0xCE, regceval);
}

/* set color saturation
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: +1
 * 4: +2
 * */
void ov2640_set_color_saturation(uint8_t sat)
{
    uint8_t reg7dval = ((sat + 2) << 4) | 0x08;
    write_reg(0xFF, 0X00);
    write_reg(0x7C, 0X00);
    write_reg(0x7D, 0X02);
    write_reg(0x7C, 0X03);
    write_reg(0x7D, reg7dval);
    write_reg(0x7D, reg7dval);
}

/* set brightness
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
void ov2640_set_brightness(uint8_t bright)
{
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, 0x04);
    write_reg(0x7c, 0x09);
    write_reg(0x7d, bright << 4);
    write_reg(0x7d, 0x00);
}

/* set contrast
 * 0: -2
 * 1: -1
 * 2: 0
 * 3: 1
 * 4: 2
 * */
void ov2640_set_contrast(uint8_t contrast)
{
    uint8_t reg7d0val, reg7d1val;

    switch ( contrast ) {
    case 0:
        reg7d0val = 0x18;
        reg7d1val = 0x34;
        break;

    case 1:
        reg7d0val = 0x1C;
        reg7d1val = 0x2A;
        break;

    case 3:
        reg7d0val = 0x24;
        reg7d1val = 0x16;
        break;

    case 4:
        reg7d0val = 0x28;
        reg7d1val = 0x0C;
        break;

    default:
        reg7d0val = 0x20;
        reg7d1val = 0x20;
        break;
    }
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, 0x04);
    write_reg(0x7c, 0x07);
    write_reg(0x7d, 0x20);
    write_reg(0x7d, reg7d0val);
    write_reg(0x7d, reg7d1val);
    write_reg(0x7d, 0x06);
}

/* set special effects
 * 0: noraml
 * 1: negative film
 * 2: black-and-white
 * 3: the red
 * 4: the green
 * 5: the blue
 * 6: Retro
*/
void ov2640_set_special_effects(uint8_t eft)
{
    uint8_t reg7d0val, reg7d1val, reg7d2val;

    switch ( eft ) {
    case 1:
        reg7d0val = 0x40;
        break;
    case 2:
        reg7d0val = 0x18;
        break;
    case 3:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0xC0;
        break;
    case 4:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0x40;
        break;
    case 5:
        reg7d0val = 0x18;
        reg7d1val = 0xA0;
        reg7d2val = 0x40;
        break;
    case 6:
        reg7d0val = 0x18;
        reg7d1val = 0x40;
        reg7d2val = 0xA6;
        break;
    default:
        reg7d0val = 0x00;
        reg7d1val = 0x80;
        reg7d2val = 0x80;
        break;
    }
    write_reg(0xff, 0x00);
    write_reg(0x7c, 0x00);
    write_reg(0x7d, reg7d0val);
    write_reg(0x7c, 0x05);
    write_reg(0x7d, reg7d1val);
    write_reg(0x7d, reg7d2val);
}

/* set the image output window */
void ov2640_set_window_size(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t endx;
    uint16_t endy;
    uint8_t temp;
    endx = sx + width / 2;
    endy = sy + height / 2;

    write_reg(0xFF, 0x01);
    read_reg(0x03, 1, &temp);
    temp &= 0xF0;
    temp |= ((endy & 0x03) << 2) | (sy & 0x03);
    write_reg(0x03, temp);
    write_reg(0x19, sy >> 2);
    write_reg(0x1A, endy >> 2);

    read_reg(0x32, 1, &temp);
    temp &= 0xC0;
    temp |= ((endx & 0x07) << 3) | (sx & 0x07);
    write_reg(0x32, temp);
    write_reg(0x17, sx >> 3);
    write_reg(0x18, endx >> 3);
}

/* set the image output size */
uint8_t ov2640_set_image_out_size(uint16_t width, uint16_t height)
{
    uint16_t outh, outw;
    uint8_t temp;

    if ( width % 4 )return 1;
    if ( height % 4 )return 2;
    outw = width / 4;
    outh = height / 4;
    write_reg(0xFF, 0x00);
    write_reg(0xE0, 0x04);
    write_reg(0x5A, outw & 0XFF);
    write_reg(0x5B, outh & 0XFF);
    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    write_reg(0x5C, temp);
    write_reg(0xE0, 0X00);

    return RT_EOK;
}

/* set the image window size */
uint8_t ov2640_set_image_window_size(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t hsize, vsize;
    uint8_t temp;
    if ( (width % 4) || (height % 4) ) {
        return RT_ERROR;
    }
    hsize = width / 4;
    vsize = height / 4;
    write_reg(0XFF, 0X00);
    write_reg(0XE0, 0X04);
    write_reg(0X51, hsize & 0XFF);
    write_reg(0X52, vsize & 0XFF);
    write_reg(0X53, offx & 0XFF);
    write_reg(0X54, offy & 0XFF);
    temp=(vsize >> 1) & 0X80;
    temp|=(offy >> 4) & 0X70;
    temp|=(hsize >> 5) & 0X08;
    temp|=(offx >> 8) & 0X07;
    write_reg(0X55, temp);
    write_reg(0X57, (hsize >> 2) & 0X80);
    write_reg(0XE0, 0X00);
    return 0;
}

/* set output resolution */
uint8_t ov2640_set_image_size(uint16_t width, uint16_t height)
{
    uint8_t temp;
    write_reg(0xFF, 0x00);
    write_reg(0xE0, 0x04);
    write_reg(0xC0, (width >> 3) & 0xFF);
    write_reg(0xC1, (height >> 3) & 0xFF);
    temp  = (width & 0x07) << 3;
    temp |= height & 0x07;
    temp |= (width >> 4) & 0x80;
    write_reg(0x8C, temp);
    write_reg(0xE0, 0x00);

    return RT_EOK;
}

static void ov2640_init(void)
{

}

struct camera_ops ov2640_ops = {
    .init = ov2640_init
};

static int camera_test(void)
{

    cam_ov2640.name     = (uint8_t*)"OV2640";
    cam_ov2640.dev_addr = OV2640_ADDRESS;
    cam_ov2640.i2c_ops  = &camera_i2c;
    cam_ov2640.ops      = &ov2640_ops;
    cam_ov2640.mid      = 0;
    cam_ov2640.pid      = 0;
    cam_ov2640.flag     = 0;

    camera_init(&cam_ov2640);
    camera_read_id(&cam_ov2640);

    // for ( uint16_t i = 0; ov2640_svga_init_reg_tbl[i][0]; i++ ) {
    //     write_reg(ov2640_svga_init_reg_tbl[i][0], ov2640_svga_init_reg_tbl[i][1]);
    // }

    // ov2640_rgb565_mode();
    // ov2640_set_light_mode(0);
    // ov2640_set_color_saturation(3);
    // ov2640_set_brightness(4);
    // ov2640_set_contrast(3);
    // ov2640_jpeg_mode();
    // ov2640_set_image_window_size(0, 0, 320, 240);
    // ov2640_set_image_out_size(320, 240);

}
#if USER_USE_RTTHREAD == 1
INIT_APP_EXPORT(camera_test);
#endif /* USER_USE_RTTHREAD */


