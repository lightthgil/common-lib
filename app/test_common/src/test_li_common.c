/***********************************************************************************

 * 文 件 名   : test_li_common.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "li_common.h"

#include "test_li_common.h"

/*****************************************************************************
 * 函 数 名  : test_common_endian
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月14日
 * 函数功能  : 测试大小端
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_endian()
{
    uint8_t buf[20] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 
        0x11, 0x12, 0x13, 0x14};
    uint8_t *ptr_buf = buf;
    uint32_t val32_network = 0;
    uint32_t val32_host = 0;
    uint32_t long_val = 0;
    uint32_t short_val = 0;
    uint32_t residual_length = 0;

    LI_LOG(LI_SHELL_NOTICE, "****************************测试大小端***************************\n");

    val32_network = *((uint32_t *)(ptr_buf));
    val32_host = LI_HTONL(val32_network);
    LI_LOG(LI_SHELL_INFO, "val32_network:%#x\n", val32_network);
    
//    LI_LOG(LI_SHELL_INFO, "val32_host:%#x\n", val32_host);
    LI_UNIT_TEST_ASSERT(val32_host == 0x01020304);
    
    residual_length = sizeof(buf);
    LI_GET_ULONG(long_val, ptr_buf, residual_length);
    LI_GET_USHORT(short_val, ptr_buf, residual_length);
    
//    LI_LOG(LI_SHELL_INFO, "long_val:%#x\n", long_val);
//    LI_LOG(LI_SHELL_INFO, "short_val:%#x\n", short_val);
    LI_UNIT_TEST_ASSERT(long_val == 0x01020304);
    LI_UNIT_TEST_ASSERT(short_val == 0x0506);
    
    return;
}

/*****************************************************************************
 * 函 数 名  : test_common_log
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月15日
 * 函数功能  : 测试打印
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_log()
{
    LI_LOG(LI_SHELL_NOTICE, "******************************测试打印***************************\n");
    LI_LOG(LI_SHELL_CRIT, "LI_SHELL_CRIT\n");
    LI_LOG(LI_SHELL_ERR, "LI_SHELL_ERR\n");
    LI_LOG(LI_SHELL_WARNING, "LI_SHELL_WARNING\n");
    LI_LOG(LI_SHELL_NOTICE, "LI_SHELL_NOTICE\n");
    LI_LOG(LI_SHELL_INFO, "LI_SHELL_INFO\n");
    LI_LOG(LI_SHELL_DEBUG, "LI_SHELL_DEBUG\n");
    LI_LOG(BMU_CRIT, "BMU_CRIT\n");
    LI_LOG(BMU_ERR, "BMU_ERR\n");
    LI_LOG(BMU_WARNING, "BMU_WARNING\n");
    LI_LOG(BMU_NOTICE, "BMU_NOTICE\n");
    LI_LOG(BMU_INFO, "BMU_INFO\n");
    li_set_debug(BMU_DEBUG, BMU_DEBUG, true);   /* 降低打印级别 */
    LI_LOG(BMU_DEBUG, "BMU_DEBUG\n");
    li_set_debug(BMU_INFO, BMU_INFO, true);   /* 恢复打印级别 */

    return;
}

/*****************************************************************************
 * 函 数 名  : test_str_result
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月23日
 * 函数功能  : 测试错误码转字符串
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_str_result()
{
    LI_LOG(LI_SHELL_NOTICE, "***************************测试错误码转字符串********************\n");

    LI_UNIT_TEST_ASSERT(0 == strcmp("CFG_OK", str_result(CFG_OK)));
    LI_UNIT_TEST_ASSERT(0 == strcmp("CFG_RESULT_MAX", str_result(CFG_RESULT_MAX)));
    LI_UNIT_TEST_ASSERT(0 == strcmp("FORCED_END", str_result(FORCED_END)));
    LI_UNIT_TEST_ASSERT(0 == strcmp("unknown", str_result((LI_RESULT)(CFG_RESULT_MAX + 1))));
    LI_UNIT_TEST_ASSERT(0 == strcmp("unknown", str_result((LI_RESULT) - 1)));

    return;
}
