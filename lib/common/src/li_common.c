/***********************************************************************************

 * 文 件 名   : li_common.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月14日
 * 版 本 号   : 
 * 文件描述   : 通用代码
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include <stdarg.h>         /* 可变参函数相关 */
#ifndef LI_USE_BMU_LIB
#include "li_stub_bmu.h"
#else
#include "bmuInclude.h"
#endif
#include "li_common.h"


/*****************************************************************************
 * 函 数 名  : str_result
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月21日
 * 函数功能  : 获取错误码的字符串
 * 输入参数  : LI_RESULT result  错误码
 * 输出参数  : 无
 * 返 回 值  : const char *      翻译成字符串的错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
const char *str_result(LI_RESULT result)
{
    /* 模块错误码解析 */
    const char * const result_str[] = {
        GET_MACRO_NAME(CFG_OK),                 /* 增加或删除条目操作成功 */
        GET_MACRO_NAME(PARAM_ERROR),            /* 参数错误 */
        GET_MACRO_NAME(KEY_NOT_EQUAL_MYID),     /* key值错误，与本地描述符不相等 */
        GET_MACRO_NAME(KEY_NOT_EXIST),          /* KEY值不存在 */
        GET_MACRO_NAME(SET_DRIVE_ERROR),        /* 写驱动失败 */
        GET_MACRO_NAME(ALLOC_ERROR),            /* 申请内存空间时失败 */
        GET_MACRO_NAME(ADD_KEY_EXIST),          /* 该key已存在 */
        GET_MACRO_NAME(CFG_FULL),               /* 配置已满 */
        GET_MACRO_NAME(FUC_NULL),               /* 回调函数未注册 */
        GET_MACRO_NAME(INSERT_ERR),             /* 配置树中增加节点失败 */
        GET_MACRO_NAME(FORCED_END),             /* 强制终结，一般调试用，不执行函数实现，直接返回 */
        GET_MACRO_NAME(FRAME_TYPE_ERR),         /* 发帧类型错误或不支持 */
        GET_MACRO_NAME(SET_ARAD),               /* 写ARAD错误 */
        GET_MACRO_NAME(FPGA_STORT_NULL),        /* 未分配存储FPGA的数据空间 */
        GET_MACRO_NAME(KEY_NOT_MATCH),          /* KEY值不匹配 */
        GET_MACRO_NAME(FPGA_MYID_TREE),         /* 操作FPGA使用的二叉树失败 */
        GET_MACRO_NAME(IP_TYPE_ERR),            /* IP类型错误 */
        GET_MACRO_NAME(FUNC_RET_ERR),           /* 调用其他接口返回错误 */
        GET_MACRO_NAME(LIST_NULL),              /* 链表为空 */
        GET_MACRO_NAME(CFG_RESULT_MAX)};

    if(result >= sizeof(result_str)/sizeof(result_str[0]))
    {
        return "unknown";
    }
    else
    {
        return result_str[result];
    }
}


/*****************************************************************************
 * 函 数 名  : li_common_init
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月24日
 * 函数功能  : 加载动态库用
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_common_init()
{
    return;
}

/*****************************************************************************
 * 函 数 名  : li_log_buf
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月14日
 * 函数功能  : 打印内存
 * 输入参数  : IN int log_level     打印级别
               IN uint8_t *buf      需要打印的内存
               IN uint32_t buf_len  需要打印的内存长度
 * 输出参数  : 无
 * 返 回 值  : LI_RESULT            错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
LI_RESULT li_log_buf(IN int log_level, IN uint8_t *buf, IN uint32_t buf_len)
{
    LI_RESULT ret = CFG_OK;
    uint32_t counter = 0;
    const uint32_t line_feed_len = 0xf;   /* 一行打印16个 */
    int addr_pri_len = 2;

    do /* 开始循环 */
    {
        LI_CHECK_PARAM_POINT(log_level, buf, ret);
        
        /* 打印出来的地址值的长度 */
        if(buf_len <= UINT8_MAX)
        {
            addr_pri_len = 2;
        }
        else if(buf_len <= UINT16_MAX)
        {
            addr_pri_len = 4;
        }
        else
        {
            addr_pri_len = 8;
        }
        
        for (counter = 0; counter < buf_len; counter++)
        {
            /* 打印地址 */
            if (0 == (counter & line_feed_len))
            {
                LI_LOG(log_level, "0x%0*"PRIx32":  ", addr_pri_len, counter);
            }
        
            /* 打印内存 */
            LI_LOG(log_level, "%02x  ", buf[counter]);
            
            if (!((counter + 1) & line_feed_len))
            {
                LI_LOG(log_level, "\n");
            }
        }
        LI_LOG(log_level, "\n");
    } while ( 0 ); /* 循环直到0不成立 */

    return ret;
}

