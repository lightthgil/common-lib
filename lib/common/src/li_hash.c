/***********************************************************************************

 * 文 件 名   : li_hash.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年4月1日
 * 版 本 号   : 
 * 文件描述   : 哈希表
 * 版权说明   : Copyright (C) 2000-2017   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include "li_hash.h"

/*****************************************************************************/
/* Data used in global hashing algorithm.                                    */
/*****************************************************************************/
const uint8_t li_hash_data[] =
  {
    0x44, 0x0B, 0x02, 0x88, 0x04, 0x05, 0xF9, 0xBC,
    0x03, 0x8A, 0x0A, 0xF5, 0x0C, 0x7B, 0xEC, 0x76,
    0x10, 0xC7, 0x12, 0xA9, 0x14, 0x97, 0xFD, 0xA7,
    0x29, 0x2B, 0x1A, 0x68, 0x9C, 0xAD, 0x3A, 0x1F,
    0x9A, 0x9B, 0xEA, 0x23, 0x5F, 0xC2, 0x7A, 0x01,
    0x8B, 0x6F, 0xF6, 0x4E, 0x21, 0xC9, 0xFE, 0x2F,
    0x8E, 0xBA, 0x15, 0x33, 0x34, 0x91, 0x5E, 0x37,
    0xE5, 0x31, 0xF2, 0x59, 0x3C, 0x3D, 0x7D, 0x4B,
    0xB4, 0x49, 0x42, 0x43, 0xC8, 0xB3, 0xF0, 0x47,
    0x07, 0x3B, 0x85, 0xDF, 0x4C, 0xB0, 0x30, 0x4F,
    0xCE, 0xE7, 0x79, 0x53, 0x9D, 0x55, 0xB6, 0x1C,
    0x27, 0xCF, 0xBB, 0xD8, 0x2A, 0xDE, 0xB2, 0x77,
    0x92, 0x5C, 0x38, 0x5B, 0x73, 0x65, 0xB7, 0x72,
    0x6D, 0xD5, 0x6A, 0x6B, 0x6C, 0x96, 0x89, 0x1E,
    0x57, 0x71, 0x3E, 0x64, 0xDC, 0x56, 0x0D, 0x98,
    0x78, 0xF8, 0x51, 0xC5, 0x48, 0xED, 0x7E, 0x36,
    0xE0, 0x16, 0x86, 0x83, 0x84, 0x50, 0xDA, 0x90,
    0x46, 0x7F, 0xF4, 0x13, 0x8C, 0x8D, 0xA6, 0x8F,
    0xD9, 0x35, 0xAC, 0xE8, 0x18, 0x95, 0x40, 0xA8,
    0x54, 0xEB, 0xAA, 0xB1, 0x08, 0xD0, 0x9E, 0xFF,
    0xA0, 0xA1, 0x32, 0x41, 0x20, 0xD7, 0xCB, 0x61,
    0x63, 0x6E, 0xBE, 0xAB, 0x24, 0xF1, 0xAE, 0xAF,
    0x25, 0x28, 0xD4, 0x45, 0x87, 0x58, 0x75, 0x5A,
    0xB8, 0xB9, 0x0E, 0xC6, 0x93, 0x80, 0x4D, 0xA3,
    0xD1, 0xB5, 0xA4, 0xC3, 0x5D, 0x17, 0xEF, 0xBF,
    0x9F, 0x2D, 0x11, 0x2E, 0x67, 0xF7, 0x2C, 0x26,
    0x0F, 0xC0, 0x94, 0x60, 0xD2, 0x69, 0xD6, 0xA5,
    0x82, 0x1B, 0xA2, 0xDB, 0xCD, 0xC4, 0xDD, 0x3F,
    0xFB, 0xE1, 0xE2, 0xE3, 0xE4, 0x62, 0xBD, 0x06,
    0x99, 0xE9, 0x22, 0x7C, 0x74, 0xCC, 0xEE, 0xCA,
    0x19, 0x1D, 0x70, 0xF3, 0x09, 0xD3, 0x4A, 0xC1,
    0x52, 0x39, 0xFA, 0xE6, 0xFC, 0x81, 0x66, 0x00
  };

