/***********************************************************************************

 * 文 件 名   : test_dpdk_timer.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月22日
 * 版 本 号   : 
 * 文件描述   : test_dpdk_timer.c 的头文件
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __TEST_DPDK_TIMER_H__
#define __TEST_DPDK_TIMER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

extern void test_dpdk_create_timer_loop_thread();
extern void test_dpdk_timer();
extern void test_dpdk_timer_callback(struct rte_timer *timer,void *arg);
extern void* test_lcore_timer_loop(__attribute__((unused)) void *arg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TEST_DPDK_TIMER_H__ */
