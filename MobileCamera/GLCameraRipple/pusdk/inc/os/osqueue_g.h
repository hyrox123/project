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



/* ���ݽṹ */
typedef struct
{
//	uint32_t type;
	char *buf;
	//data_pkt_info_t* buf;
	int32_t bufsize;
	int32_t datasize;
}os_queue_data_t;

/* ���ݶ��� */
typedef struct
{
	/* ���ݶ��� */
	os_queue_data_t *datas;
	/* ���ݶ��г��� */
	int32_t num;
	/* ���ݶ����е�ͷλ�� */
	int32_t head;
	/* ���ݶ����е�βλ�� */
	int32_t tail;
	int32_t limit_num;
	int32_t high_limit_cnt;
	int32_t down_limit_cnt;
	int32_t limit_time;
	int32_t flag;

	/* �������ݶ��н��ͳ����ź��� */
	void* mutex;
	/* �Ƿ��������¼� */
	void* event;
    char eventname[32];
}os_queue_datapos_t;

/* ��ʼ�����ݶ��� */
void* os_queue_data_create( int32_t data_size, int32_t data_num );

/* �ͷŶ�����Դ */
void os_queue_data_destroy( void* queue );

/* �����ݶ����������,0=sucess,-1=failed */
int32_t os_queue_data_post( void* queue, void *buf, int32_t size );

/* �����ݶ�����ȡ����,0=sucess,-1=failed */
int32_t os_queue_data_read( void* queue, void *buf, int32_t size );

int32_t os_queue_data_read_audio( void* queue, void *buf, int32_t size );
/* �����ݶ�����ȡ����,0=sucess,-1=failed */
int32_t os_queue_data_read_audio_timeout( void* queue, void *buf, int32_t size, int32_t timeout );
int32_t os_queue_read_data_timeout( void* queue, void *buf, int32_t size, int32_t timeout );

/* ������ݶ��������е����� */
void os_queue_data_clear( void* queue ,int cmd);

/* ȡ�����ݶ���������Ŀ */
int32_t os_queue_data_get_num( void* queue );

void os_queue_destroy( void* queue );
int32_t os_queue_data_read_timeout( void* queue, void *buf, int32_t size, int32_t timeout );
#ifdef __cplusplus
}
#endif

#endif /* __OSQUEUE_H__ */
