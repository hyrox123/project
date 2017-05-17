#include "stdafx.h"
#include "ossys.h"
#include "osmutex.h"
#include <pthread.h>

/* 创建互斥量 */
void* os_mutex_create()
{
	int ret = 0;
	pthread_mutex_t *mt;
    pthread_mutexattr_t pma;

	mt = (pthread_mutex_t*)os_sys_malloc( sizeof(pthread_mutex_t), __FILE__, __LINE__ );
	if( mt )
	{
		memset( mt, 0, sizeof(pthread_mutex_t) );

		ret |= pthread_mutexattr_init( &pma );
        
	//	ret |= pthread_mutexattr_settype( &pma, PTHREAD_MUTEX_ERRORCHECK_NP );
		ret |= pthread_mutex_init( mt, &pma );
		ret |= pthread_mutexattr_destroy( &pma );

		//pthread_mutex_init( mt, NULL );
	}
	return mt;
}

/* 销毁互斥量 */
void os_mutex_destroy( void* mutex )
{
	pthread_mutex_t *mt = (pthread_mutex_t *)mutex;

	pthread_mutex_destroy( mt );

	os_sys_free( mt, __FILE__, __LINE__ );
    mt = NULL;
}

/* 上锁, timeout(ms)=-1表示无限等待，返回0表示成功，返回-1表示失败 */
int32_t os_mutex_lock( void* mutex, int32_t timeout )
{
	uint32_t start, now;

	if( timeout <= 0 )
	{
		return pthread_mutex_lock( (pthread_mutex_t *)mutex );
	}
	else
	{
		now = start = os_sys_get_time();
		while( now < start + timeout && start <= now )
		{
			if( pthread_mutex_trylock( (pthread_mutex_t *)mutex ) == 0 )
				return 0;
			os_sys_usleep( 1000 );
            //printf("while\n");
		}
	}
	return -1;
}

/* 解锁 */
void os_mutex_unlock( void* mutex )
{
	pthread_mutex_unlock( (pthread_mutex_t *)mutex );
    

}
