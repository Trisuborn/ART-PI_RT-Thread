/************************************************
 * @file pin_wkup.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-01-13
 *
 * @copyright Copyright (c) 2021
 *
*************************************************/
#include "pin_wkup.h"
#include "pro_conf.h"


#if USER_USE_RTTHREAD == 1
#include <rtthread.h>
#include <drv_gpio.h>
#else
#define __STM32_PORT(port)  GPIO##port##_BASE

#if defined(SOC_SERIES_STM32MP1)
#define GET_PIN(PORTx,PIN) (GPIO##PORTx == GPIOZ) ? (176 + PIN) : ((rt_base_t)((16 * ( ((rt_base_t)__STM32_PORT(PORTx) - (rt_base_t)GPIOA_BASE)/(0x1000UL) )) + PIN))
#else
#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__STM32_PORT(PORTx) - (rt_base_t)GPIOA_BASE)/(0x0400UL) )) + PIN)
#endif
#endif


#if USER_USE_RTTHREAD == 1
rt_uint32_t wkup_pin = 0;
#else
uint32_t wkup_pin = 0;
#endif

static rt_mailbox_t wkup_mb = RT_NULL;


#if USER_USE_RTTHREAD == 1
/* sys_wkup thread */
#define PIN_WKUP_THREAD_NAME        "sys_wkup"
#define PIN_WKUP_THREAD_STACK_SIZE  512
#define PIN_WKUP_THREAD_PRIORITY    25
#define PIN_WKUP_THREAD_SLICE       1
static rt_thread_t sys_wkup_th;
static void sys_wkup(void* param)
{
    // param = param;
    rt_err_t err = RT_ERROR;
    uint8_t* mb_val = 0;

    while (1) {
        err = rt_mb_recv(wkup_mb, (rt_ubase_t*)&mb_val, RT_WAITING_FOREVER);
        if (err != RT_EOK) {
            rt_kprintf("mail recv error\n");
            goto __delay;
        }
        rt_kprintf("mail content is: %s\n", mb_val);

    __delay:
        rt_thread_mdelay(100);
    }
}
#endif 

static uint8_t led_flag = 0;
static uint8_t buf[] = "Hello mailbox.";
static void pin_callback(void* param)
{
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);

    rt_mb_send(wkup_mb, (rt_ubase_t)&buf);
}


static void pin_wkup_hw_init(void)
{
#if USER_USE_RTTHREAD == 1
    rt_uint32_t wkup_pin = 0;

    switch (WKUP_USE_PIN) {
    case PWR_WAKEUP_PIN1:
        wkup_pin = GET_PIN(A, 0);
        break;
    case PWR_WAKEUP_PIN3:
        wkup_pin = GET_PIN(C, 13);
        break;
    default:
        break;
    }
#else
    GPIO_TypeDef* wkup_port = { 0 };

    switch (WKUP_USE_PIN) {
    case PWR_WAKEUP_PIN1:
        wkup_port = GPIOA;
        wkup_pin = GPIO_PIN_0;
        break;
    case PWR_WAKEUP_PIN3:
        wkup_port = GPIOC;
        wkup_pin = GPIO_PIN_13;
        break;
    default:
        break;
    }
#endif


#if USER_USE_RTTHREAD == 1
    rt_pin_mode(GET_PIN(E, 3), PIN_MODE_OUTPUT);
    rt_pin_attach_irq(wkup_pin, PIN_IRQ_MODE_RISING, pin_callback, RT_NULL);
    rt_pin_irq_enable(wkup_pin, PIN_IRQ_ENABLE);
#else
    /*  */
#endif 
}

int pin_wkup_init(void)
{
#if USER_USE_RTTHREAD == 1

    wkup_mb = rt_mb_create("wkup_mb", (4 * 8), RT_IPC_FLAG_FIFO);
    if (wkup_mb == RT_NULL) {
        rt_kprintf("Create mailbox wkup_mb error.\n");
        return -1;
    }

    sys_wkup_th = rt_thread_create(
        PIN_WKUP_THREAD_NAME,
        sys_wkup,
        RT_NULL,
        PIN_WKUP_THREAD_STACK_SIZE,
        PIN_WKUP_THREAD_PRIORITY,
        PIN_WKUP_THREAD_SLICE
    );
    if (sys_wkup_th != RT_NULL) {
        rt_err_t err = rt_thread_startup(sys_wkup_th);
        if (err != RT_EOK) {
            rt_kprintf("Create thread '%s' error.\n", sys_wkup_th->name);
            return -1;
        }
    } else {
        return -1;
    }
#endif

    pin_wkup_hw_init();

    return 0;
}
#if USER_USE_RTTHREAD == 1
INIT_APP_EXPORT(pin_wkup_init);
#endif
