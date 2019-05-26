/***********************************************************************************

* 文 件 名   : li_hash.h
* 负 责 人   : jiangbo,蒋博
* 创建日期   : 2017年4月1日
* 版 本 号   :
* 文件描述   : li_hash.c 的头文件
* 版权说明   : Copyright (C) 2000-2017   
* 其    他   :
* 修改日志   :

***********************************************************************************/

#ifndef __LI_HASH_H__
#define __LI_HASH_H__

#include "li_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/* 哈希表大小 */
#define LI_HASH_SIZE_2_8                   0x00000100
#define LI_HASH_SIZE_MAX                   0x0000FFFF
#define LI_HASH_SIZE_2_16                  0x00010000
#define LI_HASH_SIZE_2_24                  0x01000000
#define LI_HASH_SIZE_EXT_MAX               LI_HASH_SIZE_2_24

/* 长散列算法宏。2 <= TAB_SIZE <= 2^16。根据输入数组返回一个最多24位的哈希值（取决于指定的表格大小）*/
#define LI_HASH_VALUE(ARRAY, ARRAY_LEN, TAB_SIZE) \
    li_hash_algorithm(ARRAY, ARRAY_LEN, TAB_SIZE)

/* 长散列算法宏。2 <= TAB_SIZE <= 2^24。根据输入数组返回一个最多24位的哈希值（取决于指定的表格大小）*/
#define LI_HASH_VALUE_LONG(ARRAY, ARRAY_LEN, TAB_SIZE) \
    li_extended_hash_algorithm(ARRAY, ARRAY_LEN, TAB_SIZE)

/* 返回指向指定NODE的32位KEY的指针 */
#define LI_HASH_GET_NODE_KEY(HASH_TABLE, NODE) \
    ((uint32_t *)(((uint8_t *)(NODE)) + (HASH_TABLE)->hash_key_offset))

/* 根据KEY计算桶索引 */
#define LI_CALC_BUCKET_INDEX(ROOT_CB, KEY)    ((KEY) % ((ROOT_CB)->hash_size))

/* 哈希表的最小大小。该值必须是2的幂 */
#define LI_HASH_MIN_SIZE 0x10            /*                16 buckets (2^4) */

/* 哈希表的最大大小。该值必须是2的幂 */
#define LI_HASH_MAX_SIZE 0x80000000      /*       2147483648 buckets (2^31) */

/* 可变大小哈希表的最大大小。 该值必须是2的幂。
   由于调整哈希表大小会占用一段CPU时间，本库将可变大小哈希表的最大值限制在32768个桶 */
#define LI_HASH_MAX_VARI_SIZE 0x00008000 /*       32768 buckets (2^15)      */

/* 阈值 */
#define LI_HASH_DFT_SHRINK_THRESHOLD  30  /* 哈希表收缩阀值，总大小的30% */
#define LI_HASH_DFT_GROW_THRESHOLD    80  /* 哈希表增长阀值，总大小的80% */
#define LI_HASH_MAX_SHRINK_THRESHOLD  35  /* 哈希表最大收缩阀值，总大小的35% */
#define LI_HASH_MIN_GROW_THRESHOLD    80  /* 哈希表最小增长阀值，总大小的80%(必须 > 2*LI_HASH_MAX_SHRINK_THRESHOLD) */
#define LI_HASH_GROWTH_RATE           200  /*  哈希表增长率，总大小的200% */

/* 检查一个32为数是否是2的幂。原理:二进制表示，2^n只有最高位为1，所以2^n - 1除最高位外都是1，
                                   2^n | (2^n - 1) == 2^n + (2^n - 1) == 所有位都为1(该运算仅有2的幂次成立),
                                   再+1即为2^(n+1),右移一位即为2^n。
                                   最后的LI_HASH_MAX_SIZE检测是因为0x80000000这样运算会溢出 */
#define LI_IS_POWER_OF_2(NUM)                           \
    ((((((NUM) | ((NUM) - 1)) + 1) >> 1) == (NUM)) || ((NUM) == LI_HASH_MAX_SIZE))

/* 初始化哈希表控制参数 */
#define LI_HASH_INIT_DEFAULT_PARMS(TABLE_PARMS)                          \
    (TABLE_PARMS)->hash_grow_threshold = LI_HASH_DFT_GROW_THRESHOLD;     \
    (TABLE_PARMS)->hash_shrink_threshold = LI_HASH_DFT_SHRINK_THRESHOLD; \
    (TABLE_PARMS)->min_hash_size = LI_HASH_MIN_SIZE;                     \
    (TABLE_PARMS)->max_hash_size = LI_HASH_MAX_VARI_SIZE;

