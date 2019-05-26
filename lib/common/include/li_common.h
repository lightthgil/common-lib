/***********************************************************************************

 * 文 件 名   : li_common.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2016年10月12日
 * 版 本 号   : 
 * 文件描述   : 通用定义
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __LI_COMMON_H__
#define __LI_COMMON_H__

#include <stdint.h>     /* 定义了uint_8_t、uint16_t、uint32_t、uint64_t等，适配16、32、64位操作系统 */
#include <inttypes.h>   /* 定义了PRIi64、PRIu64、PRIo64以及PRIx64等，适配16、32、64位操作系统 */
#include <stdlib.h>     /* 定义了malloc()、calloc()、realloc()、free()、system()、atoi()、atol()、rand()、exit()等 */
#include <stdio.h>      /* 定义了getchar()、printf()、fopen()、fread()等 */
#include <string.h>     /* 定义了strlen()、strcmp()、strncat()、strstr()等 */
#include <arpa/inet.h>  /* 定义了ntohs()、ntohl()、htons()、htonl() */


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define IN
#define OUT
#define INOUT

#define SUCCESS 0
#define ERROR -1

#define DRV_BYTE_INVALID    (0xFF)
#define DRV_USHORT_INVALID (0xFFFF)
#define DRV_ULONG_INVALID  (0xFFFFFFFF)
#define DRV_INT_INVALID    (-1)

#ifndef __cplusplus /* c++才支持true/false/bool,使用gcc编译时定义true/false/bool */
typedef int bool;
const bool true = 1;    /* 代替宏定义 */
const bool false = 0;   /* 代替宏定义 */
#endif /* __cplusplus */

/* FPGA仿真模块错误码 */
typedef enum li_result
{
    CFG_OK = 0,             /* 增加或删除条目操作成功 */
    PARAM_ERROR,            /* 参数错误 */
    KEY_NOT_EQUAL_MYID,     /* key值错误，与本地描述符不相等 */
    KEY_NOT_EXIST,          /* KEY值不存在 */
    SET_DRIVE_ERROR,        /* 写驱动失败 */
    ALLOC_ERROR,            /* 申请内存空间时失败 */
    ADD_KEY_EXIST,          /* 该key已存在 */
    CFG_FULL,               /* 配置已满 */
    FUC_NULL,               /* 回调函数未注册 */
    INSERT_ERR,             /* 配置树中增加节点失败 */
    FORCED_END,             /* 强制终结，一般调试用，不执行函数实现，直接返回 */
    FRAME_TYPE_ERR,         /* 发帧类型错误或不支持 */
    SET_ARAD,               /* 写ARAD错误 */
    FPGA_STORT_NULL,        /* 未分配存储FPGA的数据空间 */
    KEY_NOT_MATCH,          /* KEY值不匹配 */
    FPGA_MYID_TREE,         /* 操作FPGA使用的二叉树失败 */
    IP_TYPE_ERR,            /* IP类型错误 */
    FUNC_RET_ERR,           /* 调用其他接口返回错误 */
    LIST_NULL,              /* 链表为空 */
    CFG_RESULT_MAX
}LI_RESULT;

/* 获取宏名的字符串 */
#define GET_MACRO_NAME(macro)   ""#macro

/*****************************************************************************/
/* Find the offset of specified field within specified structure             */
/*****************************************************************************/
#define LI_OFFSETOF(STRUCT, FIELD)                                           \
            (intptr_t)((uint8_t *)(&((STRUCT *)0)->FIELD) - (uint8_t *)0)

#define LI_SET_STRUCT_FIELD(STRUCT, FIELD, struct_addr, data_addr, data_length)                    \
    do                                                                                                  \
    {                                                                                                   \
        LI_SET_STRUCT_MEMBER((STRUCT), LI_OFFSETOF((STRUCT), (FIELD)), data_addr, data_length);  \
    }while(0);