/*****************************************************************************
 * 函 数 名  : li_hash_algorithm
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 从数组生成16位哈希值(原理:默认一个包含256个元素的数组A，数组A是用0-255按随机排序生成的。
                                         用传入数组的每个字节与之前生成的哈希值进行异或运算，运算结果作为数组A的索引，
                                         产生新的哈希值。一直迭代运算完传入的数组，最后生成的哈希值即为该数组的哈希值。)
 * 输入参数  : IN const uint8_t *byte_array  数组指针
               IN uint16_t length            数组长度
               IN uint16_t hash_table_size   哈希表大小，如果使用可变大小的哈希表，则此值为表的最大可能大小
 * 输出参数  : 无
 * 返 回 值  : uint16_t                      哈希值(范围0 - (hash_table_size - 1))
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint16_t li_hash_algorithm(IN const uint8_t *byte_array,
                              IN uint16_t length,
                              IN uint16_t hash_table_size)
{
    uint16_t ii;
    uint8_t cur_low = 0;
    uint8_t cur_high = 0;
    uint16_t hash_value = 0;
    LI_RESULT ret = CFG_OK;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, byte_array, ret);
        LI_CHECK_PARAM_OPEN_MIN(BMU_ERR, length, 0, "%u", "%d", ret);

        /* 检查哈希表大小 */
        LI_ASSERT(BMU_ERR, LI_IS_POWER_OF_2(hash_table_size), ret);

        /* 计算哈希值的低字节 */
        for (ii = 0; ii < length; ii++)
        {
            cur_low = li_hash_data[cur_low ^ byte_array[ii]];
        }

        /* 如果哈希表大于256个条目，则计算高字节 */
        if (hash_table_size > 256)
        {
            LI_LOG(BMU_DEBUG, "Large hash table\n");
            cur_high = li_hash_data[(uint16_t)(byte_array[0] + 1) & 0xFF];
            for (ii = 1; ii < length; ii++)
            {
                cur_high = li_hash_data[cur_high ^ byte_array[ii]];
            }
        }

        hash_value = (uint16_t)((cur_low + (cur_high << 8)) % hash_table_size);
        LI_LOG(BMU_DEBUG, "Returned value (0-%d) = %d\n", (hash_table_size - 1), hash_value);

    }
    while (0);
    
    LI_IGNORE_VAL(ret);
    return(hash_value);
}