/*****************************************************************************/
/* Hash functions.                                                           */
/*                                                                           */
/* The hash functions are used to create and manage a table of hash entries. */
/* The table provides an average of O(1) lookups, insertion and deletions of */
/* entries, although with an unlucky data set or poor choice of key this     */
/* could be O(n).                                                            */
/*                                                                           */
/* The application can use the LI_HASH_VALUE or LI_HASH_VALUE_LONG macros  */
/* to create hash values from arbitrary arrays of data.  There are two main  */
/* possibilities for an application that wishes to use these values.         */
/*                                                                           */
/* 1.  The application can create and manage the hash tables that store      */
/* these elements itself.                                                    */
/*                                                                           */
/* 2.  Alternatively the application can choose to use the built-in NTL Hash */
/* table manager, which will manage the table of hash entries for the        */
/* application.                                                              */
/*                                                                           */
/* The rest of this comment concerns the NTL Hash Table Manager API.         */
/*                                                                           */
/* The hash table is created through a call to LI_HASH_CREATE.  This sets   */
/* up the parameters that govern the tables behavior.  The size of the table */
/* does not need to be known in advance, the NTL Hash Table Manager will     */
/* grow or shrink the hash table as required, which helps to conserve        */
/* occupancy.  However growing or shrinking a hash table is potentially an   */
/* expensive operation, so it should be avoided if possible.  The API        */
/* provides a mechanism to do this, by allowing the application to specify   */
/* the minimum and maximum size of each table when it is created.  By        */
/* setting min=max the table will be held at a fixed size.                   */
/*                                                                           */
/* The key to the NTL Hash Table Manager's table is an uint32_t.  A simple  */
/* use of the NTL Hash Table Manager would be to store data that is          */
/* naturally keyed off a similar quantity, for example statistics reports    */
/* that are indexed on the NBB_GET_MTIME time they were gathered at.         */
/*                                                                           */
/* To support this example the NTL Hash Table Manager's interface should be  */
/* used as follows.                                                          */
/*                                                                           */
/* -  The user creates a new data structure, containing the statistics, a    */
/*    timestamp and an LI_HASH_CB.                                          */
/*                                                                           */
/* -  The user calls LI_HASH_CREATE to create the hash table and specifies  */
/*    the offset of the timestamp from the LI_HASH_CB.  Let's say the user  */
/*    knows that on average 500 data samples would need to be stored, and at */
/*    most 4000.  To plan for average 50% occupancy the user sets the        */
/*    minimum hash table size to 1024 and the maximum to 8192.               */
/*                                                                           */
/* -  As each statistics sample is gathered the user calls LI_HASH_INIT and */
/*    then LI_HASH_PUT to insert the data item into the hash table.         */
/*                                                                           */
/* -  To retrieve the statistics report for the time they are interested in  */
/*    the user calls LI_HASH_GET to retrieve the item from the hash table.  */
/*                                                                           */
/* -  The user calls LI_HASH_REMOVE to remove data items that are no longer */
/*    required.                                                              */
/*                                                                           */
/* -  The user calls LI_HASH_DESTROY to destroy the hash list.  This also   */
/*    destroys any remaining data items in the table.                        */
/*                                                                           */
/* As a second example consider the case where the statistics report aren't  */
/* indexed on timestamp, but instead they are indexed using a string.  The   */
/* changes to the above procedure are as follows.                            */
/*                                                                           */
/* -  The user creates a new data structure, with the index string and an    */
/*    uint32_t to act as the key to the hash table, instead of the          */
/*    timestamp.                                                             */
/*                                                                           */
/* -  The user calls LI_HASH_CREATE to create the hash table and specifies  */
/*    an LI_HASH_COMPARE callback that can be used to compare the strings   */
/*    in two data items.                                                     */
/*                                                                           */
/* -  As each data item is created, the user calculates the hash key using   */
/*    the LI_HASH_VALUE_LONG function over the string.                      */
/*                                                                           */
/* -  As the hash keys are not guaranteed unique then the user calls         */
/*    LI_HASH_LOOKUP_BY_COMP to find the exact matching item using the      */
/*    LI_HASH_COMPARE callback function that was specified when the hash    */
/*    table was created.                                                     */
/*****************************************************************************/

