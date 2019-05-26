/***********************************************************************************

 * 文 件 名   : test_common_hash.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年4月5日
 * 版 本 号   : 
 * 文件描述   : test_common_hash.c 的头文件
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __TEST_COMMON_HASH_H__
#define __TEST_COMMON_HASH_H__

#include "li_hash.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


typedef struct test_hash_uint32
{
    LI_HASH_CB hash_cb;
    uint32_t  key;
    uint32_t  data;
}TEST_HASH_UINT32;


typedef struct test_hash_string
{
    LI_HASH_CB hash_cb;
    uint32_t  key;
    #define TEST_HASH_STRING_MANE   20
    char name[TEST_HASH_STRING_MANE];
}TEST_HASH_STRING;



extern void test_common_destroy_hash_node(IN LI_HASH_ROOT_CB *root_cb, IN LI_HASH_CB *node);
extern LI_RESULT test_common_hash();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TEST_COMMON_HASH_H__ */