/*****************************************************************************
 * 函 数 名  : li_hash_create
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 生成哈希表(初始大小为 table_params->min_hash_size)
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb            哈希表根节点控制块
               IN int32_t key_offset                  自定义数组中32位哈希KEY成员到哈希节点控制块成员的偏移
               IN LI_HASH_TABLE_CBKS *callbacks       回调函数
               IN LI_HASH_TABLE_PARAMS *table_params  哈希表的控制参数
                                                      (max_hash_size == min_hash_size : 哈希表固定大小 ? 哈希表可变大小)
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                              错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_create(IN LI_HASH_ROOT_CB *root_cb,
                         IN int32_t key_offset,
                         IN LI_HASH_TABLE_CBKS *callbacks,
                         IN LI_HASH_TABLE_PARAMS *table_params)
{
    LI_RESULT ret = CFG_OK;
    uint32_t i;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, callbacks, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, table_params, ret);

        /* 检查hash表大小限制 */
        LI_CHECK_PARAM_MIN(BMU_ERR, table_params->hash_grow_threshold, LI_HASH_MIN_GROW_THRESHOLD, "%d", "%d", ret);
        LI_CHECK_PARAM_MAX(BMU_ERR, table_params->hash_shrink_threshold, LI_HASH_MAX_SHRINK_THRESHOLD, "%d", "%d", ret);
        LI_CHECK_PARAM_MIN(BMU_ERR, table_params->max_hash_size, LI_HASH_MIN_SIZE, "%" PRId32, "%d", ret);
        LI_CHECK_PARAM_MIN(BMU_ERR, table_params->min_hash_size, LI_HASH_MIN_SIZE, "%" PRId32, "%d", ret);
        LI_CHECK_PARAM_MIN(BMU_ERR, table_params->max_hash_size, table_params->min_hash_size, "%" PRId32, "%d", ret);

        /* 超过系统支持的最大可变长hash表大小 */
        if (table_params->max_hash_size > LI_HASH_MAX_VARI_SIZE)
        {
            /* 当表的大小比较大时，基于调整表大小的时间效率考虑，采用不可调整大小的哈希表 */
            LI_LOG(BMU_DEBUG, "Table too big for variable size %" PRId32 "\n", table_params->max_hash_size);
            LI_CHECK_PARAM_EQUAL(BMU_ERR,
                table_params->max_hash_size,
                table_params->min_hash_size,
                "%" PRId32,
                "%" PRId32,
                ret);
        }

        LI_ASSERT(BMU_ERR, LI_IS_POWER_OF_2(table_params->max_hash_size), ret);
        LI_ASSERT(BMU_ERR, LI_IS_POWER_OF_2(table_params->min_hash_size), ret);

        /* 初始化root_cb指向的内存 */
        OS_MEMSET(root_cb, 0, sizeof(*root_cb));

        /* 分配哈希数组的内存 */
        root_cb->hash_array = (LI_HASH_CB *)OS_CALLOC(table_params->min_hash_size, sizeof(LI_HASH_CB));
        LI_CHECK_NULL_FUNC_BREAK(BMU_WARNING, OS_CALLOC, root_cb->hash_array, ret);

        root_cb->hash_size = table_params->min_hash_size;
        root_cb->hash_key_offset = key_offset;

        /* 复制哈希表的控制参数 */
        OS_MEMCPY(&(root_cb->table_params), table_params, sizeof(root_cb->table_params));

        /* 复制回调函数 */
        LI_LOG(BMU_DEBUG, "Setting comparator function to %p\n", callbacks->hash_node_comparator);
        root_cb->callbacks.hash_node_comparator = callbacks->hash_node_comparator;

        LI_LOG(BMU_DEBUG, "Setting destroy function to %p\n", callbacks->hash_node_destroy);
        root_cb->callbacks.hash_node_destroy = callbacks->hash_node_destroy;

        LI_LOG(BMU_DEBUG, "Setting verify function to %p\n", callbacks->hash_node_verify);
        root_cb->callbacks.hash_node_verify = callbacks->hash_node_verify;

        /* 使每个桶的头指针指向自己(每个桶都是一个循环链表) */
        for (i = 0; i < root_cb->hash_size; i++)
        {
            LI_LOG(BMU_DEBUG, "Initializing bucket[%" PRIu32 "]\n", i);
            root_cb->hash_array[i].next_hash = &root_cb->hash_array[i];
        }

    }
    while (0);

    return ret;

}

/*****************************************************************************
 * 函 数 名  : li_hash_put
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 添加哈希节点(添加节点后，不应更改哈希值。 如果需要改变哈希值，那么节点应该先删除，改变后再重新添加)
               (原理:找到哈希密钥（依据hash CB的key_offset），并计算要使用的正确桶，将节点添加到存储桶。 
                注意:对于可变大小的表，此操作可能会导致表增长，可能会将占用CPU一段时间。因此，可变大小的表不能大于
                     LI_HASH_MAX_VARI_SIZE。)
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根节点控制块
               IN LI_HASH_CB *hash_cb       需要添加的哈希节点控制块
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                    错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_put(IN LI_HASH_ROOT_CB *root_cb,
                      IN LI_HASH_CB *hash_cb)
{
    LI_RESULT ret = CFG_OK;
    uint32_t *node_key;
    uint32_t bucket_index;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, hash_cb, ret);

        /* 检查哈希表是否已初始化 */
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb->hash_array, ret);
        LI_CHECK_PARAM_EQUAL(BMU_ERR, root_cb->hash_size, 0, "%" PRId32, "%d", ret);
        LI_LOG(BMU_DEBUG, "Hash table is initialized\n");

        /* 检查需要插入的元素没有被放入另一个哈希表 */
        LI_ASSERT(BMU_ERR, hash_cb->next_hash == NULL, ret)

        /* 获取KEY */
        node_key = LI_HASH_GET_NODE_KEY(root_cb, hash_cb);

        /* 插入到hash桶中 */
        bucket_index = LI_CALC_BUCKET_INDEX(root_cb, (*node_key));

        hash_cb->next_hash = root_cb->hash_array[bucket_index].next_hash;
        root_cb->hash_array[bucket_index].next_hash = hash_cb;

        /* 更新哈希表的元素个数 */
        root_cb->hash_num_members++;

        /* 检查是否需要调整哈希表大小。在函数外检查是为了减少函数调用的开销 */
        if (root_cb->table_params.min_hash_size !=
            root_cb->table_params.max_hash_size)
        {
            LI_LOG(BMU_DEBUG, "Check if a resize is required.\n");
            li_hash_resize(root_cb);
        }

    }
    while (0);

    return ret;
} 