/* 哈希表管理入口 */
#define LI_HASH_CREATE(ROOT_CB, KEY_OFFSET, CALLBACKS, PARAMS) li_hash_create(ROOT_CB, KEY_OFFSET, CALLBACKS, PARAMS)

#define LI_HASH_INIT(HASH_CB) (HASH_CB)->next_hash = NULL;

#define LI_HASH_PUT(ROOT_CB, HASH_CB) li_hash_put(ROOT_CB, HASH_CB)

#define LI_HASH_REMOVE(ROOT_CB, HASH_CB) li_hash_remove(ROOT_CB, HASH_CB)

#define LI_HASH_GET(ROOT_CB, KEY) li_hash_get(ROOT_CB, KEY, NULL)

#define LI_HASH_LOOKUP_BY_COMP(ROOT_CB, HASH_CB)   \
    li_hash_get(ROOT_CB, *(LI_HASH_GET_NODE_KEY(ROOT_CB, HASH_CB)), HASH_CB)

#define LI_HASH_DESTROY(ROOT_CB) li_hash_destroy(ROOT_CB)

#define LI_HASH_VERIFY(ROOT_CB) li_hash_verify(ROOT_CB)

/* 结构体声明 */    /* 不把结构体放到这定义是因为接下来的函数指针定义用到了这些结构体，然后结构体又用到了这些函数指针
  */
struct li_hash_cb;
struct li_hash_root_cb;

/*****************************************************************************
* 函 数 名  : LI_HASH_COMPARE
* 负 责 人  : jiangbo,蒋博
* 创建日期  : 2017年4月5日
* 函数功能  : 比较两个哈希节点
* 输入参数  : IN struct li_hash_root_cb *root_cb  哈希表根节点控制块
               IN struct li_hash_cb *node1         第一个比较节点
               IN struct li_hash_cb *node2         第二个比较节点
* 输出参数  : 无
* 返 回 值  : int                                 负数 : 第一个比较节点 < 第二个比较节点
                                                      0 : 第一个比较节点 = 第二个比较节点
                                                   正数 : 第一个比较节点 > 第二个比较节点
* 调用关系  :
* 其    它  :

*****************************************************************************/
typedef int (*LI_HASH_COMPARE)(IN struct li_hash_root_cb *root_cb,
    IN struct li_hash_cb *node1,
    IN struct li_hash_cb *node2);

/*****************************************************************************
* 函 数 名  : LI_HASH_NODE_DESTROY
* 负 责 人  : jiangbo,蒋博
* 创建日期  : 2017年4月5日
* 函数功能  : 删除指定节点，包括释放内存等
* 输入参数  : IN struct li_hash_root_cb *root_cb  哈希表根节点控制块
               IN struct li_hash_cb *node          需要删除的节点
* 输出参数  : 无
* 返 回 值  : 无
* 调用关系  :
* 其    它  :

*****************************************************************************/
typedef void (*LI_HASH_NODE_DESTROY)(IN struct li_hash_root_cb *root_cb, IN struct li_hash_cb *node);

/*****************************************************************************
* 函 数 名  : LI_HASH_NODE_VERIFY
* 负 责 人  : jiangbo,蒋博
* 创建日期  : 2017年4月5日
* 函数功能  : 检测指定节点
* 输入参数  : IN struct li_hash_cb *node          需要检测的节点
* 输出参数  : 无
* 返 回 值  : 无
* 调用关系  :
* 其    它  :

*****************************************************************************/
typedef void (*LI_HASH_NODE_VERIFY)(IN struct li_hash_cb *node);

/* 哈希节点控制块。注意:该控制块没有指向自己的指针，所以必须是结构体的第一个成员 */
typedef struct li_hash_cb
{
    struct li_hash_cb *next_hash; /* 指向具有相同KEY值(碰撞节点)的下一个节点，默认为NULL */
} LI_HASH_CB;

