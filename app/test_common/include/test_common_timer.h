/***********************************************************************************

 * 文 件 名   : test_common_timer.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月24日
 * 版 本 号   : 
 * 文件描述   : test_common_timer.c 的头文件
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __TEST_COMMON_TIMER_H__
#define __TEST_COMMON_TIMER_H__

#include "li_timer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define TEST_COMMON_TIMER_INTERVAL_SECOND      (1)      /* 定时器的间隔时间(秒) */
#define TEST_COMMON_TIMER_INTERVAL_USECOND     (0)      /* 定时器的间隔时间(微妙) */


extern void test_common_timer();
extern void test_common_timer_callback(TIMER_PARAM_T TIMER_PARAM void *arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TEST_COMMON_TIMER_H__ */