/*****************************************************************************
 * 函 数 名  : li_hash_remove
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 删除指定的哈希节点
               (原理:查找哈希KEY（依据hash CB的key_offset），并计算要使用的正确桶，从桶中删除节点。 
                注意:对于可变大小的表，此操作可能导致表缩小，可能会将占用CPU一段时间。 因此，可变大小的表不能大于
                     LI_HASH_MAX_VARI_SIZE。)
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点控制块
               IN LI_HASH_CB *hash_cb       需要删除的哈希节点控制块
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                    错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_remove(IN LI_HASH_ROOT_CB *root_cb,
                         IN LI_HASH_CB *hash_cb)
{
    LI_RESULT ret = CFG_OK;
    uint32_t *key_node;
    uint32_t bucket_index;
    LI_HASH_CB *bucket_ptr;
    LI_HASH_CB *prev_node, *current_node;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);
        LI_CHECK_PARAM_POINT(BMU_ERR, hash_cb, ret);

        /* 检查哈希表是否已初始化 */
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb->hash_array, ret);
        LI_CHECK_PARAM_EQUAL(BMU_ERR, root_cb->hash_size, 0, "%" PRId32, "%d", ret);
        LI_LOG(BMU_DEBUG, "Hash table is initialized\n");

        /* 检查需要删除的元素非空 */
        LI_CHECK_PARAM_POINT(BMU_ERR, hash_cb->next_hash, ret);
        LI_LOG(BMU_DEBUG, "The node is in the hash table. Remove it from the table\n");

        /* 不用双向链表，表面上看，这种算法无法回溯到前后两个节点直接删除，需要迭代查找导致效率低。
            但因为哈希表的设计使每个桶的元素数量(平均1/2)很少，所以基本上第一次迭代就能找到，所以实际效率不低
            使用当前的设计可以节约内存 */
        key_node = LI_HASH_GET_NODE_KEY(root_cb, hash_cb);
        bucket_index = LI_CALC_BUCKET_INDEX(root_cb, (*key_node));

        bucket_ptr = &(root_cb->hash_array[bucket_index]);
        prev_node = bucket_ptr;
        current_node = bucket_ptr->next_hash;

        /* 迭代查找需要删除的元素 */
        while ((current_node != bucket_ptr)
            && (current_node != hash_cb))
        {
            LI_LOG(BMU_DEBUG, "Searching the required node to remove\n");
            prev_node = current_node;
            current_node = current_node->next_hash;
        }

        /* 找到需要删除的节点 */
        if (current_node == hash_cb)
        {
            LI_LOG(BMU_DEBUG, "Required node found\n");

            /* 前个节点链接到下个节点，从桶中删除该元素 */
            prev_node->next_hash = hash_cb->next_hash;

            /* 清楚指向的下个节点，防止内存回收失败，重复调用删除哈希接口，重复删除 */
            hash_cb->next_hash = NULL;

            /* 减少哈希表的元素个数 */
            root_cb->hash_num_members--;

            /* 检查成员的数量是否小于零，以提高健壮性 */
            if (root_cb->hash_num_members <= 0)
            {
                LI_LOG(BMU_DEBUG, "The hash table is empty\n");

                root_cb->hash_num_members = 0;
            }

            /* 检查是否需要调整哈希表大小。在函数外检查是为了减少函数调用的开销 */
            if ((root_cb->table_params.min_hash_size !=
                    root_cb->table_params.max_hash_size)
                && (root_cb->table_params.hash_shrink_threshold != 0))
            {
                LI_LOG(BMU_DEBUG, "Variable size table\n");
                li_hash_resize(root_cb);
            }
        }

    }
    while (0);

    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_hash_get
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 根据key和match_node在哈希表中查找节点
               (原理: 根据输入的KEY映射到一个存储桶，然后搜索桶中的KEY值相等的项。
                      如果输入的match_node不为空，则使用创建哈希表时注册的比较函数执行进一步的检查。
                注意: 当KEY在哈希表中不唯一时，应使用match_node，
                      如：当KEY实际上是多个变量复合时，只能使用其中一个变量作为KEY，将节点插入到哈希表中。 
                          查找时，需要使用完整密钥进行检索。如果散列表包含多个具有相同KEY值的节点，
                          则使用创建哈希表时注册的比较函数将通过比较输入的match_node找到正确的节点)
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点
               IN uint32_t key              哈希KEY
               IN LI_HASH_CB *match_node    匹配条件
                                            (match_node == NULL ? 不额外比较 : 使用存放在match_node中完整KEY值比较)
 * 输出参数  : 无
 * 返 回 值  : LI_HASH_CB                   NULL ? 没找到 : 找到的哈希节点控制块
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_HASH_CB *li_hash_get(IN LI_HASH_ROOT_CB *root_cb,
                          IN uint32_t key,
                          IN LI_HASH_CB *match_node)
{
    LI_RESULT ret = CFG_OK;
    uint32_t bucket_index = 0;
    uint32_t *current_key = NULL;
    LI_HASH_CB *bucket_ptr = NULL;
    LI_HASH_CB *current_node = NULL;
    bool isfound = false;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);
        if (NULL != match_node)  /* match_node有定义时，必须有定义比较函数 */
        {
            LI_CHECK_PARAM_POINT(BMU_ERR, root_cb->callbacks.hash_node_comparator, ret);
        }

        /* 检查哈希表是否已初始化 */
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb->hash_array, ret);
        LI_CHECK_PARAM_EQUAL(BMU_ERR, root_cb->hash_size, 0, "%" PRId32, "%d", ret);

        LI_LOG(BMU_DEBUG, "Root CB %p\n", root_cb);
        LI_LOG(BMU_DEBUG, "Key %" PRIu32 "\n", key);
        LI_LOG(BMU_DEBUG, "Match node %p\n", match_node);

        bucket_index = LI_CALC_BUCKET_INDEX(root_cb, key);

        bucket_ptr = &root_cb->hash_array[bucket_index];
        current_node = bucket_ptr->next_hash;

        /* 迭代查找元素 */
        while (current_node != bucket_ptr)
        {
            LI_LOG(BMU_DEBUG, "Checking node: %p\n", current_node);

            current_key = LI_HASH_GET_NODE_KEY(root_cb, current_node);

            /* 检查KEY是否相等 */
            if (*current_key == key)
            {
                LI_LOG(BMU_DEBUG, "The node key matches, key=%" PRIu32 "\n", key);

                if (match_node != NULL)   /* 需要额外匹配 */
                {
                    LI_LOG(BMU_DEBUG, "Specific match requested.\n");
                    if (root_cb->callbacks.hash_node_comparator(root_cb, match_node, current_node) == 0)
                    {
                        /* 找到元素 */
                        LI_LOG(BMU_DEBUG, "Found matched node\n");
                        isfound = true;
                        break;
                    }
                }
                else  /* 不用额外匹配 */
                {
                    LI_LOG(BMU_DEBUG, "Found node\n");
                    isfound = true;
                    break;
                }
            }

            current_node = current_node->next_hash;
        }

        if(!isfound)    /* 没找到 */
        {
            LI_LOG(BMU_DEBUG, "Node was not found\n");
            current_node = NULL;
        }

    }
    while (0);

    LI_LOG(BMU_DEBUG, "Returning %p\n", current_node);

    LI_IGNORE_VAL(ret);
    
    return current_node;
}