/* 哈希表的回调函数 */
typedef struct li_hash_table_cbks
{
    /* 比较节点回调函数。 用于区分具有相同KEY的两个哈希节点；如果KEY保证是唯一的，则可设为NULL */
    LI_HASH_COMPARE hash_node_comparator;

    /* 销毁哈希表成员回调函数。用于销毁哈希表里的成员；如果哈希表里的成员通过其它方式销毁，则可设为NULL */
    LI_HASH_NODE_DESTROY hash_node_destroy;

    /* 检查回调函数。用于遍历哈希表中的每一个成员时的检测用；如果不需要检测，可以设置为NULL */
    LI_HASH_NODE_VERIFY hash_node_verify;

} LI_HASH_TABLE_CBKS;

/* 控制哈希表的大小和行为的参数。 在创建哈希表时指定，并且在表的生命周期内保持不变 */
typedef struct li_hash_table_params
{
    /*
       哈希表缩小的阀值，0-100=0%-100%。如果当前哈希表条目数/总数小于此值，且表的条目数大于min_hash_size，则缩小哈希表。
       建议值为LI_HASH_DFT_SHRINK_THRESHOLD，最大值为LI_HASH_MAX_SHRINK_THRESHOLD。
       如果为0，表示哈希表不会缩小 */
    uint16_t hash_shrink_threshold;

    /*
       哈希表增长的阀值，0-100=0%-100%。如果当前哈希表条目数/总数大于此值，且表的条目数小于max_hash_size，则增长哈希表。
       建议值为LI_HASH_DFT_GROW_THRESHOLD，最小值为LI_HASH_MIN_GROW_THRESHOLD。
       为防止哈希表不必要地增长或收缩，该值必须大于2 * hash_shrink_threshold。LI_HASH_MIN_GROW_THRESHOLD
       和LI_HASH_MAX_SHRINK_THRESHOLD保证了该条件
       建议该值 < 100，不然容易产生哈希冲突 */
    uint16_t hash_grow_threshold;

    /* 哈希表的最大大小，必须是2的幂。取值范围为[LI_HASH_MIN_SIZE,LI_HASH_MAX_SIZE]。
       表的大小固定 ?　max_hash_size == min_hash_size : max_hash_size <= LI_HASH_MAX_VARI_SIZE */
    uint32_t max_hash_size;

    /* 哈希表的最小大小，必须是2的幂。取值范围为[LI_HASH_MIN_SIZE,max_hash_size] */
    uint32_t min_hash_size;
} LI_HASH_TABLE_PARAMS;

/* 哈希表根结点控制块 */
typedef struct li_hash_root_cb
{
    LI_HASH_CB *hash_array;   /* 哈希桶数组，每个桶都是循环链表 */
    uint32_t hash_size;       /* 哈希桶数量 */
    int32_t hash_num_members; /* 当前哈希表中的节点数量 */
    int32_t hash_key_offset;  /* KEY相对于hash node的偏移 */
    LI_HASH_TABLE_CBKS callbacks; /* 回调函数 */
    LI_HASH_TABLE_PARAMS table_params;    /* 哈希表的控制参数，创建哈希表的时候制定且不能更改 */
} LI_HASH_ROOT_CB;

extern uint32_t li_extended_hash_algorithm(IN const uint8_t *byte_array,
    IN uint16_t length,
    IN uint32_t hash_table_size);
extern uint16_t li_hash_algorithm(IN const uint8_t *byte_array, IN uint16_t length, IN uint16_t hash_table_size);
extern LI_RESULT li_hash_create(IN LI_HASH_ROOT_CB *root_cb,
    IN int32_t key_offset,
    IN LI_HASH_TABLE_CBKS *callbacks,
    IN LI_HASH_TABLE_PARAMS *table_params);
extern LI_RESULT li_hash_destroy(IN LI_HASH_ROOT_CB *root_cb);
extern LI_HASH_CB *li_hash_get(IN LI_HASH_ROOT_CB *root_cb,
    IN uint32_t key,
    IN LI_HASH_CB *match_node);
extern uint32_t li_hash_get_new_size(IN LI_HASH_ROOT_CB *root_cb);
extern LI_RESULT li_hash_put(IN LI_HASH_ROOT_CB *root_cb, IN LI_HASH_CB *hash_cb);
extern LI_RESULT li_hash_remove(IN LI_HASH_ROOT_CB *root_cb, IN LI_HASH_CB *hash_cb);
extern LI_RESULT li_hash_resize(IN LI_HASH_ROOT_CB *root_cb);
extern LI_RESULT li_hash_verify(IN LI_HASH_ROOT_CB *root_cb);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LI_HASH_H__ */


