#ifndef __OSTHREADPOOL_H__
#define __OSTHREADPOOL_H__

#include "osthread.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* �����̳߳� */
void* os_thread_pool_create( int32_t maxnum );

/* �����̳߳� */
void os_thread_pool_destroy( void* pool, int32_t timeout );

/* ���̳߳��з���һ���߳� */
void* os_thread_pool_alloc( void* pool, PThreadProc proc, uint32_t param, int32_t timeout );

/* �ͷ�һ���̵߳��̳߳��У�����ǿ�ƹر��߳� */
void os_thread_pool_try_free( void* pool, void* thread, int32_t timeout );

/* �ͷ�һ���̵߳��̳߳��У�������ܴ��û��������˳��Ļ������ǿ�ƹر��߳� */
void os_thread_pool_free( void* pool, void* thread, int32_t timeout );

/* ��ӡ�̳߳���Ϣ */
void os_thread_pool_print( void* pool );



#ifdef __cplusplus
}
#endif


#endif /* __OSTHREADPOOL_H__ */
