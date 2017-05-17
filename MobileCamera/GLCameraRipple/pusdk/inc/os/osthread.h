#ifndef __OSTHREAD_H__
#define __OSTHREAD_H__

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* 推荐线程堆栈大小(bytes) */
#define THREAD_DEFAULT_STACK_SIZE	65536
/* 推荐线程优先级 */
#define THREAD_DEFAULT_PRIORIOTY	100


/* 线程回调函数，会被反复调用,返回<0表示退出线程，>0表示延时时间(ms) */
typedef int32_t (*PThreadProc)( uint32_t param );

/* 创建线程，成功后处于暂停状态，priority=0(最高), priority=255(最低) */
void* os_thread_create( char *name, unsigned char priority, int32_t stack_size, PThreadProc proc, uint32_t param );

/* 销毁线程(只是试图而不是强制销毁线程)，返回0表示成功,-1表示失败 */
int32_t os_thread_try_destroy( void* thread, int32_t timeout );

/* 销毁线程(如果试图失败则会强制销毁)，返回0表示成功,-1表示失败 */
int32_t os_thread_destroy( void* thread, int32_t timeout );

/* 修改线程名称 */
void os_thread_set_name( void* thread, char* name );

/* 获取线程名称 */
void os_thread_get_name( void* thread, char* buf, int32_t size );

/* 设置线程优先级，priority的范围为[0,255]，0表示优先级最高，255最低 */
void os_thread_set_priority( void* thread, unsigned char priority );

/* 取得线程优先级，priority的范围为[0,255]，0表示优先级最高，255最低 */
unsigned char os_thread_get_priority( void* thread );

/* 修改线程执行函数 */
void os_thread_set_proc( void* thread, PThreadProc proc, uint32_t param );

/* 取得线程执行函数 */
void os_thread_get_proc( void* thread, PThreadProc *proc, uint32_t *param );

/* 暂停 */
void os_thread_pause( void* thread );

/* 继续 */
void os_thread_resume( void* thread );

/* 是否正在运行:1=是,0=否 */
int32_t os_thread_is_running( void* thread );

/* 是否在用户执行函数中没有返回，1=yes,0=no */
int32_t os_thread_is_busy( void* thread );


#ifdef __cplusplus
}
#endif

#endif  /* __OSTHREAD_H__ */
