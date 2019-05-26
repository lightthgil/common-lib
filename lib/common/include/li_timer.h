/***********************************************************************************

 * 文 件 名   : li_timer.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月23日
 * 版 本 号   : 
 * 文件描述   : li_timer.c 的头文件
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __LI_TIMER_H__
#define __LI_TIMER_H__

#include "li_common.h"
#ifdef USE_DPDK_LIB
#include "rte_timer.h"
#elif defined(USE_DRIVE_LIB)
#include "drive_if.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/* 暂时没想到在源文件中实现区分的办法，因为DPDK定时器和POSIX定时的回调函数不一样 */
#ifdef USE_DPDK_LIB
typedef struct rte_timer    LI_TIMER_T;
#define TIMER_PARAM_T __attribute__((unused))LI_TIMER_T*
#define TIMER_PARAM  timer,
#elif defined(USE_DRIVE_LIB)
typedef int    LI_TIMER_T;
#define TIMER_PARAM_T __attribute__((unused))void*
#define TIMER_PARAM timer,
#else
typedef timer_t    LI_TIMER_T;
#define TIMER_PARAM_T
#define TIMER_PARAM
#endif

typedef void (*li_timer_callback)(TIMER_PARAM_T TIMER_PARAM void *arg);

typedef struct li_timer_func
{
    li_timer_callback func;     /* 回调函数 */
    void *arg;                  /* 回调函数的参数 */
}LI_TIMER_FUNC;


extern LI_RESULT li_destroy_timer(INOUT LI_TIMER_T *timer);
extern void li_print_timer(IN int level, IN LI_TIMER_T *timer);
extern LI_RESULT li_start_timer(INOUT LI_TIMER_T *timer,
             IN struct itimerspec *time_interval,
             IN li_timer_callback func,
             IN void *func_arg);
extern LI_RESULT li_stop_timer(IN LI_TIMER_T *timer);
extern LI_RESULT li_transfer_us_to_timespec(IN uint32_t us, OUT struct timespec *time_interval);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LI_TIMER_H__ */
