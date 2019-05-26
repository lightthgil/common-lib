/***********************************************************************************

 * 文 件 名   : test_common_hash.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年4月5日
 * 版 本 号   : 
 * 文件描述   : 测试通用哈希表
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "test_common_hash.h"

/*****************************************************************************
 * 函 数 名  : test_common_destroy_hash_node
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 销毁指定的哈希表节点
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点控制块
               IN LI_HASH_CB *node          哈希表节点控制块
 * 输出参数  : 无
 * 返 回 值  : void                         无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void test_common_destroy_hash_node(IN LI_HASH_ROOT_CB *root_cb, IN LI_HASH_CB *node)
{
    LI_RESULT ret = CFG_OK;
    
    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(LI_SHELL_ERR, root_cb, ret);
        LI_CHECK_PARAM_POINT(LI_SHELL_ERR, node, ret);

        OS_FREE(node);
    } while ( 0 ); /* 循环直到0不成立 */

    LI_IGNORE_VAL(ret);
    return;
}


/*****************************************************************************
 * 函 数 名  : test_common_hash
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 测试通用哈希表
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT test_common_hash()
{
    LI_RESULT ret = CFG_OK;
    LI_HASH_ROOT_CB hash_table;
    LI_HASH_TABLE_CBKS callbacks;
    LI_HASH_TABLE_PARAMS table_params;
    uint32_t i = 0;
    TEST_HASH_UINT32 *test_node = NULL;
    LI_HASH_CB *found_hash_node = NULL;
    TEST_HASH_UINT32 *found_node = NULL;

    do
    {
        /* 初始化参数 */
        OS_MEMSET(&hash_table, 0, sizeof(hash_table));
        OS_MEMSET(&callbacks, 0, sizeof(callbacks));
        LI_HASH_INIT_DEFAULT_PARMS(&table_params);

        /* 注册销毁函数 */
        callbacks.hash_node_destroy = test_common_destroy_hash_node;

        /* 指定哈希表大小 */
        table_params.max_hash_size = 0x800; /* 2048 */

        /* 创建哈希表 */
        ret = li_hash_create(&hash_table, LI_OFFSETOF(TEST_HASH_UINT32, key), &callbacks, &table_params);
        LI_CHECK_FUNC_RETURN_BREAK(LI_SHELL_WARNING, li_hash_create, ret);

        /* 测试创建哈希表 */
        LI_LOG(LI_SHELL_NOTICE, "************************测试创建通用哈希表*******************\n");
        LI_UNIT_TEST_ASSERT(hash_table.hash_num_members == 0);
        LI_UNIT_TEST_ASSERT(hash_table.hash_size == LI_HASH_MIN_SIZE);

        /* 哈希表赋值 */
        for (i = 1; i <= 1024; i++) /* 对变量i进行循环 */
        {
            test_node = (TEST_HASH_UINT32*)OS_CALLOC(1, sizeof(TEST_HASH_UINT32));
            LI_CHECK_NULL_FUNC_BREAK(LI_SHELL_ERR, OS_CALLOC, test_node, ret);
            
            test_node->key = i;
            test_node->data = i * 10;
            ret = li_hash_put(&hash_table, &(test_node->hash_cb));
            LI_CHECK_FUNC_RETURN_BREAK(LI_SHELL_WARNING, li_hash_put, ret);
        }

        /* 测试添加是否成功 */
        LI_LOG(LI_SHELL_NOTICE, "************************测试添加通用哈希表*******************\n");
        LI_UNIT_TEST_ASSERT(hash_table.hash_num_members == 1024);
        LI_UNIT_TEST_ASSERT(hash_table.hash_size == 2048);

        /* 测试查找哈希表 */
        LI_LOG(LI_SHELL_NOTICE, "************************测试查找通用哈希表*******************\n");
        found_hash_node = li_hash_get(&hash_table, 1, NULL);
        LI_CHECK_NULL_FUNC_BREAK(LI_SHELL_WARNING, li_hash_get, found_hash_node, ret)
        found_node = (TEST_HASH_UINT32 *)found_hash_node;
        LI_UNIT_TEST_ASSERT(found_node->data == 1 * 10);

        found_hash_node = li_hash_get(&hash_table, 100, NULL);
        LI_CHECK_NULL_FUNC_BREAK(LI_SHELL_WARNING, li_hash_get, found_hash_node, ret)
        found_node = (TEST_HASH_UINT32 *)found_hash_node;
        LI_UNIT_TEST_ASSERT(found_node->data == 100 * 10);

        found_hash_node = li_hash_get(&hash_table, 1024, NULL);
        LI_CHECK_NULL_FUNC_BREAK(LI_SHELL_WARNING, li_hash_get, found_hash_node, ret)
        found_node = (TEST_HASH_UINT32 *)found_hash_node;
        LI_UNIT_TEST_ASSERT(found_node->data == 1024 * 10);

        found_hash_node = li_hash_get(&hash_table, 0, NULL);
        LI_UNIT_TEST_ASSERT(found_hash_node == NULL);

        found_hash_node = li_hash_get(&hash_table, 1025, NULL);
        LI_UNIT_TEST_ASSERT(found_hash_node == NULL);

        /* 销毁哈希表 */
        ret = li_hash_destroy(&hash_table);

        /* 测试销毁是否成功 */
        LI_LOG(LI_SHELL_NOTICE, "************************测试销毁通用哈希表*******************\n");
        LI_UNIT_TEST_ASSERT(hash_table.hash_array == NULL);
        LI_UNIT_TEST_ASSERT(hash_table.hash_size == 0);
        LI_UNIT_TEST_ASSERT(hash_table.hash_num_members == 0);
        LI_UNIT_TEST_ASSERT(hash_table.hash_key_offset == 0);
        LI_UNIT_TEST_ASSERT(hash_table.callbacks.hash_node_comparator == NULL);
        LI_UNIT_TEST_ASSERT(hash_table.callbacks.hash_node_destroy == NULL);
        LI_UNIT_TEST_ASSERT(hash_table.callbacks.hash_node_verify == NULL);
        LI_UNIT_TEST_ASSERT(hash_table.table_params.hash_shrink_threshold == 0);
        LI_UNIT_TEST_ASSERT(hash_table.table_params.hash_grow_threshold == 0);
        LI_UNIT_TEST_ASSERT(hash_table.table_params.max_hash_size == 0);
        LI_UNIT_TEST_ASSERT(hash_table.table_params.min_hash_size == 0);
    }while(0);
    
    return ret;
}

//TODO:测试字符串哈希表
