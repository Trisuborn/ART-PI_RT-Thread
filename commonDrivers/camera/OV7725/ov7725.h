/************************************************
 * @file pin_wkup.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-13
 * 
 * @copyright Copyright (c) 2021
 * 
*************************************************/
#ifndef __PIN_WKUP_H__
#define __PIN_WKUP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include "stm32h7xx_hal.h"

/* definations */

/************************************************
* @arg PWR_WAKEUP_PIN1, PWR_WAKEUP_PIN2, PWR_WAKEUP_PIN3,
*      PWR_WAKEUP_PIN4, PWR_WAKEUP_PIN5, PWR_WAKEUP_PIN6.
*      or one of the following values where the user can explicitly states
*      the enabled pin and the chosen polarity:
*@arg PWR_WAKEUP_PIN1_HIGH, PWR_WAKEUP_PIN1_LOW,
*     PWR_WAKEUP_PIN2_HIGH, PWR_WAKEUP_PIN2_LOW,
*     PWR_WAKEUP_PIN3_HIGH, PWR_WAKEUP_PIN3_LOW,
*     PWR_WAKEUP_PIN4_HIGH, PWR_WAKEUP_PIN4_LOW,
*     PWR_WAKEUP_PIN5_HIGH, PWR_WAKEUP_PIN5_LOW,
*     PWR_WAKEUP_PIN6_HIGH, PWR_WAKEUP_PIN6_LOW.
*
*************************************************/
#define WKUP_USE_PIN    PWR_WAKEUP_PIN3     // PC13 HIGH


void sys_enter_standby(void);
uint8_t wkup_pin_check(uint8_t os_flag);

#ifdef __cplusplus
}
#endif

#endif
