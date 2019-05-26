/***********************************************************************************

 * 文 件 名   : test_common_main.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "test_li_common.h"
#include "test_common_timer.h"
#include "test_common_hash.h"
#include "test_common_main.h"

/*****************************************************************************
 * 函 数 名  : test_common_init
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 加载动态库用
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_init()
{
    return;
}

/*****************************************************************************
 * 函 数 名  : test_common_main
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月14日
 * 函数功能  : 单元测试入口函数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void           无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_main()
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
        test_common_log();          /* 测试打印 */
        test_common_endian();       /* 测试大小端 */
        test_str_result();          /* 测试错误码转字符串 */
        test_common_timer();        /* 测试定时器 */
        test_common_hash();         /* 测试通用哈希表 */
        
        /*************** 在该行以上加入自己的调试函数 *******************/

//        #endif
    } while ( 0 ); /* 循环直到0不成立 */
    
    return;
}