#define LI_SET_STRUCT_MEMBER(STRUCT, struct_addr, member_offset, data_addr, data_length)                         \
    do                                                                                                                \
    {                                                                                                                 \
        if((member_offset) + (data_length) < sizeof(STRUCT))                                                          \
        {                                                                                                             \
            OS_MEMCPY((size_t)(struct_addr) + (size_t)(member_offset), (data_addr), (data_length)); \
        }                                                                                                             \
    }while(0);


/*****************************************************************************/
/* These are necessary in addition to the NBB_MEMSET                         */
/*****************************************************************************/
#define OS_MEMSET(MEM, VALUE, LENGTH) \
                               memset((char *)(MEM), (VALUE), (size_t)(LENGTH))
#define OS_MEMCMP(MEM1, MEM2, LENGTH) memcmp((MEM1), (MEM2), (size_t)(LENGTH))
#define OS_MEMCHR(MEM1, VALUE, LENGTH)memchr((MEM1), (VALUE), (size_t)(LENGTH))
#define OS_MEMMOVE(DEST, SRC, LENGTH) memmove((DEST), (SRC), (size_t)(LENGTH))
#define OS_MEMCPY(DEST, SRC, LENGTH)  \
                  memcpy((char *)(DEST), (const char *)(SRC), (size_t)(LENGTH))
#define OS_CALLOC(NUM, SIZE) calloc((NUM), (SIZE))
#define OS_MALLOC(SIZE) malloc(SIZE)
#define OS_FREE(MEM) if(NULL != MEM) {free(MEM); (MEM) = NULL;}

/* 主机序转网络序 */ 
#define LI_HTONL(addr)    htonl(addr)

/* 网络序转主机序 */
#define LI_NTOHL(addr)    ntohl(addr)

/* 主机序转网络序 */
#define LI_HTONS(addr)    htons(addr)

/* 网络序转主机序 */
#define LI_NTOHS(addr)    ntohs(addr)

/* 如果没指定大小端，则根据当前编译器所在的操作系统的来 */
#if !defined(LI_BIG_ENDIAN) && !defined(LI_LITTLE_ENDIAN)
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define LI_BIG_ENDIAN
    #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define LI_LITTLE_ENDIAN
    #else
        #error __BYTE_ORDER__ error
    #endif
#endif

#if defined(LI_BIG_ENDIAN)  /* 不用__BYTE_ORDER__==__ORDER_BIG_ENDIAN__判断是因为可能在大端系统下编译小端版本 */

#if 0
/* 主机序转网络序 */ 
#define LI_HTONL(addr)    ((uint32_t) (addr))

/* 网络序转主机序 */
#define LI_NTOHL(addr)    ((uint32_t) (addr))

/* 主机序转网络序 */
#define LI_HTONS(addr)    ((uint16_t) (addr))

/* 网络序转主机序 */
#define LI_NTOHS(addr)    ((uint16_t) (addr))
#endif

#define bit_2fields(A,B)      A; B
#define bit_3fields(A,B,C)    A; B; C
#define bit_4fields(A,B,C,D)  A; B; C; D
#define bit_5fields(A,B,C,D,E)  A; B; C; D; E
#define bit_6fields(A,B,C,D,E,F)  A; B; C; D; E; F
#define bit_7fields(A,B,C,D,E,F,G)  A; B; C; D; E; F; G
#define bit_8fields(A,B,C,D,E,F,G,H)  A; B; C; D; E; F; G;H
#define bit_9fields(A,B,C,D,E,F,G,H,I)  A; B; C; D; E; F; G; H; I
#define bit_10fields(A,B,C,D,E,F,G,H,I,J)  A; B; C; D; E; F; G; H; I; J
#define bit_11fields(A,B,C,D,E,F,G,H,I,J,K)  A; B; C; D; E; F; G; H; I; J; K
#define bit_12fields(A,B,C,D,E,F,G,H,I,J,K,L)  A; B; C; D; E; F; G; H; I; J; K; L

#elif defined(LI_LITTLE_ENDIAN)

