/***********************************************************************************

 * 文 件 名   : li_timer.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月23日
 * 版 本 号   : 
 * 文件描述   : 封装定时器
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include <time.h>
#ifdef USE_DPDK_LIB
#include "rte_lcore.h"
#include "rte_cycles.h"
#include "rte_timer.h"
#elif defined(USE_DRIVE_LIB)
#include "drive_if.h"
#else
#include <signal.h>
#endif
#include "li_timer.h"

#ifdef USE_DPDK_LIB /* 使用DPDK库的定时器 */

/*****************************************************************************
 * 函 数 名  : li_print_timer
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年5月2日
 * 函数功能  : 打印DPDK定时器
 * 输入参数  : IN int level          打印级别
               IN LI_TIMER_T *timer  定时器
 * 输出参数  : 无
 * 返 回 值  : void                  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_print_timer(IN int level, IN LI_TIMER_T *timer)
{
    char buf[255];
    size_t i = 0;
    if(NULL != timer)
    {
        snprintf(buf, sizeof(buf), "DPDK timer={expire=%"PRIu64", sl_next={", timer->expire);
        for(i = 0; i < sizeof(timer->sl_next) / sizeof(timer->sl_next[0]) - 1; i++)
        {
            snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%p,", timer->sl_next[i]);
        }
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%p}", timer->sl_next[i]);
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
            ",status={state=%u,owner=%d},period=%"PRIu64",f=%p,arg=%p}",
            timer->status.state, timer->status.owner, timer->period, timer->f, timer->arg);
        LI_LOG(level, "%s\n", buf);
    }
    else
    {
        LI_LOG(level, "NULL tiemr\n");
    }

    return;
}

/*****************************************************************************
 * 函 数 名  : li_start_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月28日
 * 函数功能  : 启动DPDK定时器
 * 输入参数  : INOUT LI_TIMER_T *timer              定时器
               IN struct itimerspec *time_interval  时间间隔(it_value:第一次时间间隔，
                                                             it_interval:以后的时间间隔，为0表示定时器只运行一次)
               IN li_timer_callback func            回调函数
               IN void *func_arg                    回调函数参数
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                            错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_start_timer(INOUT LI_TIMER_T *timer,
    IN struct itimerspec *time_interval,
    IN li_timer_callback func,
    IN void *func_arg)
{
    LI_RESULT ret = CFG_OK;
    int result = 0;

    uint64_t hz = rte_get_timer_hz();   /* 1秒 */
    unsigned lcore_id = rte_get_master_lcore(); //rte_lcore_id(); //LCORE_ID_ANY
    enum rte_timer_type interval_type = SINGLE;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, time_interval, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func_arg, ret);

        if(0 == timer->status.u32)
        {
            rte_timer_init(timer);
        }

        if((0 == time_interval->it_interval.tv_sec) && (0 == time_interval->it_interval.tv_nsec))
        {
            /* 加上double是为了防止数值太大乘越界了 */
            hz = (double)hz * time_interval->it_value.tv_nsec / 1000000000 + hz * time_interval->it_value.tv_sec;
            interval_type = SINGLE;
        }
        else
        {
            /* 加上double是为了防止数值太大乘越界了 */
            hz = (double)hz * time_interval->it_interval.tv_nsec / 1000000000 + hz * time_interval->it_interval.tv_sec;
            interval_type = PERIODICAL;    /* 周期运行 */
        }

        result = rte_timer_reset(timer, hz, interval_type, lcore_id, func, func_arg);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, rte_timer_reset, result, "%d", ret);
    }while(0);
    
    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_stop_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年5月2日
 * 函数功能  : 关闭dpdk定时器
 * 输入参数  : IN LI_TIMER_T * timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT              错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_stop_timer(IN LI_TIMER_T *timer)
{
    LI_RESULT ret = CFG_OK;

    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);

        rte_timer_stop(timer);
    } while ( 0 ); /* 循环直到0不成立 */
        
    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_destroy_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月11日
 * 函数功能  : 删除DPDK定时器
 * 输入参数  : INOUT LI_TIMER_T *timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_destroy_timer(INOUT LI_TIMER_T *timer)
{
    return CFG_OK;
}