/*****************************************************************************
 * 函 数 名  : li_hash_resize
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 调整哈希表的大小(根据root_cb->table_params决定是否调整)
               (原理:哈希现有成员数量是否在的可接受范围内 ? 不改变大小 : 调整表的大小)
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                    错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_resize(IN LI_HASH_ROOT_CB *root_cb)
{
    LI_RESULT ret = CFG_OK;
    uint32_t ii;
    int32_t element_counter;
    uint32_t old_size;
    uint32_t new_size;
    uint32_t bucket_index;
    uint32_t *current_key;
    LI_HASH_CB *current_bucket;
    LI_HASH_CB *current_node = NULL;
    LI_HASH_CB *new_hash_array;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);

        /* 检查哈希表是否已初始化 */
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb->hash_array, ret);
        LI_CHECK_PARAM_EQUAL(BMU_ERR, root_cb->hash_size, 0, "%" PRId32, "%d", ret);

        /* 计算新的哈希表大小 */
        new_size = li_hash_get_new_size(root_cb);

        /* 如果不需要调整大小操作，则只能记录并退出 */
        if (new_size == root_cb->hash_size)
        {
            LI_LOG(BMU_DEBUG, "It is not necessary to resize the hash size\n");
            break;
        }

        /* 分配新的哈希数组，失败了也没事，调整大小只是为了平衡元素，而且下次增加/删除元素后会重新调整 */
        new_hash_array = (LI_HASH_CB *)OS_CALLOC(new_size, sizeof(LI_HASH_CB));
        LI_CHECK_NULL_FUNC_BREAK(BMU_WARNING, OS_CALLOC, new_hash_array, ret);

        /* 最初使每个桶指针指向自己(每个桶都是循环链表) */
        for (ii = 0; ii < new_size; ii++)
        {
            LI_LOG(BMU_DEBUG, "Initializing bucket[%" PRIu32 "]\n", ii);
            new_hash_array[ii].next_hash = &new_hash_array[ii];
        }

        element_counter = 0;
        old_size = root_cb->hash_size;
        root_cb->hash_size = new_size;

        /* 将哈希节点从旧桶移动到新桶，重新计算每个节点的位置 */
        for (ii = 0; ii < old_size; ii++)
        {
            LI_LOG(BMU_DEBUG, "Moving bucket[%" PRIu32 "]\n", ii);

            current_bucket = &root_cb->hash_array[ii];

            while (current_bucket->next_hash != current_bucket)
            {
                LI_LOG(BMU_DEBUG, "Moving node from bucket[%" PRIu32 "]\n", ii);

                current_node = current_bucket->next_hash;

                /* 从旧桶中删除节点 */
                current_bucket->next_hash = current_node->next_hash;
                current_node->next_hash = NULL;

                /* 计算新哈希数组中的节点位置 */
                current_key = LI_HASH_GET_NODE_KEY(root_cb, current_node);
                bucket_index = LI_CALC_BUCKET_INDEX(root_cb, (*current_key));

                /* 将节点添加到新桶 */
                current_node->next_hash = new_hash_array[bucket_index].next_hash;
                new_hash_array[bucket_index].next_hash = current_node;

                /* 更新元素计数器 */
                element_counter++;

            }
        }

        /* 释放旧哈希数组使用的分配内存 */
        OS_FREE(root_cb->hash_array);

        /* 将哈希表数组更新为新分配的数组 */
        root_cb->hash_array = new_hash_array;

        /* 确保所有节点都被移到新桶 */
        LI_CHECK_PARAM_NOT_EQUAL(BMU_ERR, element_counter, root_cb->hash_num_members, "%" PRIu32, "%" PRIu32, ret);
    }
    while (0);

    return ret;
}

