#include "stdafx.h"
#include "ossys.h"
#include "osmem.h"
#include "osmutex.h"
#include "osevent.h"
#include "osqueue_g.h"
#include "osqueue.h"


/* 初始化系统消息队列 */
void* os_queue_data_create( int32_t data_size, int32_t data_num )
{
	int32_t size, i;
	os_queue_datapos_t *q = NULL;

	size = sizeof(os_queue_datapos_t) + ( sizeof(os_queue_data_t) + data_size) * data_num;
	q = (os_queue_datapos_t *)os_sys_malloc( size, __FILE__, __LINE__ );
	if( q )
	{
		memset( q, 0, sizeof(os_queue_datapos_t) );
		q->datas = (os_queue_data_t *)(((char*)q) + sizeof(os_queue_datapos_t));
		q->num = data_num;
		for( i=0; i<data_num; i++ )
		{
			q->datas[i].buf = ((char*)(q->datas + data_num)) + i * data_size;
			q->datas[i].bufsize = data_size;
			q->datas[i].datasize = 0;
		}
		q->mutex = os_mutex_create();//[34]24
		if( q->mutex == NULL )
			goto OS_QUEUE_FAILED;

        long id = (long)q->eventname;
        memset(q->eventname, 0, sizeof(q->eventname));
        sprintf(q->eventname, "%ld", id);
		q->event = os_event_create(q->eventname);
		if( q->event == NULL )
			goto OS_QUEUE_FAILED;
	}
	return q;

OS_QUEUE_FAILED:

	os_queue_destroy( q );

	return NULL;
}

/* 释放队列资源 */
void os_queue_data_destroy( void* queue )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;

	if( q->mutex )
		os_mutex_destroy( q->mutex );
	if( q->event )
		os_event_destroy( q->eventname );

	os_sys_free( q, __FILE__, __LINE__ );
}

/* 向数据队列添加数据,0=sucess,-1=failed */
int32_t os_queue_data_post( void* queue, void *buf, int32_t size )
{

	//printf("\n use  os_queue_data_post stat !\n");
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data;
	int32_t next, rtn = -1;

	if( os_mutex_lock( q->mutex, -1 ) != 0 )
		return -1;
	//printf("rudui\n");
	next = q->tail + 1;
	if( q->num <= next )
		next = 0;
	data = &q->datas[q->tail];
	//printf("if( next != q->head && size <= data->bufsize )\n");
	if( next != q->head && size <= data->bufsize )
	//if(next != q->head)
	{
	
		//msg->type = type;
		//buf = data->buf;
		data->datasize = size;
        //data->buf = buf;
		
		memcpy( data->buf, (char *)buf, size);
		q->tail = next;
		q->limit_num++;
        //printf("q->limit_num %d\n",q->limit_num);
		os_event_give( q->event );
		rtn = 0;
	}

	os_mutex_unlock( q->mutex );
	return rtn;
}




/* 从数据队列提取数据,0=sucess,-1=failed */
int32_t os_queue_data_read( void* queue, void *buf, int32_t size )
{
	return os_queue_data_read_timeout( queue, buf, size, 10);
}