/*****************************************************************************
 * 函 数 名  : li_log_file
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月29日
 * 函数功能  : 封装打印文件，在C文件中决定使用哪个库，即由库决定实现，外面模块引用头文件不用传入选择宏
 * 输入参数  : const char *file     文件名
               const char *fun      函数名
               const int line       行号
               int level            打印级别
               const char *logfile  log文件位置
               const char *fmt      打印格式
               ...                  打印内容
 * 输出参数  : 无
 * 返 回 值  : int                  错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
int li_log_file(IN const char *file,
    IN const char *fun,
    IN const int line,
    IN int level,
    IN const char *logfile,
    IN const char *fmt, ...)
{
    va_list vargs;
    char buf[BMU_PRINT_MSG_BUF_SIZE] = {0};
    int num = 0;
    
    va_start(vargs, fmt);
    num = vsnprintf(buf, sizeof(buf), fmt, vargs);  /* 返回值<0错误，否则是需要写入的长度(不是实际写入的长度) */
    va_end(vargs);

    if(num < 0)
    {
        return num;
    }
    else
    {        
        /* 比较是否和上次记录的LOG重复，合并重复的LOG */
        if(li_process_repeat_log(file, fun, line, level, logfile, buf))
        {
            return CFG_OK;
        }
        else
        {        
            return BmuLog(file, fun, line, level, logfile, "%s", buf);
        }
    }
}

/*****************************************************************************
 * 函 数 名  : li_log_file
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年7月3日
 * 函数功能  : 比较是否和上次记录的LOG重复，合并重复的LOG
 * 输入参数  : const char *file     文件名
               const char *fun      函数名
               const int line       行号
               int level            打印级别
               const char *logfile  log文件位置
               const char *buf      需要记录的LOG，必须是字符串格式，以'\0'结尾
 * 输出参数  : 无
 * 返 回 值  : boll                 是否和上次记录的LOG重复
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
bool li_process_repeat_log(IN const char *file,
    IN const char *fun,
    IN const int line,
    IN int level,
    IN const char *logfile,
    IN const char *buf)
{
    static char s_last_buf[BMU_PRINT_MSG_BUF_SIZE] = {0};
    static uint64_t s_repeat_num = 0;
    static int s_last_level = 0;
    
    if ((0 == strcmp(s_last_buf, buf)) && (s_last_level == level))
    {
        ++s_repeat_num;
        return true;
    }
    else
    {
        if(s_repeat_num != 0)
        {
            BmuLog(file, fun, line, s_last_level, logfile, "[%"PRIu64"*]%s", s_repeat_num, s_last_buf);
        }
        s_repeat_num = 0;
        s_last_level = level;
        strcpy(s_last_buf, buf);
    }

    return false;
}

/*****************************************************************************
 * 函 数 名  : li_wlog
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2017年7月3日
 * 函数功能  : 同步内存的LOG到文件,立刻写文件
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : void无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_wlog()
{
    LI_LOG(BMU_CRIT, "[LILOG]Sync msg log in memory to file\n");
    return;
}

/*****************************************************************************
 * 函 数 名  : li_set_debug
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月6日
 * 函数功能  : 封装设置打印及日志级别，在C文件中决定使用哪个库，即由库决定实现，外面模块引用头文件不用传入选择宏
 * 输入参数  : uint8_t print_level   打印级别
               uint8_t log_level     日志级别
               uint8_t color_enable  是否打印颜色
 * 输出参数  : 无
 * 返 回 值  : void                  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void li_set_debug(uint8_t print_level, uint8_t log_level, uint8_t color_enable)
{
    #ifndef LI_USE_BMU_LIB  /* NFV的BMU没有移植setdebug，暂时屏蔽，当BMU移植成功后再放开 */
    return setdebug(print_level, log_level, color_enable);
    #else
    return;
    #endif
}

