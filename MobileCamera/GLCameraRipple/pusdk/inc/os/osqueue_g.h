#ifndef __OSQUEUEG_H__
#define __OSQUEUEG_H__
#include "stdafx.h"
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
  int pkt_size;
  unsigned char *pkt_pt;
  //char *pkt_pt;
  int frame_type;
}data_pkt_info_t;



/* 数据结构 */
typedef struct
{
//	uint32_t type;
	char *buf;
	//data_pkt_info_t* buf;
	int32_t bufsize;
	int32_t datasize;
}os_queue_data_t;

/* 数据队列 */
typedef struct
{
	/* 数据队列 */
	os_queue_data_t *datas;
	/* 数据队列长度 */
	int32_t num;
	/* 数据队列中的头位置 */
	int32_t head;
	/* 数据队列中的尾位置 */
	int32_t tail;
	int32_t limit_num;
	int32_t high_limit_cnt;
	int32_t down_limit_cnt;
	int32_t limit_time;
	int32_t flag;

	/* 控制数据队列进和出的信号量 */
	void* mutex;
	/* 是否有数据事件 */
	void* event;
    char eventname[32];
}os_queue_datapos_t;

/* 初始化数据队列 */
void* os_queue_data_create( int32_t data_size, int32_t data_num );

/* 释放队列资源 */
void os_queue_data_destroy( void* queue );

/* 向数据队列添加数据,0=sucess,-1=failed */
int32_t os_queue_data_post( void* queue, void *buf, int32_t size );

/* 从数据队列提取数据,0=sucess,-1=failed */
int32_t os_queue_data_read( void* queue, void *buf, int32_t size );

int32_t os_queue_data_read_audio( void* queue, void *buf, int32_t size );
/* 从数据队列提取数据,0=sucess,-1=failed */
int32_t os_queue_data_read_audio_timeout( void* queue, void *buf, int32_t size, int32_t timeout );
int32_t os_queue_read_data_timeout( void* queue, void *buf, int32_t size, int32_t timeout );

/* 清除数据队列中所有的数据 */
void os_queue_data_clear( void* queue ,int cmd);

/* 取得数据队列数据数目 */
int32_t os_queue_data_get_num( void* queue );

void os_queue_destroy( void* queue );
int32_t os_queue_data_read_timeout( void* queue, void *buf, int32_t size, int32_t timeout );
#ifdef __cplusplus
}
#endif

#endif /* __OSQUEUE_H__ */