int32_t os_queue_data_read_audio( void* queue, void *buf, int32_t size )
{
	//return os_queue_data_read_timeout( queue, buf, size, -1 );
	return os_queue_data_read_audio_timeout( queue, buf, size,-1 );
}
int32_t os_queue_data_read_audio_timeout( void* queue, void *buf, int32_t size, int32_t timeout )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data;
	int32_t rtn;
	//uint32_t start, now;
	
	if( q->head == q->tail )
	{
	 //   printf("os_queue_data_read_timeout loop\n");
	//	if( 0 <= timeout && ( start + timeout <= now || now < start ) )
			return -1;
	//	printf("\n  os_queue_data_read_timeout2\n");
	//	os_sys_usleep( timeout );
	//	now = os_sys_get_time();
	}

	if(q->mutex == NULL || q->mutex == 0)
		return -1;

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
			printf("\n  os_queue_data_read_timeout3\n");
		return -1;
	}

	if( q->head != q->tail )																		
	{
		//printf("\n  os_queue_data_read_timeout4\n");
		data = &q->datas[q->head];
		
		//printf("\n    data->datasize value is %d \n", data->datasize);
		if((data->datasize > 0) && ( size >0) )
		{
			if( data->datasize < size )
				size = data->datasize;
			//printf("\n  data->datasize is %d\n",size);
			memcpy( buf, data->buf, size<data->datasize ? size:data->datasize ); 
		}
		q->head++;
		if( q->num <= q->head )
			q->head = 0;
		rtn = data->datasize;

		//printf("\n   q->num is %d\n", q->num);
		//printf("\n  q->head is %d\n",q->head);
	}
	else{
		printf("\n  os_queue_data_read_timeout4\n");
		rtn = -1;
	}

	/* 如果队列不为空，继续点亮信号灯 */
	if( q->head != q->tail )
		os_event_give( q->event );

	os_mutex_unlock( q->mutex );

	return rtn;
}

/* 从数据队列提取数据,0=sucess,-1=failed */
int32_t os_queue_data_read_timeout( void* queue, void *buf, int32_t size, int32_t timeout )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data;
	int32_t rtn;
	uint32_t start, now;

   

	now = start = os_sys_get_time();

	while( q->head == q->tail)
	{
		if(q->head == q->tail)
		{
			//printf("q->head == q->tail\n");
		}
		
		if( 0 <= timeout && ( start + timeout <= now ))
				 return -1;

		os_sys_usleep( 1000 );
		
		now = os_sys_get_time();
	}

	
	//end=os_sys_get_time();
	//os_event_wait( q->event, timeout );

	if(q->mutex == NULL || q->mutex == 0)
		return -1;

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
		printf("\n  os_queue_data_read_timeout3\n");
		return -1;
	}

	if( q->head != q->tail )																		
	{
		data = &q->datas[q->head];
		
		if((data->datasize > 0) && ( size >0) )
		{
			if( data->datasize < size )
				size = data->datasize;
			
			memcpy( buf, data->buf, size<data->datasize ? size:data->datasize );
            //data->buf = NULL;
          
            
            
		}
        else
        {
			os_mutex_unlock( q->mutex );
			os_sys_usleep( 1000 );
			return -1;
		}
		q->head++;
		q->limit_num--;
		if( q->num <= q->head )
			q->head = 0;
		rtn = data->datasize;
        //data->datasize = 0;
		//printf("\n   q->num is %d\n", q->num);
		//printf("\n  q->head is %d\n",q->head);
	}
	else
    {
		printf("\n  os_queue_data_read_timeout4\n");
		os_mutex_unlock( q->mutex );
		rtn = -1;
	}

	/* 如果队列不为空，继续点亮信号灯 */
	if( q->head != q->tail )
		os_event_give( q->event );

	os_mutex_unlock( q->mutex );

	return rtn;
}



/* 向数据队列添加数据,0=sucess,-1=failed */
int32_t os_queue_audio_data_post( void* queue, void *buf, int32_t size )
{


	//printf("\n use  os_queue_data_post stat !\n");
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data;
	int32_t next, rtn = -1;

	if( os_mutex_lock( q->mutex, -1 ) != 0 )
		return -1;

	next = q->tail + 1;
	if( q->num <= next )
		next = 0;
	data = &q->datas[q->tail];

	if( next != q->head && size <= data->bufsize )
	{
	
		//msg->type = type;
		data->datasize = size;
		
		memcpy( data->buf, buf, size );
		q->tail = next;
	
		if( (q->limit_num >= 8))
			q->flag = 1;
		else if(q->limit_num <= 1){

				q->limit_time = 43;
				q->flag = 0;
		}

		q->limit_num++;
		os_event_give( q->event );
		rtn = 0;
	}

	os_mutex_unlock( q->mutex );
	return rtn;

}