#if 0
/* 主机序转网络序 */
#define LI_HTONL(addr) \
        ( \
            (((uint32_t) (addr) & 0x000000FF) << 24) | \
                (((uint32_t) (addr) & 0x0000FF00) << 8) | \
                    (((uint32_t) (addr) & 0x00FF0000) >> 8) | \
                        (((uint32_t) (addr) & 0xFF000000) >> 24) \
        )

/* 网络序转主机序 */
#define LI_NTOHL(addr) LI_HTONL(addr)

/* 主机序转网络序 */
#define LI_HTONS(addr) \
        ( \
            (((uint16_t) (addr) & 0x00FF) << 8) | \
                (((uint16_t) (addr) & 0xFF00) >> 8) \
        )

/* 网络序转主机序 */
#define LI_NTOHS(addr) LI_HTONS(addr)
#endif

#define bit_2fields(A,B)      B; A
#define bit_3fields(A,B,C)    C; B; A
#define bit_4fields(A,B,C,D)  D; C; B; A
#define bit_5fields(A,B,C,D,E)  E; D; C; B; A
#define bit_6fields(A,B,C,D,E,F)  F; E; D; C; B; A
#define bit_7fields(A,B,C,D,E,F,G)  G; F; E; D; C; B; A
#define bit_8fields(A,B,C,D,E,F,G,H)  H; G; F; E; D; C; B; A
#define bit_9fields(A,B,C,D,E,F,G,H,I)  I; H; G; F; E; D; C; B; A
#define bit_10fields(A,B,C,D,E,F,G,H,I,J)  J; I; H; G; F; E; D; C; B; A
#define bit_11fields(A,B,C,D,E,F,G,H,I,J,K)  K; J; I; H; G; F; E; D; C; B; A
#define bit_12fields(A,B,C,D,E,F,G,H,I,J,K,L)  L; K; J; I; H; G; F; E; D; C; B; A

#else
    #error NOT DEFINED LI_BIG_ENDIAN OR LI_LITTLE_ENDIAN
#endif

/* 从盘间通信数据的BUF中获取N字节 */
#define LI_GET_BUF(val_point, point, len, residual_length)         \
        do                                                          \
        {                                                           \
            if((residual_length) >= (len))                          \
            {                                                       \
                if((NULL != (val_point)) && (NULL != (point)))     \
                {                                                   \
                    OS_MEMCPY((val_point), (point), (len));         \
                    (point) = (point) + (len);                      \
                    (residual_length) = (residual_length) - (len);  \
                }                                                   \
            }                                                       \
        }while(0)

#define LI_WRITE_BUF(point, val_point, len, residual_length)                \
        do                                                                  \
        {                                                                   \
            if((residual_length) >= (len))                          \
            {                                                               \
                if((NULL != (point)) && (NULL != (val_point)))              \
                {                                                               \
                    OS_MEMCPY((point), (val_point), (len));        \
                    (point) = (point) + (len);              \
                    (residual_length) = (residual_length) - (len);  \
                }                                                           \
            }                                                               \
        }while( 0 )                                                         

/* 从盘间通信数据的BUF中获取4字节 */
#define LI_GET_ULONG(val, point, residual_length)                  \
        do                                                          \
        {                                                           \
            if((residual_length) >= 4)                              \
            {                                                       \
                (val) = LI_NTOHL(*((uint32_t *)(point)));    \
                (point) = (point) + 4;                              \
                (residual_length) = (residual_length) - 4;          \
            }                                                       \
        }while(0)
        
/* 从盘间通信数据的BUF中获取2字节 */
#define LI_GET_USHORT(val, point, residual_length)                 \
        do                                                          \
        {                                                           \
            if((residual_length) >= 2)                              \
            {                                                       \
                (val) = LI_NTOHS(*((uint16_t *)(point)));   \
                (point) = (point) + 2;                              \
                (residual_length) = (residual_length) - 1;          \
            }                                                       \
        }while(0)

/* 从盘间通信数据的BUF中获取1字节 */
#define LI_GET_UCHAR(val, point, residual_length)          \
        do                                                  \
        {                                                   \
            if((residual_length) >= 2)                      \
            {                                               \
                (val) = *((uint8_t *)(point));        \
                (point)++;                                  \
                (residual_length)--;                        \
            }                                               \
        }while(0)

