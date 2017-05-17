/* ע�⣺���ļ��еĺ�����Ϊos�еĸ���ģ�����ģ���Ҫ��OSģ���еĺ��������ں˶��� */

#include "stdafx.h"
#include "ossys.h"
#include "osmem.h"
#include "signal.h"
#include <sys/sem.h>
#include "help.h"
#include "ms_pu_glob.h"
static void *m_memory = NULL;
static void *m_buf = NULL;
static pthread_mutex_t m_mutex;

static unsigned char m_ocg_exit;

static void os_sys_on_signal( int signal );


/* ��ʼ��OSģ�飬����0��ʾ�ɹ���-1��ʾʧ�� */
int32_t os_sys_init(int32_t size )
{
/*	int semid;
	key_t key;
	struct sembuf sops;
	const char *ocgsemkey = "huangxb2005@sina.com";

	/ * ��ʼ������������� * /
	srand( time(NULL) );

	memcpy(&key, ocgsemkey, sizeof(key_t));
	semid = semget((key_t)key, 1, 0666|IPC_CREAT|IPC_EXCL);
	if(semid == -1){
		printf("OCG is running!  semkey=%u\n", (unsigned int)key);
		return -1;
	}
	sops.sem_num = 0;
    sops.sem_op =  1;
	sops.sem_flg = IPC_NOWAIT|SEM_UNDO;
    if (semop (semid, &sops, 1) == -1){
        printf("semop failed\n");
        return -1;
	}
	semctl(semid, 1, IPC_RMID);

	pthread_mutex_init( &m_mutex, NULL );
	if(m_buf == NULL){
//		m_buf = malloc(size);
//		m_memory = os_mem_create( (char*)m_buf, size, 8 );
	}
*/
	m_ocg_exit = 0;
	/*��ʼ��Kill�źŴ�����*/
//	signal(SIGTERM, os_sys_kill);
//	signal(SIGPIPE, os_sys_on_signal );
//	signal(SIGHUP,SIG_IGN);
	signal( SIGPIPE, SIG_IGN ); 
	return 0;
}

/* �ͷ�OSģ�� */
void os_sys_exit()
{
	pthread_mutex_destroy( &m_mutex );
	if( m_memory )
	{
		os_mem_destroy( m_memory );
		m_memory = NULL;
	}
	if(m_buf != NULL)
		free(m_buf);
}

/* �����ڴ� */
#if 0
void* os_sys_malloc( int32_t size, const char *file, int32_t line )
{
//	void *p;

//	pthread_mutex_lock( &m_mutex );

//	p = os_mem_alloc( m_memory, size, file, line, NULL );

//	pthread_mutex_unlock( &m_mutex );
    void *p = (void *)malloc(size);
    //LOG("[os_sys_malloc] malloc size %d %s %d\n",size,file,line);
    printf("[os_sys_malloc] malloc size %d %s %d\n",size,file,line);
    if(NULL == p)
    {
        LOG("malloc failed %s",strerror(errno));
        LOG("%s %d\n",file,line);
        net_error_num = -3;//malloc failed;
    }

	return p;
}
#endif
void* os_sys_malloc( int32_t size, const char *file, int32_t line )
{
//	void *p;

//	pthread_mutex_lock( &m_mutex );

//	p = os_mem_alloc( m_memory, size, file, line, NULL );

//	pthread_mutex_unlock( &m_mutex );
    printf("[os_sys_malloc] malloc size %d %s %d\n",size,file,line);

	return (void *)malloc(size);
}


/* �ͷ��ڴ� */
void os_sys_free( void *buf, const char *file, int32_t line )
{
//	pthread_mutex_lock( &m_mutex );

//	os_mem_free( m_memory, buf, file, line );

//	pthread_mutex_unlock( &m_mutex );
    printf("++++++++++free %s %d\n",file,line);
	free(buf);
    buf = NULL;
}


/* ��ӡ��Ϣ */
void os_sys_print( const char* file, int32_t line, const char* fmt, ... )
{
	static char buf[1024*4];
	va_list ap;
	int n;

	va_start(ap, fmt);
	if( file )
		n = sprintf( buf, "%s(%d): ", file, line );
	else
		n = 0;
	vsprintf( buf+n, fmt, ap );
	va_end(ap);

	printf( buf );
}

/* ��ʱ(us) */
void os_sys_usleep( int32_t us )
{
	usleep( us );
/*
	struct timeval tv;

	if( 0 <= timeout )
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		select( 0, 0, 0, 0, &tv );
	}
	else
		select( 0, 0, 0, 0, NULL );
*/
}