/* 从数据队列提取音频数据,0=sucess,-1=failed */
int32_t os_queue_audio_data_read_timeout( void* queue, void *buf, int32_t size, int32_t timeout )
{


	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data;
	int rtn;
	uint64_t now;
	uint64_t  start;
	
	start = now = os_sys_get_time();


	while( q->head == q->tail ||  q->flag != 0)
	{

		if(q->limit_num > 50)
			break;
		
		if(q->flag ==1){

			if(  start + q->limit_time <= now )
				break;
		    
		}else{

			 if(  start + timeout <= now )
			 	return -1;
		}

		os_sys_usleep( 2000 );
		
		now = os_sys_get_time();
	}


	if(q->mutex == NULL || q->mutex == 0)
	{
		return -1;
	}

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
		return -1;
	}

	if( q->head != q->tail  && q->flag != 0){		
		
		data = &q->datas[q->head];
			
		if((data->datasize > 0) && ( size >0) )
		{
			if( data->datasize < size )
				size = data->datasize;
			//printf("\n  data->datasize is %d\n",size);
			memcpy( buf, data->buf, size<data->datasize ? size:data->datasize ); 
		}
		else
		{
			os_mutex_unlock( q->mutex );
			os_sys_usleep( 1000 );
			return -1;
		}
		
		q->head++;
		q->limit_num--;
		
		if( q->num <= q->head )
			q->head = 0;
		rtn = data->datasize;

		}
	
	else{

		os_mutex_unlock( q->mutex );
		return  -1;

	}
	

	/* 如果队列不为空，继续点亮信号灯 */
	if( q->head != q->tail )
		os_event_give( q->event );

	os_mutex_unlock( q->mutex );

	return rtn;


}




/* 清除数据队列*/
void os_queue_data_clear( void* queue,int cmd )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	os_queue_data_t *data = NULL;
	void * buf = NULL;
	data_pkt_info_t *data_pkt_pt = NULL;
    
	int i = 0;
	int32_t mid;
	uint32_t start, now;

	//memset(data_pkt_pt,0,sezeof())
	now = start = os_sys_get_time();
	mid = q->head;
	
	for(; now <= start+100; i++)
	{		
		
		now = os_sys_get_time();
	}

	if( os_mutex_lock( q->mutex, -1 ) != 0 ){
			//printf("\n  os_queue_data_read_timeout3\n");
		
	}

	//printf("\n======FREE DATE STATE!  head value %d  tai value %d  num value %d ======\n",q->head,q->tail,q->limit_num);

	for(;q->head != q->tail;q->head++){


		if( q->num <= q->head )
			q->head = 0;

		data = &q->datas[q->head];

		if(data->datasize > 0){

			buf = (void *)data->buf;
			//printf("_+_+_+_+_+_+_+_+__+_+_+_+_+_+_+_+_+_+\n");
			data_pkt_pt = (data_pkt_info_t * )buf;

			if(data_pkt_pt->pkt_pt != NULL  && data_pkt_pt->pkt_size != 0){

				free(data_pkt_pt->pkt_pt);
				data_pkt_pt->pkt_pt=NULL;
				data_pkt_pt->pkt_size = 0;
				q->limit_num--;
 			}
 			
		}
        //data->buf = NULL;
        data->datasize = 0;
		data = NULL;
		buf = NULL;
		data_pkt_pt = NULL;     
		
	}
	//os_mutex_unlock( q->mutex );

		//printf("\n======FREE DATE END!  head value %d  tai value %d  num value %d ======\n",q->head,q->tail,q->limit_num);

	if(q->head == q->tail){
		q->tail = q->head = 0;
		
		
	}

	os_mutex_unlock( q->mutex );

	
}



/* 清除数据队列*/
/*
void os_queue_data_clear( void* queue )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	q->tail = q->head = 0;
}

*/

/* 取得数据队列数据数目 */
int32_t os_queue_data_get_num( void* queue )
{
	os_queue_datapos_t *q = (os_queue_datapos_t *)queue;
	return ( q->tail - q->head + q->num ) % q->num;
}
