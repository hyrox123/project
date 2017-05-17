#ifndef __OSSYS_H__
#define __OSSYS_H__

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ��ʼ��OSģ�飬�Ƽ�size=65536, ����0��ʾ�ɹ���-1��ʾʧ�� */
int32_t os_sys_init( int32_t size );

/* �ر�OSģ�� */
void os_sys_exit();

/* �����ڴ� */
void* os_sys_malloc( int32_t size, const char *file, int32_t line );

/* �ͷ��ڴ� */
void os_sys_free( void *buf, const char *file, int32_t line );

/* ��ӡ��Ϣ */
void os_sys_print( const char* file, int32_t line, const char* fmt, ... );

/* ��ʱ(us) */
void os_sys_usleep( int32_t us );

/* ȡ�ÿ�������ǰʱ��(ms) */
uint32_t os_sys_get_time();

/* ȡ�ÿ�������ǰʱ��(us) */
uint64_t os_sys_get_us();

/* DNS����������IP��ַ�������ֽ��� */
uint32_t os_sys_dns( const char *hostname );

/* ȡ�ñ�������: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_sys_get_local_name( char *name, int size );

/* ���� */
void os_sys_assert( int32_t flag, const char *file, int32_t line );

/* ����ʱ��(����=8) */
int32_t os_sys_get_time_zone();


void os_sys_kill(int sig);

unsigned char os_sys_is_exit();

int32_t msleep(uint32_t millsecs);



//�����ִ�Сд�Ƚ��ַ�����ǰn���ַ�
int32_t xstrncasecmp(const char* s1, const char* s2, size_t n);
//�����ִ�С�Ƚ��ַ���
int32_t xstrcasecmp(const char *s1, const char *s2);



#ifdef __cplusplus
}
#endif

#endif  /* __OSSYS_H__ */
