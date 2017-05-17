#ifndef __OSMUTEX_H__
#define __OSMUTEX_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* ���������� */
void* os_mutex_create();

/* ���ٻ����� */
void os_mutex_destroy( void* mutex );

/* ����, timeout(ms)=-1��ʾ���޵ȴ�������0��ʾ�ɹ�������-1��ʾʧ�� */
int32_t os_mutex_lock( void* mutex, int32_t timeout );

/* ���� */
void os_mutex_unlock( void* mutex );


#ifdef __cplusplus
}
#endif

#endif  /* __OSMUTEX_H__ */
