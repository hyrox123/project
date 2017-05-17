#ifndef __MS_PU_TYPE_H__
#define __MS_PU_TYPE_H__

#include "stdafx.h"
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/types.h>


#define THREADPOOL_MAX_NUM	100
#define THREADPOOL_TIMEOUT	2000
#define  MESQUEUE_NUM			2

#define MQ_MSG_MAX_SIZE		1024*2//消息的长度2K
#define MQ_MSG_MAX_NUM		20//消息的条数

#define AUDIO_COMBINPACKET_SIZE 344*4
#define AUDIO_PACKET_SIZE 344
#define AUDIOI_PAKCET_BODY_SIZE  320



#define SOCKET_TIMEOUT		1
#define MQ_TIMEOUT			5

#define KPL_TIMEOUT_CNT     3  //心跳超时重连检测次数

#define KPL_MDU_PERIOD      30  //心跳超时重连检测次数

#define MAX_BUFFER_LEN		1024*4

/********************************************/
typedef struct _handle_grp
{
	void* hdl_mq[MESQUEUE_NUM];
	void* hdl_mutex;
	void* hdl_thrd_sock;
	void* hdl_thrd_reg;
	void* hdl_thrd_msg;	
	void* hdl_thrd_alarm;	
	void* hdl_thrd_operate_msg;
}handle_grp;

typedef struct
{
	unsigned short ver;
	unsigned short length;
	unsigned int   cseq;//指令序号
}sock_header, *lp_sock_header;

typedef enum _status_reg
{
	REGISTER_OFF = 0,
	REGISTER_ON,
	REGISTER_ING, //正在注册状态
	UNREGISTER_ING, //正在注销状态
	UNREGISTER_END
}status_reg;

#define REGISTER_TIME 10 //2//30秒注册一次

#define SUCCESS   1
#define FAILED    -1
//typedef enum _request_type
//{
//	REQUEST_REAL_STREAM = 0,
//	REQUEST_FILE_STREAM
//}request_type;
//
//typedef enum _link_mode
//{
//	LINK_MODE_TCP=0,
//	LINK_MODE_UDP,
//	LINK_MODE_RTP,
//	LINK_MODE_MULTICAST,
//	LINK_MODE_UNKNOW
//}link_mode;


typedef enum
{
	STREAM_STATE_READY   = 0,
	STREAM_STATE_RUNNING,
	STREAM_STATE_EXIT,
	STREAM_STATE_UNKNOW,
	STREAM_STATE_PUSH_STOP,
	STREAM_STATE_POP_STOP
}stream_state;

typedef struct
{
	int				is_reg;
	int				kpl_period;
	int             kpl_num;
	int             kpl_timing_count;
	time_t			last_kpl;
	
	char			sip_ip[16];
	unsigned int  sip_port;
	
	char			pu_id[32];
	time_t		    last_time;
	int				cmd;
	int				loop_num;
}unit_info;

typedef struct _local_socket
{
	char			ser_ip[16];
	unsigned int	ser_port;
	
	char			cli_ip[16];
	unsigned int	cli_port;
	
	int				cli_skt;
}sock_info;

typedef struct								// 设备数字量上报
{
	int   motion_flags[16];
	int   hide_flags[16];
	int 	vide_lose_flags[16];
	int   inout_flags[16]; 
	int   disk_error;
	int 	disk_full;
	int		alarm_upload_num;


}ALARM_UPLOAD_TYPEFLAGS;

typedef struct 
{

	void *  mutex;
	int      flag;
	char	 sendbuf[132*20];
	int 	sendsize;
	int	     alarm_interval_time;
	int      max_interval_time;
	time_t     alarm_start_time;
	time_t	     alarm_end_time;
	ALARM_UPLOAD_TYPEFLAGS    type_flags;

}alram_data;



#endif
