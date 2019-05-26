/***********************************************************************************

 * 文 件 名   : test_dpdk_hash.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : 测着玩，验证功能
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "rte_errno.h"
#ifdef RTE_MACHINE_CPUFLAG_SSE4_2
#include <rte_hash_crc.h>
#define DEFAULT_HASH_FUNC       rte_hash_crc
#else
#include <rte_jhash.h>
#define DEFAULT_HASH_FUNC       rte_jhash
#endif
#include "rte_hash.h"
#include "test_dpdk_hash.h"

/*****************************************************************************
 * 函 数 名  : test_dpdk_hash
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月20日
 * 函数功能  : 测试DPDK的哈希表功能
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : RESULT_FPGASIM错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT test_dpdk_hash()
{
    LI_RESULT ret = CFG_OK;
    int  result = 0;
    struct rte_hash *hash_tabel = NULL;
    rte_hash_parameters hash_params;
    const int32_t hash_length = 1024;
    int32_t i = 0;
    int32_t hash_key = 0;
    int32_t *hash_data_ptr = NULL;
    int32_t hash_data = 0;

    do
    {
        LI_LOG(LI_SHELL_NOTICE, "**************************测试dpdk哈希表*********************\n");

        OS_MEMSET(&hash_params, 0, sizeof(hash_params));
        
        hash_params.name = "test_hash";
        hash_params.entries = hash_length << 2;
        hash_params.key_len = sizeof(hash_key);
        hash_params.hash_func = DEFAULT_HASH_FUNC;
        hash_params.hash_func_init_val = 0;

        /* DPDK建立哈希表后，没有销毁接口，所以哈希表会残留，为保证健壮性，此处需要判断哈希表是否建立过 */
        hash_tabel = rte_hash_find_existing(hash_params.name);
        if(NULL == hash_tabel)
        {
            hash_tabel = rte_hash_create(&hash_params);
            if(NULL == hash_tabel)
            {            
                LI_LOG(LI_SHELL_ERR, "rte_hash_create return:%s\n", rte_strerror(rte_errno));
            }
            LI_CHECK_NULL_FUNC_BREAK(LI_SHELL_WARNING, rte_hash_create, hash_tabel, ret);
        }

        for (i = 1; i <= hash_length; i++) /* 对变量i进行循环 */
        {
            hash_data = i * 10;
            result = rte_hash_add_key_data(hash_tabel, &i, &hash_data);
            LI_CHECK_EXTERN_FUNC_RETURN_BREAK(LI_SHELL_WARNING, rte_hash_add_key_data, result, "%d", ret);
        }
        LI_CHECK_RETURN(ret);

        hash_key = 1;
        result = rte_hash_lookup_data(hash_tabel, &hash_key, (void **)&hash_data_ptr);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(LI_SHELL_WARNING, rte_hash_lookup_data, result, "%d", ret);
        LI_UNIT_TEST_ASSERT(*hash_data_ptr == hash_key * 10);
        
        hash_key = 100;
        result = rte_hash_lookup_data(hash_tabel, &hash_key, (void **)&hash_data_ptr);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(LI_SHELL_WARNING, rte_hash_lookup_data, result, "%d", ret);
        LI_UNIT_TEST_ASSERT(*hash_data_ptr == hash_key * 10);

        hash_key = 1024;
        result = rte_hash_lookup_data(hash_tabel, &hash_key, (void **)&hash_data_ptr);
        LI_CHECK_EXTERN_FUNC_RETURN_BREAK(LI_SHELL_WARNING, rte_hash_lookup_data, result, "%d", ret);
        LI_UNIT_TEST_ASSERT(*hash_data_ptr == hash_key * 10);

        hash_key = 0;
        result = rte_hash_lookup_data(hash_tabel, &hash_key, (void **)&hash_data_ptr);
        LI_UNIT_TEST_ASSERT(result != 0);

        hash_key = 1025;
        result = rte_hash_lookup_data(hash_tabel, &hash_key, (void **)&hash_data_ptr);
        LI_UNIT_TEST_ASSERT(result != 0);

    }while(0);
    
    if(0 != result)
    {
        if(result < 0)
        {
            result = 0 - result;
        }
        LI_LOG(LI_SHELL_WARNING, "dpdk return:%s\n", rte_strerror(result));
    }
    
    /* 释放哈希表内存,函数内会判断hash_tabel是否为空，此处不判断 */
    rte_hash_free(hash_tabel);

    return ret;
}

