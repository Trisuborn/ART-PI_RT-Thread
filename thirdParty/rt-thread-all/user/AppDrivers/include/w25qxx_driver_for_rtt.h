

#ifndef __W25QXX_DRIVER_FOR_RTT_H__
#define __W25QXX_DRIVER_FOR_RTT_H__

#include <rtthread.h>
#include <stm32h7xx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SF0         "w25qxx_spi"
#define SF0_MP      "/SF0"
#define SF0_BUS     "spi1"
#define SF0_DEV     "spi10"

#define QSF0        "w25qxx_qspi"
#define QSF0_MP     "/QSF0"
#define QSF0_BUS    "qspi1"
#define QSF0_DEV    "qspi10"

int dfs_spi_flash_mnt_init(void);

#ifdef __cplusplus
}
#endif

#endif