/*****************************************************************************
* 函 数 名  : li_hash_get_new_size
* 负 责 人  : jiangbo,蒋博
* 创建日期  : 2017年4月5日
* 函数功能  : 计算哈希表的新大小
* 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点控制块
* 输出参数  : 无
* 返 回 值  : uint32_t                     哈希表的新大小
* 调用关系  :
* 其    它  :

*****************************************************************************/
uint32_t li_hash_get_new_size(IN LI_HASH_ROOT_CB *root_cb)
{
    LI_RESULT ret = CFG_OK;
    bool resize = false;
    bool shrink = false;
    int32_t bottom_limit = 0;
    int32_t top_limit = 0;
    uint32_t new_size = 0;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);

        /* 默认大小不变 */
        new_size = root_cb->hash_size;

        /* 根据当前的哈希表大小计算上限和下限 */
        bottom_limit = root_cb->hash_size * root_cb->table_params.hash_shrink_threshold;
        top_limit = root_cb->hash_size * root_cb->table_params.hash_grow_threshold;

        /* 如果节点数量小于当前哈希表大小的下限，则需要缩小哈希表大小（前提是未达到最小大小） */
        if ((100 * root_cb->hash_num_members < bottom_limit)
            && (root_cb->hash_size > root_cb->table_params.min_hash_size))
        {
            LI_LOG(BMU_DEBUG, "Shrink needed\n");
            resize = true;
            shrink = true;
        }

        /* 如果节点数量大于当前哈希表大小的上限，则需要增加哈希表大小（前提是未达到最大大小） */
        else if ((100 * root_cb->hash_num_members > top_limit)
            && (root_cb->hash_size < root_cb->table_params.max_hash_size))
        {
            LI_LOG(BMU_DEBUG, "Grow needed\n");
            resize = true;
            shrink = false;
        }

        /*
           通常情况下，这个循环只会执行一次。仅当以前调整大小失败时，可能会出现减少或增加一次大小不够的情况，
           会出现多次迭代。该算法可保证一次性把以前没有调整成功的哈希表大小一次调整到位 */
        while (resize)
        {
            LI_LOG(BMU_DEBUG, "Calculating the new hash size\n");

            /* 计算新的哈希表大小 */
            if (shrink)
            {
                LI_LOG(BMU_DEBUG, "Shrinking hash table\n");

                /* 将当前大小除以2（哈希表大小总是2的幂） */
                new_size = new_size >> 1;

                bottom_limit = new_size * root_cb->table_params.hash_shrink_threshold;

                if (!((100 * root_cb->hash_num_members < bottom_limit)
                        && (new_size > root_cb->table_params.min_hash_size)))
                {
                    LI_LOG(BMU_DEBUG, "Shrink calculation complete\n");
                    resize = false;
                }
            }
            else
            {
                LI_LOG(BMU_DEBUG, "Growing hash table\n");

                /* 将当前大小乘以2（哈希表大小总是2的幂） */
                new_size = new_size << 1;

                top_limit = new_size * root_cb->table_params.hash_grow_threshold;

                if (!((100 * root_cb->hash_num_members > top_limit) 
                    && (new_size < root_cb->table_params.max_hash_size)))
                {
                    LI_LOG(BMU_DEBUG, "Growth calculation complete\n");
                    resize = false;
                }
            }
        }
    }
    while (0);

    LI_IGNORE_VAL(ret);
    return(new_size);
}