/* 忽略N字节 */
#define LI_IGNORE_BYTES(num, point, residual_length)           \
        do                                                      \
        {                                                       \
            if((residual_length) >= (num))                      \
            {                                                   \
                (point) = (point) + (num);                      \
                (residual_length) = (residual_length) - (num);  \
            }                                                   \
        }while(0)

/* escape sequence code打印控制 */
#if (defined (linux) || defined (__CYGWIN__)) && !defined(NOT_PRT_COLOR)
#define LI_COLOR_RED       "\33[1;31m"             /* 加粗，红色字体 */
#define LI_COLOR_MAGENTA   "\33[1;35m"             /* 加粗，洋红色字体 */
#define LI_COLOR_YELLOW    "\33[4;33m"             /* 下划线，黄色字体 */
#define LI_COLOR_CYAN      "\33[0;36m"             /* 无特效，蓝色字体 */
#define LI_COLOR_GREEN     "\033[0;32m"            /* 无特效，绿色字体 */
#define LI_COLOR_NORMAL    "\033[0m"              /* 无特效，默认颜色 */
#else   /* windows printf无颜色控制 */
#define LI_COLOR_RED       ""
#define LI_COLOR_MAGENTA   ""
#define LI_COLOR_YELLOW    ""
#define LI_COLOR_CYAN      ""
#define LI_COLOR_GREEN     ""
#define LI_COLOR_NORMAL    ""
#endif

#define LI_LOG_DIR         "li.log"             /* log日志路径 */

