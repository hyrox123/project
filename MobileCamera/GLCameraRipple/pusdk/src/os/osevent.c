#include "stdafx.h"
#include "ossys.h"
#include "osevent.h"


/* 创建信号量 */
void* os_event_create(char *name)
{
    /*
	sem_t *sem;

	sem = (sem_t *)os_sys_malloc( sizeof(sem_t), __FILE__, __LINE__ );
	memset( sem, 0, sizeof(sem_t) );

	sem_init( sem, 0, 0 );
   */
    
    sem_t *sem = sem_open(name, O_CREAT, 0664, 0);
	return sem;
}

/* 销毁信号量 */
void os_event_destroy(char* name)
{
    /*
	sem_t *sem = (sem_t *)signal;

	sem_destroy( sem );

	os_sys_free( sem, __FILE__, __LINE__ );
     */
    
    sem_unlink(name);
}

/* 等待信号量, time(ms)=-1表示无限等待 */
int32_t os_event_wait( void* signal, int32_t timeout )
{
	sem_t *sem = (sem_t *)signal;

	if( timeout < 0 )
	{
		return sem_wait( sem );
	}
	else
	{
#if 0
        struct timespec ts;
		ts.tv_sec = time(NULL) + timeout/1000;
		ts.tv_nsec = (timeout%1000)*1000;
		if( sem_timedwait( sem, (const struct timespec *)&ts ) != 0 )
			return -1;
#else
        return sem_wait( sem );
#endif
	}
	return 0;
}

/* 点亮信号量 */
void os_event_give( void* signal )
{
	sem_t *sem = (sem_t *)signal;

	sem_post( sem );
}
