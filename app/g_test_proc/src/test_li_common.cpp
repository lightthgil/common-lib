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

#include "test_common.h"
#include "proc.h"
#include "gtest/gtest.h"

TEST(common_endian, all)
{
    uint8_t buf[20] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 
        0x11, 0x12, 0x13, 0x14};
    uint8_t *ptr_buf = buf;
    uint32_t long_val = 0;
    uint32_t short_val = 0;
    uint32_t residual_length = sizeof(buf);
    uint32_t expect_long = 0x01020304;
    uint16_t expect_short = 0x0506;

    EXPECT_EQ(expect_long, LI_HTONL(*((uint32_t *)(ptr_buf))));
    
    LI_GET_ULONG(long_val, ptr_buf, residual_length);
    LI_GET_USHORT(short_val, ptr_buf, residual_length);
    
    EXPECT_EQ(expect_long, long_val);
    EXPECT_EQ(expect_short, short_val);
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

    return;
}

TEST(str_result, normal_input)
{
    EXPECT_EQ(std::string("CFG_OK"), std::string(str_result(CFG_OK)));
    EXPECT_EQ(std::string("FORCED_END"), std::string(str_result(FORCED_END)));
}

TEST(str_result, max_input)
{
    EXPECT_EQ(std::string("CFG_RESULT_MAX"), std::string(str_result(CFG_RESULT_MAX)));
}

TEST(str_result, overlord_input)
{
    EXPECT_EQ(std::string("unknown"), std::string(str_result((LI_RESULT)(CFG_RESULT_MAX + 1))));
}


TEST(str_result, zero_input)
{
    EXPECT_EQ(std::string("unknown"), std::string(str_result((LI_RESULT) - 1)));
}