#define LI_LOG_FILE(level, logfile, fmt, args...)   \
    li_log_file(__FILE__, __FUNCTION__, __LINE__, level, logfile, fmt, ##args)

/*************** bmulib信息打印级别***************/
#ifndef BMU_CRIT
#define BMU_CRIT                1       /* critical conditions */
#endif
#ifndef BMU_ERR
#define BMU_ERR                 2       /* error conditions */
#endif
#ifndef BMU_WARNING
#define BMU_WARNING             3       /* warning conditions */
#endif
#ifndef BMU_NOTICE
#define BMU_NOTICE              4       /* normal but significant condition */
#endif
#ifndef BMU_INFO
#define BMU_INFO                5       /* informational */
#endif
#ifndef BMU_DEBUG
#define BMU_DEBUG               6       /* debug-level messages */
#endif

#define LI_SHELL_LEVEL     (0x100)                     /* 仅在shell上打印 */
#define LI_SHELL_CRIT      (LI_SHELL_LEVEL + 1)      /* 在shell上打印critical级别 */
#define LI_SHELL_ERR       (LI_SHELL_LEVEL + 2)      /* 在shell上打印error级别 */
#define LI_SHELL_WARNING   (LI_SHELL_LEVEL + 3)      /* 在shell上打印warning级别 */
#define LI_SHELL_NOTICE    (LI_SHELL_LEVEL + 4)      /* 在shell上打印normal but significant级别 */
#define LI_SHELL_INFO      (LI_SHELL_LEVEL + 5)      /* 在shell上打印informational级别 */
#define LI_SHELL_DEBUG     (LI_SHELL_LEVEL + 6)      /* 在shell上打印debug级别 */

/* FPGA仿真日志 */
#define LI_LOG(level, fmt, args...)                                        \
        do                                                                  \
        {                                                                   \
            switch (level)                                                  \
            {                                                               \
                case LI_SHELL_CRIT:                                        \
                    printf(LI_COLOR_RED fmt LI_COLOR_NORMAL, ##args);     \
                    break;                                                  \
                case LI_SHELL_ERR:                                         \
                    printf(LI_COLOR_MAGENTA fmt LI_COLOR_NORMAL, ##args); \
                    break;                                                  \
                case LI_SHELL_WARNING:                                     \
                    printf(LI_COLOR_YELLOW fmt LI_COLOR_NORMAL, ##args);  \
                    break;                                                  \
                case LI_SHELL_NOTICE:                                      \
                    printf(LI_COLOR_CYAN fmt LI_COLOR_NORMAL, ##args);    \
                    break;                                                  \
                case LI_SHELL_INFO:                                        \
                    printf(LI_COLOR_GREEN fmt LI_COLOR_NORMAL, ##args);   \
                    break;                                                  \
                case LI_SHELL_DEBUG:                                       \
                    printf(fmt, ##args);                                    \
                    break;                                                  \
                default:                                                    \
                    LI_LOG_FILE(level, LI_LOG_DIR, fmt, ##args);            \
            }                                                               \
        }                                                                   \
        while(0)

/* 检查指针参数，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_POINT(log_level, point_param, ret)                  \
    if (NULL == (point_param))                                              \
    {                                                                       \
        LI_LOG((log_level), #point_param" is NULL\n");                   \
        (ret) = PARAM_ERROR;                                                \
        break;                                                              \
    }

/* 检查参数的最大值(闭区间)，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_MAX(log_level, param, max, param_format, max_format, ret)               \
    if ((param) > (max))                                                                        \
    {                                                                                           \
        LI_LOG((log_level),#param":"param_format" > max:"max_format"\n", (param), (max));    \
        (ret) = PARAM_ERROR;                                                                    \
        break;                                                                                  \
    }

/* 检查参数的最大值(开区间)，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_OPEN_MAX(log_level, param, max, param_format, max_format, ret)          \
    if ((param) >= (max))                                                                       \
    {                                                                                           \
        LI_LOG((log_level), #param":"param_format" >= max:"max_format"\n", (param), (max));  \
        (ret) = PARAM_ERROR;                                                                    \
        break;                                                                                  \
    }

/* 检查参数的最小值(闭区间)，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_MIN(log_level, param, min, param_format, min_format, ret)               \
    if ((param) < (min))                                                                        \
    {                                                                                           \
        LI_LOG((log_level), #param":"param_format" < min:"min_format"\n", (param), (min));   \
        (ret) = PARAM_ERROR;                                                                    \
        break;                                                                                  \
    }

/* 检查参数的最小值(开区间)，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_OPEN_MIN(log_level, param, min, param_format, min_format, ret)          \
    if ((param) <= (min))                                                                       \
    {                                                                                           \
        LI_LOG((log_level), #param":"param_format" <= min:"min_format"\n", (param), (min));  \
        (ret) = PARAM_ERROR;                                                                    \
        break;                                                                                  \
    }


/* 检查参数的值是否相等，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_EQUAL(log_level, param1, param2, param1_format, param2_format, ret)          \
    if ((param1) == (param2))                                                                       \
    {                                                                                           \
        LI_LOG((log_level), #param1":"param1_format" != "#param2":"param2_format"\n", (param1), (param2));  \
        (ret) = PARAM_ERROR;                                                                    \
        break;                                                                                  \
    }

/* 检查参数的值是否不等，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_CHECK_PARAM_NOT_EQUAL(log_level, param1, param2, param1_format, param2_format, ret)          \
        if ((param1) != (param2))                                                                       \
        {                                                                                           \
            LI_LOG((log_level), #param1":"param1_format" != "#param2":"param2_format"\n", (param1), (param2));  \
            (ret) = PARAM_ERROR;                                                                    \
            break;                                                                                  \
        }

/* 检查参数的条件是否成立，配合do{}while(0);使用，LOG建议使用ERR级别 */
#define LI_ASSERT(log_level, condition, ret)                    \
    if (!(condition))                                           \
    {                                                           \
        LI_LOG((log_level), GET_MACRO_NAME(condition)"fail\n"); \
        (ret) = PARAM_ERROR;                                    \
        break;                                                  \
    }


/* 检查返回值是FPGA仿真错误码LI_RESULT的函数执行结果(不成功退出)，配合do{}while(0);使用，LOG建议使用WARNING级别 */
#define LI_CHECK_FUNC_RETURN_BREAK(log_level, func_name, ret)                  \
    if(CFG_OK != (ret))                                                         \
    {                                                                           \
        LI_LOG((log_level), #func_name" return %s\n", str_result(ret));    \
        break;                                                                  \
    }

/* 检查返回值是FPGA仿真错误码LI_RESULT的函数执行结果(不成功不退出)，LOG建议使用NOTICE级别 */
#define LI_CHECK_FUNC_RETURN_NO_BREAK(log_level, func_name, ret)               \
    if(CFG_OK != (ret))                                                         \
    {                                                                           \
        LI_LOG((log_level), #func_name" return %s\n", str_result(ret));    \
        (ret) = CFG_OK;                                                         \
    }

/* 检查外部函数执行结果(不成功退出)，配合do{}while(0);使用，LOG建议使用WARNING级别 */
#define LI_CHECK_EXTERN_FUNC_RETURN_BREAK(log_level, func_name, func_ret, func_ret_format, ret)    \
    if(CFG_OK != (func_ret))                                                                        \
    {                                                                                               \
        LI_LOG((log_level), #func_name" return:"func_ret_format"\n", (func_ret));                \
        (ret) = FUNC_RET_ERR;                                                                       \
        break;                                                                                      \
    }

/* 检查外部函数执行结果(不成功不退出)，LOG建议使用NOTICE级别 */
#define LI_CHECK_EXTERN_FUNC_RETURN_NO_BREAK(log_level, func_name, func_ret, func_ret_format)      \
    if(CFG_OK != (func_ret))                                                                        \
    {                                                                                               \
        LI_LOG((log_level), #func_name" return:"func_ret_format"\n", (func_ret));                \
    }

/* 检查返回值为NULL的函数执行结果(不成功退出)，配合do{}while(0);使用，LOG建议使用WARNING级别 */
#define LI_CHECK_NULL_FUNC_BREAK(log_level, func_name, func_ret, ret)          \
    if(NULL == (func_ret))                                                      \
    {                                                                           \
        LI_LOG((log_level), #func_name" return null\n");                     \
        (ret) = FUNC_RET_ERR;                                                   \
        break;                                                                  \
    }

/* 检查返回值为NULL的函数执行结果(不成功不退出)，LOG建议使用NOTICE级别 */
#define LI_CHECK_NULL_FUNC_NO_BREAK(log_level, func_name, func_ret)            \
    if(NULL == (func_ret))                                                      \
    {                                                                           \
        LI_LOG((log_level), #func_name" return null\n");                     \
    }

/* 检查FPGA仿真错误码LI_RESULT是否成功，配合do{}while(0) */
#define LI_CHECK_RETURN(ret)   \
    if(CFG_OK != ret)           \
    {                           \
        break;                  \
    }

/* 判断输入条件是否成立，如果成立则打印OK */
#define LI_UNIT_TEST_ASSERT(condition)                                     \
    do                                                                          \
    {                                                                           \
        if(condition)                                                           \
        {                                                                       \
            LI_LOG(LI_SHELL_INFO, "[ OK ] ");                          \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            LI_LOG(LI_SHELL_CRIT, "[FAIL] ");                       \
        }                                                                       \
        LI_LOG(LI_SHELL_DEBUG, GET_MACRO_NAME(condition) "\n");     \
    }while(0)

#define LI_IGNORE_VAL(VAL)  (VAL) = (VAL)


extern void li_common_init();
extern LI_RESULT li_log_buf(IN int log_level, IN uint8_t *buf, IN uint32_t buf_len);
extern int li_log_file(IN const char *file,
             IN const char *fun,
             IN const int line,
             IN int level,
             IN const char *logfile,
             IN const char *fmt, ...);
extern bool li_process_repeat_log(IN const char *file,
             IN const char *fun,
             IN const int line,
             IN int level,
             IN const char *logfile,
             IN const char *buf);
extern void li_set_debug(uint8_t print_level, uint8_t log_level, uint8_t color_enable);
extern void li_wlog();
extern const char *str_result(LI_RESULT result);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LI_COMMON_H__ */

