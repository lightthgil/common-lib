/***********************************************************************************

 * 文 件 名   : fpgasim_test_code.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : fpgasim_test_code.c 的头文件
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __FPGASIM_TEST_CODE_H__
#define __FPGASIM_TEST_CODE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/********* 标准头文件加在以下行↓ **************/

/********* 标准头文件加在以上行↑ **************/

/********* 外部模块头文件加在以下行↓ **************/
#include "test_common.h"
#include "test_sim.h"
#ifdef USE_DPDK_LIB
#include "test_dpdk.h"
#endif

/********* 外部模块头文件加在以上行↑ **************/

/********* 模块内部头文件加在以下行↓ **************/
#include "proc_main.h"

/********* 模块内部头文件加在以上行↑ **************/

/********* 单元自测头文件加在以下行↓ **************/
/********* 单元自测头文件加在以上行↑ **************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __FPGASIM_TEST_CODE_H__ */
