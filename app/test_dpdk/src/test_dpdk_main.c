/***********************************************************************************

 * 文 件 名   : test_dpdk_main.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "test_dpdk_hash.h"
#include "test_dpdk_timer.h"
#include "test_dpdk_main.h"

/*****************************************************************************
 * 函 数 名  : test_dpdk_init
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 加载动态库用
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_dpdk_init()
{
    //rte_timer_subsystem_init();   /* 由主线程调用一次 */
    
    //test_dpdk_create_timer_loop_thread(); /* 驱动有在CPU主核调用轮询timer，只当关联到其他无timer管理的核时使用 */
    return;
}

/*****************************************************************************
 * 函 数 名  : test_dpdk_main
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月14日
 * 函数功能  : 单元测试入口函数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void           无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_dpdk_main()
{
    do /* 开始循环 */
    {        
//        rte_eal_init(argc, argv);

        
        /* 使用BMU的库，可以在x86环境的linux上实现类似设备的效果 */
//        #ifdef USE_BMU_LIB
//            ret = BmuLibInit(0);
//            LI_CHECK_FUNC_RETURN_BREAK(LI_SHELL_WARNING, BmuLibInit, ret);
//            ret = CliCmdInit(argv[0], "eth1", 9002);
//            LI_CHECK_FUNC_RETURN_BREAK(LI_SHELL_WARNING, CliCmdInit, ret);
//            
//            while(1)
//            {
//                sleep(1000);
//            }
//        #else
        
        /*************** 在该行以下加入自己的调试函数 *******************/
        test_dpdk_hash();       /* 测试DPDK库提供的哈希表 */
        test_dpdk_timer();      /* 测试DPDK提供的定时器 */
        
        /*************** 在该行以上加入自己的调试函数 *******************/

//        #endif
    } while ( 0 ); /* 循环直到0不成立 */
    
    return;
}