/*****************************************************************************
 * 函 数 名  : li_hash_destroy
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 删除整个哈希表
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点控制块
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                    错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_destroy(IN LI_HASH_ROOT_CB *root_cb)
{
    LI_RESULT ret = CFG_OK;
    uint32_t i;
    LI_HASH_CB *bucket_ptr = NULL;
    LI_HASH_CB *current_node = NULL;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);

        /* 如果哈希数组为空，则不需要删除 */
        if (root_cb->hash_array == NULL)
        {
            LI_LOG(BMU_DEBUG, "Nothing to destroy\n");
            break;
        }

        /* 在每个桶上迭代释放每个节点 */
        for (i = 0; i < root_cb->hash_size; i++)
        {
            LI_LOG(BMU_DEBUG, "Freeing bucket[%" PRIu32 "].\n", i);
            bucket_ptr = &root_cb->hash_array[i];

            while (bucket_ptr->next_hash != bucket_ptr)
            {
                LI_LOG(BMU_DEBUG, "Freeing node of bucket[%" PRIu32 "]\n", i);

                /* 从桶中删除当前节点，释放其分配的内存 */
                current_node = bucket_ptr->next_hash;
                bucket_ptr->next_hash = current_node->next_hash;
                current_node->next_hash = NULL;
                if (root_cb->callbacks.hash_node_destroy != NULL)
                {
                    LI_LOG(BMU_DEBUG, "Using specified destroy call\n");
                    root_cb->callbacks.hash_node_destroy(root_cb, current_node);
                }
            }
        }

        /* 释放所有节点后，释放哈希表数组 */
        OS_FREE(root_cb->hash_array);

        /* 重置由root_cb指向的内存 */
        OS_MEMSET(root_cb, 0, sizeof(*root_cb));

    }
    while (0);

    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_hash_verify
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 遍历验证哈希表
 * 输入参数  : IN LI_HASH_ROOT_CB *root_cb  哈希表根结点控制块
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT                    错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_hash_verify(IN LI_HASH_ROOT_CB *root_cb)
{
    LI_RESULT ret = CFG_OK;
    uint32_t i;
    LI_HASH_CB *bucket_ptr = NULL;
    LI_HASH_CB *current_node = NULL;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, root_cb, ret);

        /* 如果哈希数组为空，则不需要删除 */
        if (root_cb->hash_array == NULL)
        {
            LI_LOG(BMU_DEBUG, "Nothing to verify\n");
            break;
        }

        LI_LOG(BMU_DEBUG, "Verify.the hash table memory\n");

        /* 在每个桶上迭代验证每个节点 */
        if (root_cb->callbacks.hash_node_verify != NULL)
        {
            LI_LOG(BMU_DEBUG, "Verify function is available\n");

            for (i = 0; i < root_cb->hash_size; i++)
            {
                LI_LOG(BMU_DEBUG, "Verifying bucket[%" PRIu32 "]\n", i);
                bucket_ptr = &root_cb->hash_array[i];

                current_node = bucket_ptr->next_hash;

                while (current_node != bucket_ptr)
                {
                    LI_LOG(BMU_DEBUG, "Verifying node of bucket[%" PRIu32 "]\n", i);

                    root_cb->callbacks.hash_node_verify(current_node);

                    current_node = current_node->next_hash;
                }
            }
        }

    }
    while (0);
    
    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_extended_hash_algorithm
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月5日
 * 函数功能  : 从数组生成24位哈希值(原理:默认一个包含256个元素的数组A，数组A是用0-255按随机排序生成的。
                                         用传入数组的每个字节与之前生成的哈希值进行异或运算，运算结果作为数组A的索引，
                                         产生新的哈希值。一直迭代运算完传入的数组，最后生成的哈希值即为该数组的哈希值。
                                         生成高阶字节时，传入数组的第一个字节被加了1或2)
 * 输入参数  : IN const uint8_t *byte_array  数组指针(当输入数组为空时，返回0)
               IN uint16_t length            数组长度
               IN uint16_t hash_table_size   哈希表大小，如果使用可变大小的哈希表，则此值为表的最大可能大小
 * 输出参数  : 无
 * 返 回 值  : uint16_t                      哈希值(范围0 - (hash_table_size - 1))(当输入数组为空时，返回0)
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
uint32_t li_extended_hash_algorithm(IN const uint8_t *byte_array,
                                      IN uint16_t length,
                                      IN uint32_t hash_table_size)
{
    uint16_t ii;
    uint8_t cur_low;
    uint8_t cur_middle;
    uint8_t cur_high;
    uint32_t hash_value = 0;
    LI_RESULT ret = CFG_OK;

    do
    {
        LI_CHECK_PARAM_POINT(BMU_ERR, byte_array, ret);
        LI_CHECK_PARAM_OPEN_MIN(BMU_ERR, length, 0, "%u", "%d", ret);

        /* 检查哈希表大小 */
        LI_ASSERT(BMU_ERR, LI_IS_POWER_OF_2(hash_table_size), ret);
        LI_CHECK_PARAM_MAX(BMU_ERR, hash_table_size, LI_HASH_SIZE_EXT_MAX, "%" PRIu32, "%d", ret);

        /* 初始化哈希值 */
        cur_low = li_hash_data[byte_array[0]];
        cur_middle = li_hash_data[((uint16_t)byte_array[0] + 1) & 0xFF];
        cur_high = li_hash_data[((uint16_t)byte_array[0] + 2) & 0xFF];

        for (ii = 1; ii < length; ii++)
        {
            /* 计算哈希值的低字节 */
            /* FLOW TRACING NOT REQUIRED Reason: Performance. */
            cur_low = li_hash_data[cur_low ^ byte_array[ii]];

            /* 如果哈希表大于256个条目，则计算中间字节 */
            if (hash_table_size > 256)
            {
                /* FLOW TRACING NOT REQUIRED Reason: Performance. */
                cur_middle = li_hash_data[cur_middle ^ byte_array[ii]];

                /* 如果哈希表大于65536个条目，则计算高字节 */
                if (hash_table_size > 65536)
                {
                    /* FLOW TRACING NOT REQUIRED Reason: Performance. */
                    cur_high = li_hash_data[cur_high ^ byte_array[ii]];
                }
            }
        }

        hash_value = (uint32_t)((cur_low + (cur_middle << 8) + (cur_high << 16)) & (hash_table_size - 1));
    }
    while (0);

    LI_LOG(BMU_DEBUG, "Returned value (0-%d) = %d\n", (hash_table_size - 1), hash_value);

    LI_IGNORE_VAL(ret);
    return(hash_value);
}

