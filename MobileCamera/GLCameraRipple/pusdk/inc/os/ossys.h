#ifndef __OSSYS_H__
#define __OSSYS_H__

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 初始化OS模块，推荐size=65536, 返回0表示成功，-1表示失败 */
int32_t os_sys_init( int32_t size );

/* 关闭OS模块 */
void os_sys_exit();

/* 分配内存 */
void* os_sys_malloc( int32_t size, const char *file, int32_t line );

/* 释放内存 */
void os_sys_free( void *buf, const char *file, int32_t line );

/* 打印信息 */
void os_sys_print( const char* file, int32_t line, const char* fmt, ... );

/* 延时(us) */
void os_sys_usleep( int32_t us );

/* 取得开机至当前时间(ms) */
uint32_t os_sys_get_time();

/* 取得开机至当前时间(us) */
uint64_t os_sys_get_us();

/* DNS解析，返回IP地址，网络字节序 */
uint32_t os_sys_dns( const char *hostname );

/* 取得本机名称: 返回0表示成功,-1表示失败 */
int32_t os_sys_get_local_name( char *name, int size );

/* 断言 */
void os_sys_assert( int32_t flag, const char *file, int32_t line );

/* 计算时区(北京=8) */
int32_t os_sys_get_time_zone();


void os_sys_kill(int sig);

unsigned char os_sys_is_exit();

int32_t msleep(uint32_t millsecs);



//不区分大小写比较字符串的前n个字符
int32_t xstrncasecmp(const char* s1, const char* s2, size_t n);
//不区分大小比较字符串
int32_t xstrcasecmp(const char *s1, const char *s2);



#ifdef __cplusplus
}
#endif

#endif  /* __OSSYS_H__ */
