/************************************************
 * @file pro_conf.h
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright (c) 2020
 *
 ************************************************/
#ifndef PRO_CONF_H
#define PRO_CONF_H

#ifdef __cplusplus
extern "C" {
#endif



#define USER_USE_OS         1u

/* USER USE OS */
#if USER_USE_OS == (1u)
#define USER_USE_RTTHREAD   1u
#if USER_USE_RTTHREAD == (1u)
#include <rtthread.h>
#define RT_OS_INT_ENTER     rt_interrupt_enter
#define RT_OS_INT_EXIT      rt_interrupt_leave
#define OS_INT_ENTER        RT_OS_INT_ENTER
#define OS_INT_EXIT         RT_OS_INT_EXIT
#elif USER_USE_UCOS == (1u)
#define RT_OS_INT_ENTER     
#define RT_OS_INT_EXIT      
#define OS_INT_ENTER        
#define OS_INT_ENTER        
#endif


#define RT_PRINT        rt_kprintf
#define RT_DELAY        rt_thread_mdelay
#define RT_STRCMP       rt_strcmp
#define RT_STRCAT       rt_strcat
#define RT_STRLEN       rt_strlen
#define RT_SPRINTF      rt_sprintf
#define RT_MALLOC       rt_malloc
#define RT_FREE         rt_free
#define RT_MEMSET       rt_memset
#define RT_MEMCMP       rt_memcmp
#define RT_MEMCPY       rt_memcpy


#define DEBUG_PRINT     RT_PRINT
#define DELAY           RT_DELAY
#define SPRINTF         RT_SPRINTF
#define STRCMP          RT_STRCMP
#define STRCAT          RT_STRCAT
#define STRLEN          RT_STRLEN
#define MALLOC          RT_MALLOC
#define FREE            RT_FREE
#define MEMSET          RT_MEMSET
#define MEMCMP          RT_MEMCMP
#define MEMCPY          RT_MEMCPY
#else
#define SECTION(x)      __attribute__((section(x)))
#define DEBUG_PRINT     printf
#define DELAY           HAL_Delay
#define STRCMP          strcmp
#define STRCAT          strcat
#define STRLEN          strlen
#define MALLOC          malloc
#define FREE            free
#define MEMSET          memset
#define MEMCMP          memcmp
#endif



#ifdef __cplusplus
}
#endif

#endif

