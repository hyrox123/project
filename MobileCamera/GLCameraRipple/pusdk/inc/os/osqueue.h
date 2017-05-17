#ifndef __OSQUEUE_H__
#define __OSQUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* ��ʼ��ϵͳ��Ϣ���� */
void* os_queue_create( int32_t msg_size, int32_t msg_num );

/* �ͷŶ�����Դ */
void os_queue_destroy( void* queue );

/* ��ϵͳ��Ϣ���������Ϣ,0=sucess,-1=failed */
int32_t os_queue_post( void* queue, size_t type, void *buf, int32_t size );

/* ��ϵͳ��Ϣ������ȡ��Ϣ,0=sucess,-1=failed */
int32_t os_queue_read( void* queue, size_t *type, void *buf, int32_t size );

/* ��ϵͳ��Ϣ������ȡ��Ϣ,0=sucess,-1=failed */
int32_t os_queue_read_timeout( void* queue, size_t *type, void *buf, int32_t size, int32_t timeout );

/* �����Ϣ���������е���Ϣ */
void os_queue_clear( void* queue );

/* ȡ��ϵͳ��Ϣ������Ϣ��Ŀ */
int32_t os_queue_get_num( void* queue );


#ifdef __cplusplus
}
#endif

#endif /* __OSQUEUE_H__ */
