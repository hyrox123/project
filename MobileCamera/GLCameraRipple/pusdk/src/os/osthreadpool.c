#include "stdafx.h"
#include "ossys.h"
#include "osmutex.h"
#include "osthreadpool.h"
#include "help.h"
#include <pthread.h>

typedef struct
{
	/* 所有线程 */
	void** thrds;
	/* 最大线程个数 */
	int32_t maxnum;
	/* 被杀死的线程个数 */
	int32_t killnum;
	/* 会话保护锁 */
	void* mutex;
}thead_pool_t;//线程池


/* 创建线程池 */
void* os_thread_pool_create( int32_t maxnum )
{
	thead_pool_t *tp;
	int32_t size;

	size = sizeof(thead_pool_t) + maxnum*sizeof(void*);
	tp = (thead_pool_t *)os_sys_malloc( size, __FILE__, __LINE__ );//[1]216
	if( tp )
	{
		memset( tp, 0, size );
		tp->maxnum = maxnum;
		tp->thrds = (void**)(tp + 1);
        
		tp->mutex = os_mutex_create();//[2]24
		if( tp->mutex == NULL )
		{
			os_sys_free( tp, __FILE__, __LINE__ );
			tp = NULL;
		}
	}
	return tp;
}

/* 销毁线程池 */
void os_thread_pool_destroy( void* pool, int32_t timeout )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	int32_t i;
	/* 销毁所有线程 */
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] )
			os_thread_destroy( tp->thrds[i], timeout );
        //printf("end,");
	}
	/* 销毁保护锁 */
	if( tp->mutex )
		os_mutex_destroy( tp->mutex );
	os_sys_free( pool, __FILE__, __LINE__ );
    pool = NULL;
}


/* 从线程池中分配一个线程 */
void* os_thread_pool_alloc( void* pool, PThreadProc proc, uint32_t param, int32_t timeout )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	static uint32_t cnt = 0;
	int32_t i;
	char name[32];
	void* thread = NULL;
	PThreadProc oldproc = NULL;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
		return NULL;

	/* 寻找一个没有被分配出去并且已经退出用户函数的线程 */
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] == NULL )
			continue;

		/* 根据线程执行函数是否有效来判断线程是否已经被分配出去 */
		oldproc = NULL;
		os_thread_get_proc( tp->thrds[i], &oldproc, NULL );

		if( oldproc == NULL && os_thread_is_busy( tp->thrds[i] ) == 0 )
		{
			thread = tp->thrds[i];
			os_thread_set_proc( tp->thrds[i], proc, param );
			break;
		}
	}

	/* 如果没有找到可用线程,并且没有达到最大数限制,则创建一个 */
	if( thread == NULL )
	{
		for( i=0; i<tp->maxnum; i++ )
		{
			if( tp->thrds[i] == NULL )
			{
				sprintf( name, "thread%d", ++cnt );
				thread = os_thread_create( name, THREAD_DEFAULT_PRIORIOTY, THREAD_DEFAULT_STACK_SIZE, proc, param );
                if(NULL == thread)
                {
                    LOG("[os_thread_pool_alloc][os_thread_create] failed\n");
                    printf("++++++++++++++++++++++++++++++++++++++++\n");
                    fprintf(stderr,"tp->maxnum %d",tp->maxnum);
                    //return NULL;
                    break;
                }
                //printf("thread name%s\n",name);   
				tp->thrds[i] = thread;
				break;
			}
		}
	}

	/* 线程数满，销毁还没有从用户函数中退出的线程（可能已经异常）,空出名额以创建新线程 */
	if( thread == NULL )
	{
		for( i=0; i<tp->maxnum; i++ )
		{
			if( tp->thrds[i] == NULL )
				continue;

			/* 根据线程执行函数是否有效来判断线程是否已经被分配出去 */
			oldproc = NULL;
			os_thread_get_proc( tp->thrds[i], &oldproc, NULL );

			if( oldproc == NULL && os_thread_is_busy( tp->thrds[i] ) == 0 )
			{
				/* 销毁线程，空出名额 */
				os_thread_destroy( tp->thrds[i], timeout );
                printf("os_thread_pool_alloc os_thread_destroy%d\n",tp->killnum);
				tp->thrds[i] = NULL;
				tp->killnum++;

				/* 创建新线程 */
				sprintf( name, "thread%d", ++cnt );
				thread = os_thread_create( name, THREAD_DEFAULT_PRIORIOTY, THREAD_DEFAULT_STACK_SIZE, proc, param );
				tp->thrds[i] = thread;
				break;
			}
		}
	}

	os_mutex_unlock( tp->mutex );

	return thread;
}

/* 释放一个线程到线程池中，不会强制关闭线程 */
void os_thread_pool_try_free( void* pool, void* thread, int32_t timeout )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	int32_t i;
	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
		return;

	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] == thread )
		{
			os_thread_pause( thread );
			os_thread_set_proc( thread, NULL, 0 );
			break;
		}
	}

	os_mutex_unlock( tp->mutex );
    //pthread_cleanup_pop(0);
}

/* 释放一个线程 */
void os_thread_pool_free( void* pool, void* thread, int32_t timeout )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	int32_t i;
	uint32_t start, now;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
	{
        LOG("thread_pool_free mutex lock failed\n");
        return;
    }
		

	for( i=0; i<tp->maxnum; i++ )
	{

		if( tp->thrds[i] == thread )
		{
			os_thread_pause( thread );
			os_thread_set_proc( thread, NULL, 0 );

			/* 等待用户函数退出 */
			start = now = os_sys_get_time();
			while( os_thread_is_busy( thread ) )
			{

				os_mutex_unlock( tp->mutex );
				os_sys_usleep( 1000 );
				os_mutex_lock( tp->mutex, timeout );

				now = os_sys_get_time();
				if( start + timeout < now || now < start )
					break;
			}

			/* 如果用户函数无法正常退出，则销毁线程 */
			if( os_thread_is_busy( thread ) )
			{

				os_mutex_unlock( tp->mutex );

				os_thread_destroy( thread, timeout );
                LOG("os_thread_pool_free thread exit non_normal\n");

				os_mutex_lock( tp->mutex, timeout );

				tp->thrds[i] = NULL;
				tp->killnum++;
			}
			break;
		}
	}

	os_mutex_unlock( tp->mutex );
}


/* 打印线程池信息 */
void os_thread_pool_print( void* pool )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	int32_t i = 0, allocnum = 0, idlenum = 0, sicknum = 0;
	PThreadProc oldproc = NULL;

	os_mutex_lock( tp->mutex, -1 );

	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] )
		{
			/* 根据线程执行函数是否有效来判断线程是否已经被分配出去 */
			oldproc = NULL;
			os_thread_get_proc( tp->thrds[i], &oldproc, NULL );

			if( oldproc )
				allocnum++;
			else if( os_thread_is_busy( tp->thrds[i] ) )
				sicknum++;
			else
				idlenum++;
		}
	}
	os_sys_print( NULL, 0, "threadpool(%d): allocated=%d, idle=%d, sick=%d, killed=%d\n",
			tp->maxnum, allocnum, idlenum, sicknum, tp->killnum );

	os_mutex_unlock( tp->mutex );
}