/* ȡ���Կ��������ڵ�ʱ��(ms) */
uint32_t os_sys_get_time()
{
	static int first = 1;
	static struct timeval start;
	struct timeval now;

	gettimeofday( &now, NULL );

	if( first )
	{
		start = now;
		first = 0;
		return 0;
	}
	else
//	    return (now.tv_sec-start.tv_sec)*1000 + (now.tv_usec-start.tv_usec)/1000;// now.tv_usec-start.tv_usec����Ϊһ������ comment by fengyj
	    //return now.tv_sec*1000+now.tv_usec/1000 - start.tv_sec*1000-start.tv_usec/1000;// now.tv_usec-start.tv_usec����Ϊһ������ comment by fengyj
	    return now.tv_sec*1000+now.tv_usec/1000 - start.tv_sec*1000-start.tv_usec/1000;
}

/* ȡ�ÿ�������ǰʱ��(us) */
uint64_t os_sys_get_us()
{
	static int first = 1;
	static struct timeval start;
	struct timeval now;
	uint64_t tt;

	gettimeofday( &now, NULL );
	
	if( first )
	{
		start = now;
		first = 0;
		return 0;
	}
	else
	{
		tt = now.tv_sec - start.tv_sec;
		tt *= 1000000;
		tt += now.tv_usec - start.tv_usec;
		return tt;
	}
}


/* DNS����������IP��ַ�������ֽ��� */
uint32_t os_sys_dns( const char *hostname )
{
	struct hostent *he;
	uint32_t ip = 0;

	he = gethostbyname( hostname );
	if( he )
		memcpy( &ip, he->h_addr_list[0], 4 );

	return ip;
}

/* ȡ�ñ������� */
int32_t os_sys_get_local_name( char *name, int size )
{
	memset( name, 0, size );
	return gethostname( name, size-1 );
}


/* ���� */
void os_sys_assert( int32_t flag, const char *file, int32_t line )
{
	if( flag == 0 )
	{
		printf( "%s(%d): assert failed\n", file, line );
	}
}

/* ����ʱ��(����=8) */
int32_t os_sys_get_time_zone()
{
	static int32_t zone = -1;
	struct tm *pt;
	time_t now, tmp;

	if( zone == -1 )
	{
		/* ����UTCʱ�� */
		now = time( NULL );
		/* ����UTCʱ�� */
		pt = gmtime( &now );
		if( pt )
		{
			/* ���챾ʱ����UTCʱ�� */
			tmp = mktime( pt );
			zone = (now-tmp)/3600;
		}
		else
			zone = 8;
	}
	return zone;
}


void os_sys_kill(int sig)
{
	m_ocg_exit = 1;
}

unsigned char os_sys_is_exit()
{
	return m_ocg_exit;
}

static void os_sys_on_signal( int signal )
{
	/* linux command: "man 7 siganl" */
	switch( signal )
	{
	case SIGHUP: /* 1 Term    Hangup detected on controlling terminal or death of controlling process */
		break;
	case SIGINT: /* 2 Term    Interrupt from keyboard */
		break;
	case SIGQUIT: /* 3 Core    Quit from keyboard */
		break;
	case SIGILL: /* 4 Core    Illegal Instruction */
		break;
	case SIGABRT: /* 6 Core    Abort signal from abort(3) */
		break;
	case SIGFPE: /* 8 Core    Floating point exception */
		break;
	case SIGKILL: /* 9 Term    Kill signal */
		break;
	case SIGSEGV: /* 11 Core    Invalid memory reference */
		break;
	case SIGPIPE: /* 13 Term    Broken pipe: write to pipe with no readers */
		break;
	case SIGALRM: /* 14 Term    Timer signal from alarm(2) */
		break;
	case SIGTERM: /* 15 Term    Termination signal */
		break;
	case SIGUSR1: /* Term    User-defined signal 1 */
		break;
	case SIGUSR2: /* Term    User-defined signal 2 */
		break;
	case SIGCHLD: /* Ign     Child stopped or terminated */
		break;
	case SIGCONT: /* Continue if stopped */
		break;
	case SIGSTOP: /* Stop    Stop process */
		break;
	case SIGTSTP: /*  Stop    Stop typed at tty */
		break;
	case SIGTTIN: /* Stop    tty input for background process */
		break;
	case SIGTTOU: /* Stop    tty output for background process */
		break;
	default:
		break;
	}
	printf( "sytem signal %d captured!\n", signal );
	
}

