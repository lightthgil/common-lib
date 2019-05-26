/***********************************************************************************

 * 文 件 名   : test_common_timer.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月24日
 * 版 本 号   : 
 * 文件描述   : 测试通用定时器
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include <time.h>
#include <unistd.h>
#include "test_common_timer.h"

/*****************************************************************************
 * 函 数 名  : test_common_timer_callback
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 测试通用定时器的回调函数
 * 输入参数  : void *arg                定时器传入的参数
 * 输出参数  : 无
 * 返 回 值  : void                     无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_timer_callback(TIMER_PARAM_T TIMER_PARAM void *arg)
{
    struct timespec *old_time;
    struct timespec now;
    double time_diff = 0.0;

    clock_gettime(CLOCK_REALTIME, &now);

    old_time = (struct timespec *)arg;

    time_diff = difftime(now.tv_sec, old_time->tv_sec);
    time_diff = (time_diff * 1000000000 + now.tv_nsec - old_time->tv_nsec) / 1000000000;

    OS_MEMCPY(old_time, &now, sizeof(*old_time));       /* 赋值为新的时间，方便下次定时器回调的时候使用 */
    
    LI_LOG(LI_SHELL_INFO, "timer call back use %f s\n", time_diff);
    LI_UNIT_TEST_ASSERT(time_diff < TEST_COMMON_TIMER_INTERVAL_SECOND * 3);
    return;
}


/*****************************************************************************
 * 函 数 名  : test_common_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 测试通用定时器
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_timer()
{
    LI_TIMER_T timer;
    struct itimerspec time_interval;
    static struct timespec s_now;
    
    LI_LOG(LI_SHELL_NOTICE, "***************************测试通用定时器************************\n");

    OS_MEMSET(&timer, 0, sizeof(timer));

    clock_gettime(CLOCK_REALTIME, &s_now);

    time_interval.it_value.tv_sec = TEST_COMMON_TIMER_INTERVAL_SECOND;       /* 第一次时间间隔 */
    time_interval.it_value.tv_nsec = TEST_COMMON_TIMER_INTERVAL_USECOND;     /* 第一次时间间隔 */
    time_interval.it_interval.tv_sec = 0;          /* 以后的时间间隔 */ /* 如果定时器要执行多次，则赋值为非0 */
    time_interval.it_interval.tv_nsec = 0;        /* 以后的时间间隔 */  /* 如果定时器要执行多次，则赋值为非0 */
    li_start_timer(&timer, &time_interval, test_common_timer_callback, &s_now);

    sleep(TEST_COMMON_TIMER_INTERVAL_SECOND * 2);                   /* 等待时间让定时器执行 */

    li_stop_timer(&timer);
    
//    sleep(TEST_COMMON_TIMER_INTERVAL_SECOND);                   /* 等待时间让定时器回调函数打印完 */

    return;
}