#elif defined(USE_DRIVE_LIB)    /* 使用驱动模块封装的DPDK库定时器 */

/*****************************************************************************
 * 函 数 名  : li_print_timer
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年4月21日
 * 函数功能  : 打印驱动封装的DPDK定时器
 * 输入参数  : IN int level          打印级别
               IN LI_TIMER_T *timer  定时器
 * 输出参数  : 无
 * 返 回 值  : void                  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_print_timer(IN int level, IN LI_TIMER_T *timer)
{
    if(NULL != timer)
    {
        LI_LOG(level, "driver timerid=%d\n", *timer);
    }
    else
    {
        LI_LOG(level, "NULL tiemr\n");
    }

    return;
}

/*****************************************************************************
 * 函 数 名  : li_start_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月28日
 * 函数功能  : 启动驱动封装的DPDK定时器
 * 输入参数  : INOUT LI_TIMER_T *timer              定时器
               IN struct itimerspec *time_interval  时间间隔(it_value:第一次时间间隔，
                                                             it_interval:以后的时间间隔，为0表示定时器只运行一次)
               IN li_timer_callback func            回调函数
               IN void *func_arg                    回调函数参数
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                            错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_start_timer(INOUT LI_TIMER_T *timer,
    IN struct itimerspec *time_interval,
    IN li_timer_callback func,
    IN void *func_arg)
{
    LI_RESULT ret = CFG_OK;
    int result = 0;

    unsigned int peroid_us = 0;     /* 单位微妙 */
    int interval_type = DRIVE_TIMER_PEROID_SINGLE;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, time_interval, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func_arg, ret);

        if(0 == *timer)
        {
            result = ApiTimerInit(timer);
            LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, ApiTimerStart, result, "%d", ret)
        }

        if((0 == time_interval->it_interval.tv_sec) && (0 == time_interval->it_interval.tv_nsec))
        {
            /* 加上double是为了防止数值太大乘越界了 */
            peroid_us = (double)time_interval->it_value.tv_nsec / 1000 + time_interval->it_value.tv_sec * 1000000;
            interval_type = DRIVE_TIMER_PEROID_SINGLE;
        }
        else
        {
            /* 加上double是为了防止数值太大乘越界了 */
            peroid_us = (double)time_interval->it_interval.tv_nsec / 1000 + time_interval->it_interval.tv_sec * 1000000;
            interval_type = DRIVE_TIMER_PEROID_PERIODICAL;    /* 周期运行 */
        }

        result = ApiTimerStart(*timer, peroid_us, interval_type, func, func_arg);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, ApiTimerStart, result, "%d", ret);
    }while(0);
    
    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_stop_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年5月2日
 * 函数功能  : 关闭驱动封装的DPDK定时器
 * 输入参数  : IN LI_TIMER_T * timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT              错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_stop_timer(IN LI_TIMER_T *timer)
{
    LI_RESULT ret = CFG_OK;

    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);

        ApiTimerStop(*timer);
    } while ( 0 ); /* 循环直到0不成立 */
        
    return ret;
}


/*****************************************************************************
 * 函 数 名  : li_destroy_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月11日
 * 函数功能  : 删除驱动封装的DPDK定时器
 * 输入参数  : INOUT LI_TIMER_T *timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_destroy_timer(INOUT LI_TIMER_T *timer)
{
    ApiTimerDestroy(*timer);
    return CFG_OK;
}

#else   /* 使用POSIX定时器 */

