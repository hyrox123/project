#ifndef __OSTHREADPOOL_H__
#define __OSTHREADPOOL_H__

#include "osthread.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* 创建线程池 */
void* os_thread_pool_create( int32_t maxnum );

/* 销毁线程池 */
void os_thread_pool_destroy( void* pool, int32_t timeout );

/* 从线程池中分配一个线程 */
void* os_thread_pool_alloc( void* pool, PThreadProc proc, uint32_t param, int32_t timeout );

/* 释放一个线程到线程池中，不会强制关闭线程 */
void os_thread_pool_try_free( void* pool, void* thread, int32_t timeout );

/* 释放一个线程到线程池中，如果不能从用户函数中退出的话，则会强制关闭线程 */
void os_thread_pool_free( void* pool, void* thread, int32_t timeout );

/* 打印线程池信息 */
void os_thread_pool_print( void* pool );



#ifdef __cplusplus
}
#endif


#endif /* __OSTHREADPOOL_H__ */
