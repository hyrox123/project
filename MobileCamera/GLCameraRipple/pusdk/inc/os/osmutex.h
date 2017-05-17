#ifndef __OSMUTEX_H__
#define __OSMUTEX_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* 创建互斥量 */
void* os_mutex_create();

/* 销毁互斥量 */
void os_mutex_destroy( void* mutex );

/* 上锁, timeout(ms)=-1表示无限等待，返回0表示成功，返回-1表示失败 */
int32_t os_mutex_lock( void* mutex, int32_t timeout );

/* 解锁 */
void os_mutex_unlock( void* mutex );


#ifdef __cplusplus
}
#endif

#endif  /* __OSMUTEX_H__ */
