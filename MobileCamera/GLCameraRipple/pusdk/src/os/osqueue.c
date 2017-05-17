#include "stdafx.h"
#include "ossys.h"
#include "osmem.h"
#include "osmutex.h"
#include "osevent.h"
#include "osqueue.h"

/* ��Ϣ�ṹ */
typedef struct
{
	uint32_t type;
	char *buf;
	int32_t bufsize;
	int32_t datsize;
}os_queue_msg_t;

/* ��Ϣ���� */
typedef struct
{
	/* ��Ϣ���� */
	os_queue_msg_t *msgs;
	/* ��Ϣ���г��� */
	int32_t num;
	/* ��Ϣ�����е�ͷλ�� */
	int32_t head;
	/* ��Ϣ�����е�βλ�� */
	int32_t tail;
	/* ������Ϣ���н��ͳ����ź��� */
	void* mutex;
	/* �Ƿ�����Ϣ�¼� */
	void* event;
    char eventname[32];
}os_queue_t;


/* ��ʼ��ϵͳ��Ϣ���� */
void* os_queue_create( int32_t msg_size, int32_t msg_num )//1024*2 20
{
	int32_t size, i;
	os_queue_t *q = NULL;/**/


	size = sizeof(os_queue_t) + ( sizeof(os_queue_msg_t) + msg_size ) * msg_num;
	q = (os_queue_t *)os_sys_malloc( size, __FILE__, __LINE__ );
	if( q )
	{
		memset( q, 0, sizeof(os_queue_t) );
		q->msgs = (os_queue_msg_t *)(((char*)q) + sizeof(os_queue_t));
		q->num = msg_num;
		for( i=0; i<msg_num; i++ )
		{
			q->msgs[i].buf = ((char*)(q->msgs + msg_num)) + i * msg_size;
			q->msgs[i].bufsize = msg_size;
			q->msgs[i].datsize = 0;
		}

		q->mutex = os_mutex_create();//[10]24 [13]24  [26mdu]24
		if( q->mutex == NULL )
		  goto OS_QUEUE_FAILED;

        long id = (long)q->eventname;
        memset(q->eventname, 0, sizeof(q->eventname));
        sprintf(q->eventname, "%ld", id);
		q->event = os_event_create(q->eventname);//�����ź���
		if( q->event == NULL )
		  goto OS_QUEUE_FAILED;
	}
	return q;

OS_QUEUE_FAILED:

	os_queue_destroy( q );

	return NULL;
}

/* �ͷŶ�����Դ */
void os_queue_destroy( void* queue )
{
	os_queue_t *q = (os_queue_t *)queue;

	if( q->mutex )
	{
        os_mutex_destroy( q->mutex );
        
    }
	  
	if( q->event )
	  os_event_destroy( q->eventname );

	os_sys_free( q, __FILE__, __LINE__ );
    q = NULL;
}

/* ��ϵͳ��Ϣ���������Ϣ,0=sucess,-1=failed */
int32_t os_queue_post( void* queue, size_t type, void *buf, int32_t size )//û�����������Ĵ���
{

	os_queue_t *q = (os_queue_t *)queue;
	os_queue_msg_t *msg;
	int32_t next, rtn = -1;

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
		return -1;
	}

	next = q->tail + 1;
	if( q->num <= next )
	  next = 0;
	msg = &q->msgs[q->tail];

	if( next != q->head && size <= msg->bufsize )
	{
		msg->type = type;//type��ָ�����
		msg->datsize = size;
        
		memcpy( msg->buf, buf, size );
		q->tail = next;
		os_event_give( q->event );//�����ź���
		rtn = 0;
	}

	os_mutex_unlock( q->mutex );
	return rtn;
}

/* ��ϵͳ��Ϣ������ȡ��Ϣ,0=sucess,-1=failed */
int32_t os_queue_read( void* queue, size_t *type, void *buf, int32_t size )
{
	return os_queue_read_timeout( queue, type, buf, size, -1 );
}


/* ��ϵͳ��Ϣ������ȡ��Ϣ,0=sucess,-1=failed */
int32_t os_queue_read_timeout( void* queue, size_t *type, void *buf, int32_t size, int32_t timeout )
{
	os_queue_t *q = (os_queue_t *)queue;
	os_queue_msg_t *msg;
	int32_t rtn;
	unsigned long start, now;

	/* �����Ϣ����Ϊ�գ��ȴ���Ϣ */
	now = start = os_sys_get_time();
	while( q->head == q->tail )
	{
		if( 0 <= timeout && ( start + timeout <= now || now < start ) ){
			return -1;
		}
		os_sys_usleep( 1000);
		now = os_sys_get_time();
	}

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
		printf("\n=====os_queue_read_timeout_2=======\n");
		return -1;
	}

	if( q->head != q->tail )
	{
		msg = &q->msgs[q->head];

		*type = msg->type;
		if( buf && 0 < msg->datsize && 0 < size )
		{
			if( msg->datsize < size )
			  size = msg->datsize;
			memcpy( buf, msg->buf, size<msg->datsize ? size:msg->datsize ); 
		}
		q->head++;
		if( q->num <= q->head )
		  q->head = 0;
		rtn = 0;
	}
	else
	  rtn = -1; 


	/* ������в�Ϊ�գ����������źŵ� */
	if( q->head != q->tail )
	  os_event_give( q->event );

	os_mutex_unlock( q->mutex );

	return rtn;
}


/* �����Ϣ����*/
void os_queue_clear( void* queue )
{
	os_queue_t *q = (os_queue_t *)queue;
	q->tail = q->head = 0;
}

/* ȡ��ϵͳ��Ϣ������Ϣ��Ŀ */
int32_t os_queue_get_num( void* queue )
{
	os_queue_t *q = (os_queue_t *)queue;
	return ( q->tail - q->head + q->num ) % q->num;
}


