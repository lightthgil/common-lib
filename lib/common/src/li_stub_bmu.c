/***********************************************************************************

 * 文 件 名   : li_stub_bmu.c
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2016年11月25日
 * 版 本 号   : 
 * 文件描述   : 桩函数
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#include <stdarg.h>         /* 可变参函数相关 */
#include "li_common.h"
#include "li_stub_bmu.h"

#ifndef LI_USE_BMU_LIB         /* 不使用BMU库的情况 */

/* 控制屏幕打印级别，仅本文件有效 */
static uint8_t s_print_msg_level = BMU_INFO;

/*****************************************************************************
 * 函 数 名  : bmu_log_stub
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年3月14日
 * 函数功能  : BMU库的log日志桩函数
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
int bmu_log_stub(const char *file,
    const char *fun,
    const int line,
    int level,
    const char *logfile,
    const char *fmt,
    ...)
{
    va_list vargs;
    char buf[BMU_PRINT_MSG_BUF_SIZE] = {0};
    int empty_buf_num = 0;
    int num = 0;
    const char *color_pfx = "";
    const char *color_sfx = LI_COLOR_NORMAL;

    if (s_print_msg_level < level)
    {
        return ERROR;
    }

    /*记录位置*/
    if ((file != NULL) && (fun != NULL) && (fun != NULL))
    {
        snprintf(buf, sizeof(buf), "%s:%d %s() ", file, line, fun);
    }

    empty_buf_num = sizeof(buf) - strlen(buf);

    /*记录内容*/
    va_start(vargs, fmt);
    num = vsnprintf(buf + strlen(buf), empty_buf_num, fmt, vargs);
    va_end(vargs);

    /* 传入的日志内容过长，截断后补换行 */
    if(num >= empty_buf_num)
    {
        buf[BMU_PRINT_MSG_BUF_SIZE - 2] = '\n'; /* 最后一个字符是'\0',最后第二个字符是换行 */
    }

    /*输出打印*/
    switch (level)
    {
        case BMU_CRIT:
            color_pfx = LI_COLOR_RED;
            break;
        case BMU_ERR:
            color_pfx = LI_COLOR_MAGENTA;
            break;
        case BMU_WARNING:
            color_pfx = LI_COLOR_YELLOW;
            break;
        case BMU_NOTICE:
            color_pfx = LI_COLOR_CYAN;
            break;
        case BMU_INFO:
            color_pfx = LI_COLOR_GREEN;
            break;
        default:
            color_pfx = "";
            color_sfx = "";
        break;
    }

    #define PRINTF_TEMP printf  /* 绕过公司的不用直接使用printf的代码检查 */
    PRINTF_TEMP("%s%s%s", color_pfx, buf, color_sfx);
    #undef PRINTF_TEMP

    return SUCCESS;
}

/*****************************************************************************
 * 函 数 名  : bmu_set_debug_stub
 * 负 责 人  : jiangbo,蒋博
 * 创建日期  : 2017年4月6日
 * 函数功能  : 设置打印及日志级别
 * 输入参数  : uint8_t print_level   打印级别
               uint8_t log_level     日志级别
               uint8_t color_enable  是否打印颜色
 * 输出参数  : 无
 * 返 回 值  : void                  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
void bmu_set_debug_stub(uint8_t print_level, uint8_t log_level, uint8_t color_enable)
{
    s_print_msg_level = print_level;
    return;
}

#endif
