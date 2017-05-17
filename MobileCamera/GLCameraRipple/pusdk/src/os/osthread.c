	#include "stdafx.h"
#include "ossys.h"
#include "osthread.h"
#include "help.h"

typedef struct
{
	/* 线程名称 */
	char name[32];
	/* 线程ID */
	pthread_t tid;
	/* 线程属性对象 */
	pthread_attr_t attr;
	/* 线程的优先级 */
	int32_t priority;
	/* 控制信号灯 */
	sem_t *ctlsem;
	/* 退出信号灯 */
	sem_t *endsem;

	/* 线程回调函数 */
	PThreadProc proc;
	/* 回调函数参数 */
	uint32_t param;
	
	/* 是否暂停 */
	int32_t pause;
	/* 表示当前线程是否退出 */
	int32_t quit;
	/* 是否在用户执行函数中没有返回 */
	int32_t busy;
    char ctlsemname[32];
    char endsemname[32];

}thread_info_t;

static void* os_thread_proc( void* lpParam );

/* 创建线程，成功后处于暂停状态，priority=0(最高), priority=255(最低) */
void* os_thread_create( char *name, unsigned char priority, int32_t stack_size, PThreadProc proc, uint32_t param )
{
	thread_info_t *info = NULL;
//	int32_t policy = 0;
//	struct sched_param sparam;
	int size;

	size = sizeof(thread_info_t) + sizeof(sem_t) * 2;
	info = (thread_info_t*)os_sys_malloc( size, __FILE__, __LINE__ );//[15]136  [16]136[17]136[18]136[19]136 mdu[28]136[29]136[30]136
	if( info == NULL )
	{
	    printf( " malloc thread %d space failed!\n ", size );
		return NULL;
	}	

	memset( info, 0, size );
	info->proc = proc;
	info->param = param;
	info->priority = priority;
	info->pause = 1;
	strncpy( info->name, name, sizeof( info->name )-1 );
	info->ctlsem = (sem_t *)(info + 1);
	info->endsem = info->ctlsem + 1;

	/* 设置线程的优先级 */
	pthread_attr_init( &info->attr );

	/* Modify for work successful */


    long id = (long)info->ctlsemname;
    memset(info->ctlsemname, 0, sizeof(info->ctlsemname));
    sprintf(info->ctlsemname, "%ld", id);
    
    info->ctlsem = sem_open(info->ctlsemname, O_CREAT, 0664, 0);
	/* 创建信号灯 */
//	if( sem_init( info->ctlsem, 0, 0 ) == -1 )
    if(info->ctlsem <= 0)
	{
//        sem_unlink(info->ctlsemname);

		pthread_attr_destroy( &info->attr );
        LOG("[os_thread_create]thread info->ctlsem failed\n");
		goto THREAD_FAILED;
	}
    id = (long)info->endsemname;
    memset(info->endsemname, 0, sizeof(info->endsemname));
    sprintf(info->endsemname, "%ld", id);
    info->endsem = sem_open(info->endsemname, O_CREAT, 0664, 0);
//	if( sem_init( info->endsem, 0, 0 ) == -1 )
    if(info->endsem < 0)
	{
//		sem_destroy( info->ctlsem );
        sem_unlink(info->ctlsemname);
		pthread_attr_destroy( &info->attr );
        LOG("[os_thread_create]thread info->endsem failed\n");
		goto THREAD_FAILED;
	}
     pthread_attr_t a;
     //这里设置为“detached  state”
     pthread_attr_init(&a);
     pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
	/* 开始运行线程 */
	if( pthread_create( &info->tid, &a, os_thread_proc, (void*)info ) != 0 )
	{
        fprintf(stderr,"pthread_create %d %s \n",errno,strerror(errno));
//		sem_destroy( info->endsem );
//		sem_destroy( info->ctlsem );
        sem_unlink(info->ctlsemname);
        sem_unlink(info->endsemname);
		pthread_attr_destroy( &info->attr );
        LOG("[os_thread_create]pthread_create failed %d %s \n",errno,strerror(errno));
		goto THREAD_FAILED;
	}
     pthread_attr_destroy (&a);
     //printf(" +++++++++++++++++++++%s \n%u,\n",info->name,(unsigned int)info->tid);

	return info;

THREAD_FAILED:

	os_sys_free( info, __FILE__, __LINE__ );

	return NULL;
}

/* 销毁线程(只是试图而不是强制销毁线程)，返回0表示成功,-1表示失败 */
int32_t os_thread_try_destroy( void* thread, int32_t timeout )
{
	thread_info_t *info = (thread_info_t *)thread;
	struct timespec ts;
	int32_t ret;

	/* 通知线程结束 */
	info->quit = 1;
	if( info->ctlsem )
		sem_post( info->ctlsem );

	/* 等待线程结束*/
	if( info->tid != 0 && info->endsem )
	{
		if( timeout < 0 )
			sem_wait( info->endsem );
		else if( 0 < timeout )
		{
			ts.tv_sec = time(NULL) + timeout/1000;
			ts.tv_nsec = (timeout%1000)*1000;
		//	sem_timedwait( info->endsem, (const struct timespec *)&ts ); ljp
            sem_wait( info->endsem );
		}
	}

	ret = info->tid == 0 ? 0 : -1;

	if( info->endsem )
	{
//		sem_destroy( info->endsem );
        sem_unlink(info->endsemname);
		info->endsem = NULL;
	}
	if( info->ctlsem )
	{
//		sem_destroy( info->ctlsem );
        sem_unlink(info->ctlsemname);
		info->ctlsem = NULL;
	}

	os_sys_free( info, __FILE__, __LINE__ );

	return ret;
}