/*****************************************************************************
 * 函 数 名  : li_print_timer
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年4月21日
 * 函数功能  : 打印POSIX定时器
 * 输入参数  : IN int level          打印级别
               IN LI_TIMER_T *timer  定时器
 * 输出参数  : 无
 * 返 回 值  : void                  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_print_timer(IN int level, IN LI_TIMER_T *timer)
{
    if(NULL != timer)
    {
        #ifdef linux
        LI_LOG(level, "POSIX timerid=%p\n", *timer);
        #else
        LI_LOG(level, "windows timerid=%#lx\n", *timer);
        #endif
    }
    else
    {
        LI_LOG(level, "NULL tiemr\n");
    }

    return;
}

/*****************************************************************************
 * 函 数 名  : li_start_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月28日
 * 函数功能  : 启动POSIX定时器
 * 输入参数  : INOUT LI_TIMER_T *timer              定时器
               IN struct itimerspec *time_interval  时间间隔(it_value:第一次时间间隔，
                                                             it_interval:以后的时间间隔，为0表示定时器只运行一次)
               IN li_timer_callback func            回调函数
               IN void *func_arg                    回调函数参数
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                            错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_start_timer(INOUT LI_TIMER_T *timer,
    IN struct itimerspec *time_interval,
    IN li_timer_callback func,
    IN void *func_arg)
{
    LI_RESULT ret = CFG_OK;
    int result = 0;
    
    struct sigevent evp;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, time_interval, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, func_arg, ret);
        
        ret = li_stop_timer(timer);   /* 如果启动过的定时器先停掉 */
        LI_CHECK_FUNC_RETURN_BREAK(BMU_WARNING, li_stop_timer, ret);

        if(0 == *timer)        /* 防止反复调用timer_create导致内存溢出 */
        {
            OS_MEMSET(&evp, 0, sizeof(evp));       //清零初始化  

            evp.sigev_notify = SIGEV_THREAD;                /* 线程通知的方式，派驻新线程 */
            evp.sigev_notify_function = (void (*)(union sigval))func;    /* 线程函数地址 */
            evp.sigev_value.sival_ptr = func_arg;     /* 线程参数 */
            
            /* TODO: 设置线程属性 */
          
            result = timer_create(CLOCK_REALTIME, &evp, timer);  /* 创建一个实时定时器 */
            LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, timer_create, result, "%d", ret);
        }

        result = timer_settime(*timer, 0, time_interval, NULL);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, timer_settime, result, "%d", ret);
    }while(0);

    return ret;
}


/*****************************************************************************
 * 函 数 名  : li_stop_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 关闭POSIX定时器
 * 输入参数  : IN LI_TIMER_T * timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT              错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_stop_timer(IN LI_TIMER_T *timer)
{
    LI_RESULT ret = CFG_OK;
    int result = 0;
    
    struct itimerspec time_interval;

    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);
        
        if(0 != *timer)        /* 防止未创建定时器 */
        {
            /* 给入的时间为0表示停止定时器 */
            OS_MEMSET(&time_interval, 0, sizeof(time_interval));
            result = timer_settime(*timer, 0, &time_interval, NULL);
            LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, timer_settime, result, "%d", ret);
        }
    } while ( 0 ); /* 循环直到0不成立 */

    return ret;
}


/*****************************************************************************
 * 函 数 名  : li_destroy_timer
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月11日
 * 函数功能  : 删除POSIX定时器
 * 输入参数  : INOUT LI_TIMER_T *timer  定时器指示器
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_destroy_timer(INOUT LI_TIMER_T *timer)
{
    LI_RESULT ret = CFG_OK;
    int result = 0;
    
    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, timer, ret);
        
        if(0 != *timer)        /* 防止反复调用timer_delete导致程序奔溃 */
        {
            result = timer_delete(*timer);
            *timer = 0;     /* 停止后回收，反复调用timer_delete会导致程序奔溃 */
            LI_CHECK_EXTERN_FUNC_RETURN_BREAK(BMU_WARNING, timer_delete, result, "%d", ret);
        }
    } while ( 0 ); /* 循环直到0不成立 */

    return ret;
}

#endif

/*****************************************************************************
 * 函 数 名  : li_transfer_us_to_timespec
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月11日
 * 函数功能  : 将微秒翻译成时间间隔
 * 输入参数  : IN uint32_t ums                     微秒
 * 输出参数  : OUT struct timespec *time_interval  时间间隔
 * 返 回 值  : LI_RESULT                           错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_transfer_us_to_timespec(IN uint32_t us, OUT struct timespec *time_interval)
{
    LI_RESULT ret = CFG_OK;

    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, time_interval, ret);

        time_interval->tv_sec = us / 1000000;
        time_interval->tv_nsec = us % 1000000 * 1000;
    } while ( 0 ); /* 循环直到0不成立 */

    return ret;
}

