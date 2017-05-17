#ifndef __OSTHREAD_H__
#define __OSTHREAD_H__

#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* �Ƽ��̶߳�ջ��С(bytes) */
#define THREAD_DEFAULT_STACK_SIZE	65536
/* �Ƽ��߳����ȼ� */
#define THREAD_DEFAULT_PRIORIOTY	100


/* �̻߳ص��������ᱻ��������,����<0��ʾ�˳��̣߳�>0��ʾ��ʱʱ��(ms) */
typedef int32_t (*PThreadProc)( uint32_t param );

/* �����̣߳��ɹ�������ͣ״̬��priority=0(���), priority=255(���) */
void* os_thread_create( char *name, unsigned char priority, int32_t stack_size, PThreadProc proc, uint32_t param );

/* �����߳�(ֻ����ͼ������ǿ�������߳�)������0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_thread_try_destroy( void* thread, int32_t timeout );

/* �����߳�(�����ͼʧ�����ǿ������)������0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_thread_destroy( void* thread, int32_t timeout );

/* �޸��߳����� */
void os_thread_set_name( void* thread, char* name );

/* ��ȡ�߳����� */
void os_thread_get_name( void* thread, char* buf, int32_t size );

/* �����߳����ȼ���priority�ķ�ΧΪ[0,255]��0��ʾ���ȼ���ߣ�255��� */
void os_thread_set_priority( void* thread, unsigned char priority );

/* ȡ���߳����ȼ���priority�ķ�ΧΪ[0,255]��0��ʾ���ȼ���ߣ�255��� */
unsigned char os_thread_get_priority( void* thread );

/* �޸��߳�ִ�к��� */
void os_thread_set_proc( void* thread, PThreadProc proc, uint32_t param );

/* ȡ���߳�ִ�к��� */
void os_thread_get_proc( void* thread, PThreadProc *proc, uint32_t *param );

/* ��ͣ */
void os_thread_pause( void* thread );

/* ���� */
void os_thread_resume( void* thread );

/* �Ƿ���������:1=��,0=�� */
int32_t os_thread_is_running( void* thread );

/* �Ƿ����û�ִ�к�����û�з��أ�1=yes,0=no */
int32_t os_thread_is_busy( void* thread );


#ifdef __cplusplus
}
#endif

#endif  /* __OSTHREAD_H__ */
