/***********************************************************************************

 * 文 件 名   : test_dpdk_timer.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "rte_lcore.h"
#include "rte_cycles.h"
#include "rte_timer.h"
#include "li_common.h"
#include "test_dpdk_timer.h"

extern void eal_thread_init_master(unsigned lcore_id);

/*****************************************************************************
 * 函 数 名  : test_dpdk_timer_callback
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月21日
 * 函数功能  : 测试DPDK定时器的回调函数
 * 输入参数  : struct rte_timer *timer  定时器
               void *arg                定时器传入的参数
 * 输出参数  : 无
 * 返 回 值  : void                     无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_dpdk_timer_callback(struct rte_timer *timer,void *arg)
{
    LI_LOG(LI_SHELL_NOTICE, "timer call back\n");
    return;
}

/*****************************************************************************
 * 函 数 名  : test_dpdk_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月20日
 * 函数功能  : 测试DPDK的定时器
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_dpdk_timer()
{
    struct rte_timer timer;
    uint64_t hz = rte_get_timer_hz();   /* 1秒 */
    unsigned lcore_id = rte_get_master_lcore(); //rte_lcore_id(); //LCORE_ID_ANY
    __attribute__((unused)) LI_RESULT ret = CFG_OK;
    int result = 0;

    do
    {
        LI_LOG(LI_SHELL_NOTICE, "***************************测试dpdk定时器************************\n");

        rte_timer_init(&timer);
        result = rte_timer_reset(&timer, hz, PERIODICAL, lcore_id, test_dpdk_timer_callback, NULL);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(LI_SHELL_WARNING, rte_timer_reset, result, "%d", ret);
    }while(0);

    return;
}

/*****************************************************************************
 * 函 数 名  : test_lcore_timer_loop
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年4月26日
 * 函数功能  : DPDK要求有个线程专门轮序调用rte_timer_manage，这个是线程的回调函数
 * 输入参数  : __attribute__((unused)) void *arg  参数
 * 输出参数  : 无
 * 返 回 值  : void*                              无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void* test_lcore_timer_loop(__attribute__((unused)) void *arg)
{
    uint64_t prev_tsc = 0, cur_tsc, diff_tsc;
    unsigned lcore_id = 0;
    
    /* around 100ms at 2 Ghz */
#define TIMER_RESOLUTION_CYCLES           200000000ULL

    lcore_id = rte_lcore_id();
    eal_thread_init_master(lcore_id);

    LI_LOG(BMU_NOTICE, "Starting test dpdk timer loop on core %u\n", lcore_id);


    while (1)
    {
#if 1

        /*
         * Call the timer handler on each core: as we don't
         * need a very precise timer, so only call
         * rte_timer_manage() every ~10ms (at 2Ghz). In a real
         * application, this will enhance performances as
         * reading the HPET timer is not efficient.
         */
        cur_tsc = rte_rdtsc();
        diff_tsc = cur_tsc - prev_tsc;
        if (diff_tsc > TIMER_RESOLUTION_CYCLES)
        {
            rte_timer_manage();
            prev_tsc = cur_tsc;
        }
#else
        rte_timer_manage();
        rte_delay_ms(10);
#endif
    }

    return NULL;
}

/*****************************************************************************
 * 函 数 名  : test_dpdk_create_timer_loop_thread
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年4月26日
 * 函数功能  : 创建轮询调用rte_timer_manage函数的线程
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_dpdk_create_timer_loop_thread()
{
    pthread_t thread_id;
    int result = 0;
    
    result = pthread_create(&thread_id, NULL, test_lcore_timer_loop, NULL);
    LI_CHECK_EXTERN_FUNC_RETURN_NO_BREAK(BMU_ERR, pthread_create, result, "%d");

    return;
}

