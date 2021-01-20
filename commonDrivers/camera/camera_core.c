/************************************************
 * @file camera_core.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 1.0
 * @date 2021-01-20
 *
 * @copyright Copyright (c) 2021
 *
 *************************************************/
#include "pro_conf.h"
#include "camera_i2c.h"
#include "camera_core.h"

#if USER_USE_RTTHREAD == 1
#define LOG_TAG       "CAM_CORE"
#include <drv_log.h>

#else
// #define LOG_I     rt_kprintf
// #define LOG_D     rt_kprintf
// #define LOG_E     rt_kprintf
#endif

HAL_StatusTypeDef camera_read_id(struct camera_ov_t* cam);
void camera_init(struct camera_ov_t* cam);



HAL_StatusTypeDef camera_read_id(struct camera_ov_t* cam)
{
    uint8_t mid_reg[2] ={ 0 };
    uint8_t pid_reg[2] ={ 0 };
    uint32_t mid_pat = 0;       // mid配对值
    uint32_t pid_pat = 0;       // pid配对值

    if ( !cam ) {
        LOG_E("camera obj error.");
        return HAL_ERROR;
    }

    if ( cam->dev_addr == OV2640_ADDRESS ) {
        mid_reg[0] = 0x1C;
        mid_reg[1] = 0x1D;
        pid_reg[0] = 0x0A;
        pid_reg[1] = 0x0B;
        pid_pat = 0x2642;
        mid_pat = 0x7FA2;
    }


    cam->i2c_ops->read_reg(cam->dev_addr, mid_reg[0], &mid_reg[0], 1);
    cam->i2c_ops->read_reg(cam->dev_addr, mid_reg[1], &mid_reg[1], 1);
    cam->mid = (mid_reg[0] << 8) | mid_reg[1];
    if ( cam->mid != mid_pat )
        LOG_E("read camera [%s] mid error.");
    else
        LOG_I("Camera [%s] mid : 0x%X", cam->name, cam->mid);

    cam->i2c_ops->read_reg(cam->dev_addr, pid_reg[0], &pid_reg[0], 1);
    cam->i2c_ops->read_reg(cam->dev_addr, pid_reg[1], &pid_reg[1], 1);
    cam->pid = (pid_reg[0] << 8) | pid_reg[1];
    if ( cam->pid != pid_pat )
        LOG_E("read camera [%s] pid error.", cam->name);
    else
        LOG_I("Camera [%s] pid : 0x%X", cam->name, cam->pid);

    return HAL_OK;
}

void camera_init(struct camera_ov_t* cam)
{
    if ( cam->dev_addr == 0 ) {
        LOG_E("Error camera addr.");
        return;
    }

    if ( cam->ops == NULL ) {
        LOG_E("Error camera handler.");
        return;
    }

    if ( (cam->mid != 0) || (cam->pid != 0) ) {
        LOG_E("Camera already initialized.");
        return;
    }

    /* 初始化I2C总线 */
    if ( !cam->flag && (camera_i2c.init != NULL) ) {
        cam->i2c_ops->init();
        cam->flag = 1;
    } else {
        LOG_E("Camera i2c error.");
        return;
    }

    /* 初始化摄像头 */
    if ( cam->ops != NULL )
        cam->ops->init();
    else {
        LOG_E("Camera obj error.");
        return;
    }

}



#if USER_USE_RTTHREAD == 1
static int camera_test(void)
{
    struct camera_ov_t cam_ov2640 ={ 0 };

    cam_ov2640.name     = "OV2640";
    cam_ov2640.dev_addr = OV2640_ADDRESS;
    cam_ov2640.i2c_ops  = &camera_i2c;
    cam_ov2640.ops      = NULL;
    cam_ov2640.mid      = 0;
    cam_ov2640.pid      = 0;
    cam_ov2640.flag     = 0;

    camera_init(&cam_ov2640);
    camera_read_id(&cam_ov2640);
}
// MSH_CMD_EXPORT(camera_test, camera_test);
INIT_APP_EXPORT(camera_test);
#endif /* USER_USE_RTTHREAD */


