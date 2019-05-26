/***********************************************************************************

 * 文 件 名   : li_stub_bmu.h
 * 负 责 人   : jiangbo,蒋博
 * 创建日期   : 2017年3月13日
 * 版 本 号   : 
 * 文件描述   : li_stub_bmu.c 的头文件
 * 版权说明   : Copyright (C) 2000-2016   
 * 其    他   : 
 * 修改日志   : 

***********************************************************************************/

#ifndef __LI_STUB_H__
#define __LI_STUB_H__

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#ifndef LI_USE_BMU_LIB
#define BmuLog bmu_log_stub
#define setdebug bmu_set_debug_stub



#define BMU_PRINT_MSG_BUF_SIZE  512

#define BMU_LOG(level, logfile, fmt, args...)   BmuLog(__FILE__, __FUNCTION__, __LINE__, level, logfile, fmt, ##args)


extern int bmu_log_stub(const char *file,
             const char *fun,
             const int line,
             int level,
             const char *logfile,
             const char *fmt,
             ...);
extern void bmu_set_debug_stub(uint8_t print_level, uint8_t log_level, uint8_t color_enable);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LI_STUB_H__ */
