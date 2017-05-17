#include "stdafx.h"
#include "ossys.h"
#include "osmutex.h"
#include "osthreadpool.h"
#include "help.h"
#include <pthread.h>

typedef struct
{
	/* �����߳� */
	void** thrds;
	/* ����̸߳��� */
	int32_t maxnum;
	/* ��ɱ�����̸߳��� */
	int32_t killnum;
	/* �Ự������ */
	void* mutex;
}thead_pool_t;//�̳߳�


/* �����̳߳� */
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

/* �����̳߳� */
void os_thread_pool_destroy( void* pool, int32_t timeout )
{
	thead_pool_t *tp = (thead_pool_t *)pool;
	int32_t i;
	/* ���������߳� */
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] )
			os_thread_destroy( tp->thrds[i], timeout );
        //printf("end,");
	}
	/* ���ٱ����� */
	if( tp->mutex )
		os_mutex_destroy( tp->mutex );
	os_sys_free( pool, __FILE__, __LINE__ );
    pool = NULL;
}


/* ���̳߳��з���һ���߳� */
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

	/* Ѱ��һ��û�б������ȥ�����Ѿ��˳��û��������߳� */
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->thrds[i] == NULL )
			continue;

		/* �����߳�ִ�к����Ƿ���Ч���ж��߳��Ƿ��Ѿ��������ȥ */
		oldproc = NULL;
		os_thread_get_proc( tp->thrds[i], &oldproc, NULL );

		if( oldproc == NULL && os_thread_is_busy( tp->thrds[i] ) == 0 )
		{
			thread = tp->thrds[i];
			os_thread_set_proc( tp->thrds[i], proc, param );
			break;
		}
	}

	/* ���û���ҵ������߳�,����û�дﵽ���������,�򴴽�һ�� */
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

	/* �߳����������ٻ�û�д��û��������˳����̣߳������Ѿ��쳣��,�ճ������Դ������߳� */
	if( thread == NULL )
	{
		for( i=0; i<tp->maxnum; i++ )
		{
			if( tp->thrds[i] == NULL )
				continue;

			/* �����߳�ִ�к����Ƿ���Ч���ж��߳��Ƿ��Ѿ��������ȥ */
			oldproc = NULL;
			os_thread_get_proc( tp->thrds[i], &oldproc, NULL );

			if( oldproc == NULL && os_thread_is_busy( tp->thrds[i] ) == 0 )
			{
				/* �����̣߳��ճ����� */
				os_thread_destroy( tp->thrds[i], timeout );
                printf("os_thread_pool_alloc os_thread_destroy%d\n",tp->killnum);
				tp->thrds[i] = NULL;
				tp->killnum++;

				/* �������߳� */
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

/* �ͷ�һ���̵߳��̳߳��У�����ǿ�ƹر��߳� */
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

/* �ͷ�һ���߳� */
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

			/* �ȴ��û������˳� */
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

			/* ����û������޷������˳����������߳� */
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


/* ��ӡ�̳߳���Ϣ */
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
			/* �����߳�ִ�к����Ƿ���Ч���ж��߳��Ƿ��Ѿ��������ȥ */
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