/* 销毁线程(如果试图失败则会强制销毁)，返回0表示成功,-1表示失败 */
int32_t os_thread_destroy( void* thread, int32_t timeout)
{
	thread_info_t *info = (thread_info_t *)thread;
	struct timespec ts;
	void *result;
	int32_t ret;
    static int  pa = 0;
    //static int num = 0;
	/* 通知线程结束 */
	info->quit = 1;
	if( info->ctlsem )
		sem_post( info->ctlsem );

	/* 等待线程结束 */
	if( info->tid != 0 && info->endsem )
	{

		if( timeout < 0 )
			sem_wait( info->endsem );
		else if( 0 < timeout )
		{
#if 1
			ts.tv_sec = time(NULL) + timeout/1000;
			ts.tv_nsec = (timeout%1000)*1000;
		//	sem_timedwait( info->endsem, (const struct timespec *)&ts );
            sem_wait( info->endsem );     
#else
       		sem_wait( info->endsem );     
#endif
		}
	}

	if( info->tid != 0 )
	{
		/* 强制退出 */
        printf("info->name %s \n",info->name);
        
		if((pa = pthread_kill( info->tid, -1 )) != 0)
		{
             LOG("[os_thread_destroy]KILL PTHREAD info->name %s \n \n",info->name);
             fprintf(stderr,"pthread_kill %s\n",strerror(errno));
        }
        printf("pthread_kill( info->tid, -1 );\n");
        LOG("[os_thread_destroy]KILL PTHREAD info->name %s \n \n",info->name);
		/* 等待结束 */

		pthread_join( info->tid, &result );
		//pthread_detach(pthread_self());

	}

	ret = info->tid == 0 ? 0 : -1;

	if( info->endsem )
	{
//		sem_destroy( info->endsem );
        sem_unlink(info->endsemname);
		info->endsem = NULL;
	}
	if( info->ctlsem )
	{
//		sem_destroy( info->ctlsem );
        sem_unlink(info->ctlsemname);
		info->ctlsem = NULL;
	}
    if(NULL != info)
    {
        os_sys_free( info, __FILE__, __LINE__ );
        info = NULL;
    }
	    

	return ret;
}


/* 修改线程名称 */
void os_thread_set_name( void* thread, char* name )
{
	thread_info_t *info = (thread_info_t*)thread;
	strncpy( info->name, name, sizeof( info->name )-1 );
}

/* 获取线程名称 */
void os_thread_get_name( void* thread, char* buf, int32_t size )
{
	thread_info_t *info = (thread_info_t*)thread;

	strncpy( buf, info->name, size-1 );
	buf[size-1] = 0;
}


/* 设置线程优先级  */
void os_thread_set_priority( void* thread, unsigned char priority )
{
	thread_info_t *info = (thread_info_t*)thread;
	struct sched_param sparam;

	if( priority > 0 )
	{
		priority = sched_get_priority_max(SCHED_RR) - (priority * sched_get_priority_max(SCHED_RR)) / 255;
		sparam.sched_priority = priority;
	}
	pthread_setschedparam(info->tid, SCHED_RR, &sparam );
}

/* 设置回调函数 */
void os_thread_set_proc( void* thread, PThreadProc proc, uint32_t param )
{
	thread_info_t *info = (thread_info_t*)thread;

	info->proc = proc;
	info->param = param;
}

/* 取得线程执行函数 */
void os_thread_get_proc( void* thread, PThreadProc *proc, uint32_t *param )//我觉得这个proc param有问题
{
	thread_info_t *info = (thread_info_t*)thread;

	if( proc )
		*proc = info->proc;
	if( param )
		*param = info->param;
}

/* 暂停 */
void os_thread_pause( void* thread )
{
	((thread_info_t *)thread)->pause = 1;
}

/* 继续 */
void os_thread_resume( void* thread )
{
	thread_info_t *info = (thread_info_t *)thread;

	info->pause = 0;
	if( info->ctlsem )
		sem_post( info->ctlsem );
    else
    {
        LOG("sem_post is NULL%s",info->name);
    }
}

/* 是否正在运行 */
int32_t os_thread_is_running( void* thread )
{
	thread_info_t *info = (thread_info_t*)thread;
	
	return info->pause == 0 && info->quit == 0;
}

/* 是否在用户执行函数中没有返回 */
int32_t os_thread_is_busy( void* thread )
{
	return ((thread_info_t *)thread)->busy;
}

/* 线程执行函数 */
static void* os_thread_proc( void* lpParam )
{
	thread_info_t *info = (thread_info_t *)lpParam;
	int32_t delay;
//	struct timespec ts;

	while(info->ctlsem )
	{
		
		//取得触发事件的序号
		if( info->pause || info->proc == NULL )
			sem_wait( info->ctlsem );

		if( info->quit )
			break;


		/* 执行用户函数，返回值<0表示退出，>0表示延时(ms) */
		if( info->proc )
		{
			info->busy = 1;
			delay = info->proc( info->param );//Segmentation fault.
			info->busy = 0;
		}
		else
			delay = 0;

		if( delay < 0 )
			break;
		else if( 0 < delay )
		{
			usleep(delay*1000);
		}
	}
	info->quit = 1;
	info->proc = NULL;
	info->tid = 0;

	/* 通知线程结束 */
	if( info->endsem )
		sem_post( info->endsem );

	return 0;
}
