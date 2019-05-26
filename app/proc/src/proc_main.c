/***********************************************************************************

 * 文 件 名   : proc_main.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "fpgasim_interface.h"
#include "test_common_main.h"
#include "test_sim_main.h"
#include "proc_main.h"

/*****************************************************************************
 * 函 数 名  : main
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月14日
 * 函数功能  : 单元测试入口函数
 * 输入参数  : int argc      参数格式
               char *argv[]  参数值
 * 输出参数  : 无
 * 返 回 值  : int           返回值
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int main(int argc, char *argv[])
{
    int ret = 0;

    fpgasim_init();

    do /* 开始循环 */
    {        
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
        test_common_main();     /* 测试通用模块 */
        test_sim_main();        /* 测试仿真模块 */
        #ifdef USE_DPDK_LIB
        test_dpdk_main();       /* 测试dpdk库 */
        #endif
        
        /*************** 在该行以上加入自己的调试函数 *******************/

//        #endif

    } while ( 0 ); /* 循环直到0不成立 */
    
    return ret;
}
