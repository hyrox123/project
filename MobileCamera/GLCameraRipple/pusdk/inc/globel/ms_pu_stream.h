#ifndef MS_PU_STREAM_H_H
#define MS_PU_STREAM_H_H


#include <ms_pu_param.h>
#include <osqueue_g.h>


typedef struct 
{
	//线程句柄
	void*   thrd;
	void*   mutex;
	int32_t skt;

	//是否暂停
	int32_t pause;
	//是否退出
	int32_t quit;
	/* 是否在用户执行函数中没有返回 */
	int32_t busy;

	int32_t state;  //状态
	
    int32_t media_type;
    
    LPNET_MDU_START_MEDIA lp_media;
	os_queue_datapos_t *data_pkt_queue;
    os_queue_datapos_t *audio_data_pkt_queue;
        
}stream_info_t;


int32_t  mdu_stream_post_data( void* stream, char* buffer, int32_t size ,int frame_type);
int push(void* stream, char* buffer, int32_t size,int frame_type);
int32_t  mdu_stream_receive_data( void* stream, char* buffer,  size_t * size );
int32_t mdu_stream_is_running( void* stream );
struct RtpHeader
{
#if(BYTE_ORDER == LITTLE_ENDIAN)
	unsigned short cc      :4;
	unsigned short x       :1;
	unsigned short p       :1;
	unsigned short version :2;
	unsigned short pt      :7;
	unsigned short marker  :1;
#elif(BYTE_ORDER == BIG_ENDIAN)
	unsigned short version :2;
	unsigned short p       :1;
	unsigned short x       :1;
	unsigned short cc      :4;
	unsigned short marker  :1;
	unsigned short pt      :7;
#else
#error YOU MUST DEFINE BYTE_ORDER == LITTLE_ENDIAN OR BIG_ENDIAN !
#endif
    unsigned short seqno  :16;
    long lTimeSec;
    unsigned long ssrc;
    unsigned short usProfile;
    unsigned short usExtLen;
    unsigned long uiDecoderID;
    unsigned char ucFrameNum;
    unsigned char ucFrameSeq;
    unsigned short usPtLen;
};
#endif



