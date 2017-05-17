#include "ms_mdu_stream.h"
#include "ms_mdu_msg.h"
#include "ms_pu_glob.h"
#include <stdlib.h>
#include <stdio.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "help.h"
#include "ms_pu_param.h"
#include "ms_pu_lib.h"
#include <stdlib.h>
#include <time.h>
#include "ms_mdu_stream_pool.h"
#define  SEND_BUFFER_SIZE  4096
#define AUDIO_BUFFER_SIZE 320
#include "oslib.h"
#include "osqueue_g.h"
extern void* stream_pool;
/* 数据结构 */

/* 数据队列 */
#define MAX_DATA_PKT_QUEUE_SIZE 500

static int32_t mdu_stream_proc( uint32_t lpParam );
static int32_t mdu_audio_proc( uint32_t lpParam );

int32_t mdu_stream_socket_create(char* mdu_ip, unsigned int mdu_port);

int32_t mdu_send_media(int skt, LPNET_MDU_START_MEDIA lp_media);

//hjh comment cout @2011.8.26

#if 1
int pop(uint32_t lpParam)
{
    
	//static int count = 0;
	stream_info_t *info = (stream_info_t *)lpParam;
    char audio_buffer[AUDIO_BUFFER_SIZE];
	data_pkt_info_t data_pkt_pt;//pointer
	data_pkt_info_t audio_data_pkt_pt;//audio  pointer
    //printf("send data to CH:%d\n ",info->lp_media->media.video_id);
	static int seqnumber= 0 ;
	struct RtpHeader rtp;
	unsigned char sendbuffer[SEND_BUFFER_SIZE] = {0};
    int audio_status=0;
 
    //NET_MDU_START_MEDIA stop_media;
    
	int iLen = 0;
	int iCount = 0;
	int iSend = SEND_BUFFER_SIZE-sizeof(struct RtpHeader);
#if 0
    int iRet = os_queue_data_read_audio(info->audio_data_pkt_queue, &audio_data_pkt_pt,  sizeof(data_pkt_info_t));
    if (-1 != iRet)
    {
        audio_status=1;
        memset(audio_buffer,0,sizeof(audio_buffer));
        memcpy(audio_buffer,audio_data_pkt_pt.pkt_pt,AUDIO_BUFFER_SIZE);
        if(audio_data_pkt_pt.pkt_pt != NULL || audio_data_pkt_pt.pkt_size != 0)
        {
            free(audio_data_pkt_pt.pkt_pt);
            audio_data_pkt_pt.pkt_pt = NULL;
            audio_data_pkt_pt.pkt_size = 0;
        }
    }
    else
    {
        audio_status=0;
    }
#endif
    
	int iRet = os_queue_data_read(info->data_pkt_queue, &data_pkt_pt,  sizeof(data_pkt_info_t));
	if (-1 == iRet)
	{
        //LOG("stream read failed \n");
		return -1;
	}

        
    #if 1
    iRet = 1;
    //printf("_+_+_++++++++++++++++++++++++++++++++++++%d\n",data_pkt_pt.pkt_size);
    //sendbuffer=(char *)malloc(SEND_BUFFER_SIZE);//1200
	memset(&rtp,0,sizeof(struct RtpHeader));
	rtp.x = 1;//表示有扩展的头字段
	rtp.cc = 0;/* CSRC个数,置0,无CSRC */
	rtp.ssrc = htonl(1234567);/*数据源标识*/
	rtp.version = 2;/*协议版本*/
    //time_sec = time(NULL);
    rtp.lTimeSec = htonl((long)time(NULL));
	rtp.marker=data_pkt_pt.frame_type;
	rtp.pt=1;
	rtp.usExtLen = htons(2);
	rtp.usProfile = htons(1);
	rtp.uiDecoderID = htonl(0x00100000);//htonl(0x00660000 & 0x00ff00FF);//;//htonl(0x00660000 & 0x00ff00FF);/*解码库ID*/
	
	iLen = data_pkt_pt.pkt_size;//一帧图片的大小

	unsigned char *pBuffer = data_pkt_pt.pkt_pt;
    //printf("***************************%02x *********%dbuffer \n",pBuffer,iLen);
	rtp.seqno = seqnumber++;/*RTP包序列号,步长1递增*/

	rtp.ucFrameNum=iLen/iSend+1;/*帧内分包序号,从0开始*/
    
	while(iLen > iSend)
	{
		memset(sendbuffer,0,SEND_BUFFER_SIZE);	
		rtp.usPtLen = htons(iSend);
		rtp.ucFrameSeq = iCount;
		memcpy(sendbuffer,(char*)&rtp,sizeof(struct RtpHeader));
		memcpy(sendbuffer+sizeof(struct RtpHeader),pBuffer+iSend*iCount,iSend);
        //printf("sizeof(struct RtpHeader)%d",sizeof(struct RtpHeader));
        //printf("iSend%d\n",iSend);
        //printf("strlen(sendbuffer)%d %s\n",strlen(sendbuffer),sendbuffer);
        //printf("iLen%d\n",iLen);
        //printf("socket_send buffer = sendbuffer %d \n",iLen + sizeof(struct RtpHeader));
        //int iRet = socket_writen(info->skt, (char *)sendbuffer,iLen + sizeof(struct RtpHeader));
        //printf("[send]video_id/stream_type/skt/size/errno/strerror(errno)<<<%d/%d/%d/%d/%d/%s>>>\n",info->lp_media->media.video_id,info->lp_media->media.stream_type,info->skt,iRet,errno,strerror(errno));
		iRet = os_socket_send(info->skt,sendbuffer,SEND_BUFFER_SIZE,1000);
       
        if(iRet<0)
	    {
            //sleep(1);
            //printf("[111111111111111111111]Start_MEDIA+++++++++++++\n");
            printf("[1]net_send_stream  error %d\n",info->skt);
            //printf("\n==== Net Send Stream error  errno:%d,%s=====\n",iRet,strerror(iRet));
            os_mutex_lock(stop_mutex,-1);
            if(info->lp_media != NULL)
            {
                //memset(&stop_media,0,sizeof(stop_media));
                //strncpy(&stop_media,info->lp_media,sizeof(NET_MDU_START_MEDIA));
                //printf("[pop] media.video_id <<<%d>>>\n",(char*)stop_media.media.video_id);
            //printf("[pop] <<<%s>>>\n",(char*)info->lp_media->media.pu_id);
                //printf("[pop] media_type <<<%d>>>\n",(char*)stop_media.media.media_type);
                //printf("[pop] stream_type <<<%d>>>\n",(char*)stop_media.media.stream_type);
                //printf("[pop] request_type <<<%d>>>\n",(char*)stop_media.request_type);
                //printf("[pop] info <<<%d>>>\n",(unsigned int)info);
            //usleep(1);
               
                
                if (g_stream_proc)
        		{
        			g_stream_proc((uint32_t)info, "Stop_MEDIA", (char*)info->lp_media, sizeof(NET_MDU_START_MEDIA), g_param);
                    //printf("[pop] video_id/stream_type<<<%d/%d>>>\n",stop_media.media.video_id,stop_media.media.stream_type);
                    printf("[pop] video_id stream_type<<<%d/%d>>>\n",info->lp_media->media.video_id,info->lp_media->media.stream_type);
                    //printf(" call back success\n");
        		}
                
        		mdu_stream_pool_try_close(stream_pool, info, 1000);
            }
           os_mutex_unlock(stop_mutex);
    		//printf("[1]net_send_stream  error \n");
            //printf("info->skt%d\n",info->skt);

    		goto END;
	    }

		iLen -= iSend;
		iCount++;		
	}
  
	memset(sendbuffer,0,SEND_BUFFER_SIZE);
	rtp.usPtLen = htons(iLen);
    //printf("iLen%d\n",iLen);
	rtp.ucFrameSeq = iCount;

	memcpy(sendbuffer,&rtp,sizeof(struct RtpHeader));
	memcpy(sendbuffer+sizeof(struct RtpHeader),pBuffer+iSend*iCount,iLen);
    //printf("socket_send buffer = sendbuffer %d \n",iLen + sizeof(struct RtpHeader));
    //iRet = os_socket_send(info->skt, sendbuffer,SEND_BUFFER_SIZE,-1);
    iRet = os_socket_send(info->skt,sendbuffer,iLen + sizeof(struct RtpHeader),1000);
	//iRet = socket_writen(info->skt, (char *)sendbuffer,iLen + sizeof(struct RtpHeader));
	if(iRet<0)
	{
        //usleep(1);
        //sleep(1);
        printf("[2]net_send_stream  error %d\n",info->skt);
        //printf("\n==== Net Send Stream error  errno:%d,%s=====\n",iRet,strerror(iRet));
        os_mutex_lock(stop_mutex,-1);
        if(info->lp_media != NULL)
        {
            //memset(&stop_media,0,sizeof(stop_media));
            //strncpy(&stop_media,info->lp_media,sizeof(NET_MDU_START_MEDIA));
            //printf("[pop] media.video_id <<<%d>>>\n",(char*)stop_media.media.video_id);
            //printf("[pop] <<<%s>>>\n",(char*)info->lp_media->media.pu_id);
            //printf("[pop] media_type <<<%d>>>\n",(char*)stop_media.media.media_type);
            //printf("[pop] stream_type <<<%d>>>\n",(char*)stop_media.media.stream_type);
            //printf("[pop] request_type <<<%d>>>\n",(char*)stop_media.request_type);
            //printf("[pop] info <<<%d>>>\n",(unsigned int)info);
            //usleep(1);
                
            
            if (g_stream_proc)
    		{
    			g_stream_proc((uint32_t)info, "Stop_MEDIA", (char*)info->lp_media, sizeof(NET_MDU_START_MEDIA), g_param);
                //printf("[2pop] video_id stream_type<<<%d/%d>>>\n",stop_media.media.video_id,stop_media.media.stream_type);
                printf("[2pop] video_id stream_type<<<%d/%d>>>\n",info->lp_media->media.video_id,info->lp_media->media.stream_type);
                //printf(" call back success\n");
    		}
    		mdu_stream_pool_try_close(stream_pool, info, 1000);    
        }
        os_mutex_unlock(stop_mutex);
        
		goto END;
	}
      #endif
END:
	if(NULL != data_pkt_pt.pkt_pt  || data_pkt_pt.pkt_size != 0)
    {
		free(data_pkt_pt.pkt_pt);
		data_pkt_pt.pkt_pt = NULL;
		data_pkt_pt.pkt_size = 0;
        
	}
	return 0;
}
#endif

//////////////////////////////////////////////////////////////////////////

/* 打开线程，打开后处于暂停状态，priority=0(最高), priority=255(最低) */
void* mdu_stream_create( LPNET_MDU_START_MEDIA lp_media)
{	
	stream_info_t *info = (stream_info_t*)os_sys_malloc(sizeof(stream_info_t), __FILE__, __LINE__ );//[32]44
	if( info == NULL )
	  return NULL;

	memset( info, 0, sizeof(stream_info_t) );
	info->quit =  0;
	info->pause = 1;
	info->state = STREAM_STATE_READY;

	info->data_pkt_queue =(os_queue_datapos_t *)os_queue_data_create(sizeof(data_pkt_info_t), MAX_DATA_PKT_QUEUE_SIZE);//[33]12044
	if (NULL == info->data_pkt_queue )
	{
		return NULL;
	}

    info->audio_data_pkt_queue =(os_queue_datapos_t *)os_queue_data_create(sizeof(data_pkt_info_t), MAX_DATA_PKT_QUEUE_SIZE);//[33]12044
	if (NULL == info->audio_data_pkt_queue)
	{
		return NULL;
	}
	info->lp_media = (LPNET_MDU_START_MEDIA)os_sys_malloc(sizeof(NET_MDU_START_MEDIA), __FILE__, __LINE__ );//[36]1408

	memcpy(info->lp_media, lp_media, sizeof(NET_MDU_START_MEDIA));

	info->media_type  = lp_media->media.media_type;
    info->lp_media->scale=1.0;
	info->thrd  = NULL;
	info->data_pkt_queue->flag = 0;
	info->data_pkt_queue->limit_num = 0;
	info->data_pkt_queue->limit_time = 0;
	//创建事件处理线程

	if(lp_media->media.media_type == 0){

		LOG("\n  ====mdu_audio_proc  handle=====\n");

		if( ( info->thrd = os_thread_pool_alloc(thrd_pool, mdu_audio_proc,
							(unsigned long)info,  THREADPOOL_TIMEOUT) ) == NULL )
		{
			printf("THREADPOOL_TIMEOUT  error !\n");
            LOG("THREADPOOL_TIMEOUT  error !\n");
			goto STREAM_FAILED;
		}	

	}else{

	    //LOG("\n  ====mdu_viod_proc  handle=====\n");

		if( ( info->thrd = os_thread_pool_alloc(thrd_pool, mdu_stream_proc,
							(unsigned int)info,  THREADPOOL_TIMEOUT) ) == NULL )//[37]136
		{
            LOG("[mdu_stream_create] [os_thread_pool_alloccreate] mdu_stream_proc failed\n");
			printf("THREADPOOL_TIMEOUT  error !\n");
			goto STREAM_FAILED;
		}	
	}
	//锁
	if( ( info->mutex = os_mutex_create() ) == NULL )//[38]24
	{
		printf("STREAM_FAILED  1 error !\n");
		goto STREAM_FAILED;
	}

	//创建事件处理线程

	//LOG("\n mdu_stream ip is %s\n",lp_media->target_ip);
	//LOG("\n mdu_stream port is %d\n",lp_media->target_port);
	info->skt = mdu_stream_socket_create(lp_media->target_ip, (unsigned int)lp_media->target_port);
	if( info->skt < 0 )
	{
        LOG("mdu_stream_socket_create failed \n");
		printf("STREAM_FAILED  2 error !\n");
		goto STREAM_FAILED;
	}
	else
	{
		//LOG("\n======mdu_send_media_1=========\n");
		//printf("[1mdu_stream_create] info->skt <<%d/%s/%d>>\n",info->skt,lp_media->target_ip,lp_media->target_port);
		mdu_send_media(info->skt, info->lp_media);//发送INFO包
		//printf("[2mdu_stream_create] info->skt <<%d>>\n",info->skt);
	}
	return (void*)info;

STREAM_FAILED:
	mdu_stream_destroy( info, 10 );
	return NULL;
}

/* 销毁线程(可能会强制销毁)，返回0表示成功,-1表示失败 */
int32_t mdu_stream_destroy( void* stream, int32_t timeout )
{

	stream_info_t *info = (stream_info_t*)stream;

	if(info->mutex)
	{
		os_mutex_destroy(info->mutex);
	}

	//关闭线程
	if( info->thrd != NULL )
	{
		//通知线程结束
		info->pause = 0;
		info->quit = 1;
		info->state = STREAM_STATE_EXIT;

		os_thread_pool_free(thrd_pool, info->thrd, THREADPOOL_TIMEOUT);
	}
    if(info->skt > 0)
    {
        close(info->skt);
        //printf("close(info->skt)\n");
    }

    if(info->data_pkt_queue != NULL)
    {
        os_queue_clear(info->data_pkt_queue);
        if( info->data_pkt_queue->mutex )
        {
            os_mutex_destroy( info->data_pkt_queue->mutex);
            printf("info->data_pkt_queue->mutex\n\n");
        }
	        
	    if( info->data_pkt_queue->event )
        {
            os_event_destroy( (char *)info->data_pkt_queue->event );
            printf("info->data_pkt_queue->event\n\n");
        }
        os_sys_free(info->data_pkt_queue, __FILE__, __LINE__);
        info->data_pkt_queue = NULL;
    }
    if(info->audio_data_pkt_queue != NULL)
    {
        os_queue_clear(info->audio_data_pkt_queue);
        if( info->audio_data_pkt_queue->mutex )
        {
            os_mutex_destroy( info->audio_data_pkt_queue->mutex);
            printf("info->data_pkt_queue->mutex\n\n");
        }
	        
	    if( info->audio_data_pkt_queue->event )
        {
            os_event_destroy( (char *)info->audio_data_pkt_queue->event );
            printf("info->data_pkt_queue->event\n\n");
        }
     
        os_sys_free(info->audio_data_pkt_queue, __FILE__, __LINE__);
        info->audio_data_pkt_queue = NULL; 
    }
	os_sys_free( info->lp_media, __FILE__, __LINE__ );
    info->lp_media = NULL;
	os_sys_free( info, __FILE__, __LINE__ );
    info = NULL;
	//os_queue_data_destroy(g_DataQueue);

	return 0;
}

int32_t mdu_stream_compare( void* stream, LPNET_MDU_START_MEDIA lp_media)
{
	stream_info_t *info = (stream_info_t*)stream;
    
	if(info->lp_media == NULL)
	{
      //printf("[mdu_stream_compare] stream is null\n");
	  return -1;

	}
    //printf("[mdu_stream_compare]video id <<<%d/%d>>>\n",info->lp_media->media.video_id,lp_media->media.video_id);
   // printf("[mdu_stream_compare]media_type <<<%d/%d>>>\n",info->lp_media->media.media_type,lp_media->media.media_type);
    //printf("[mdu_stream_compare]stream_type <<<%d/%d>>>\n",info->lp_media->media.stream_type,lp_media->media.stream_type);
    //printf("[mdu_stream_compare]request type :%d,File Name:%s/\n",lp_media->request_type,info->lp_media->fileName);
    
	if(info->lp_media->media.video_id == lp_media->media.video_id
				&& info->lp_media->media.media_type  == lp_media->media.media_type
				&& info->lp_media->media.stream_type == lp_media->media.stream_type
				&&lp_media->request_type==REQUEST_REAL_STREAM)
	{
        //printf("[mdu_stream_compare] find stream\n");
		return 0;
	}
	else if(!strcmp(info->lp_media->fileName, lp_media->fileName) 
				&& (lp_media->request_type==REQUEST_FILE_STREAM  || 
				lp_media->request_type==DOWNLOAD_FILE_STREAM))
	{
      printf("file name is ok\n");
		return 0;
	}
        
	return -1;
}


/* 设置回调函数 */
void mdu_stream_set_media( void* stream, LPNET_MDU_START_MEDIA lp_media)
{
	stream_info_t *info = (stream_info_t*)stream;
	int cmd =0;

	if(lp_media != NULL && info->lp_media == NULL)
	{       
		os_queue_data_clear(info->data_pkt_queue,cmd);
        os_queue_data_clear(info->audio_data_pkt_queue,cmd);

		if(info->data_pkt_queue != NULL){

			//os_queue_data_destroy( info->data_pkt_queue );
		}

		//info->data_pkt_queue = os_queue_data_create(sizeof(data_pkt_info_t), MAX_DATA_PKT_QUEUE_SIZE);

		info->lp_media = (LPNET_MDU_START_MEDIA)os_sys_malloc(sizeof(NET_MDU_START_MEDIA), __FILE__, __LINE__ );

		if( info->lp_media != NULL )
		{

			memcpy(info->lp_media, lp_media, sizeof(NET_MDU_START_MEDIA));
		}

		if(info->skt > 0)
		{
			os_socket_destroy(info->skt);
		}

		info->skt = mdu_stream_socket_create(lp_media->target_ip, (unsigned int)lp_media->target_port);
        //printf("[INVITE]skt %d\n",info->skt);
        printf("[mdu_stream_set_media] skt/target_ip/target_port<<%d/%s/%d>>\n",info->skt,lp_media->target_ip,lp_media->target_port);
        mdu_send_media(info->skt, info->lp_media);
        //printf("[2mdu_stream_set_media]info->skt %d\n",info->skt);

		info->media_type = lp_media->media.media_type;
		info->quit = 0;
		//		info->pause = 1;
		info->state = STREAM_STATE_RUNNING;	
		//info->sendsize = 0;
	}
	else
	{
        if(info->lp_media != NULL)
        {
            printf("[stop] video_id/stream_type/skt <<<%d/%d/%d>>>\n",info->lp_media->media.video_id,info->lp_media->media.stream_type,info->skt);
    		LOG("[mdu_stream_set_media] video_id/stream_type <<<%d/%d>>>\n",info->lp_media->media.video_id,info->lp_media->media.stream_type);	
    		os_sys_free( info->lp_media, __FILE__, __LINE__ );		
    		info->lp_media = NULL;
    		if(info->skt >0)
    		{
              printf("clost socket :%d\n",info->skt);
    		  os_socket_destroy(info->skt);
    		}
    		info->state = STREAM_STATE_EXIT;
         }
       
	}

}

/* 取得线程执行函数 */
void mdu_stream_get_media( void* stream, LPNET_MDU_START_MEDIA* lp_media)
{
	stream_info_t *info = (stream_info_t*)stream;

	*lp_media = info->lp_media;

}

/* 暂停 */
void mdu_stream_pause( void* stream )
{

	//LOG("<<< mdu_stream_pause >>>.\n");	
	stream_info_t *info = (stream_info_t*)stream;
	os_thread_pause(info->thrd);
	info->pause = 1;
	info->busy = 0;
}




/* 继续 */
void mdu_stream_resume( void* stream )
{
	stream_info_t *info = (stream_info_t*)stream;

	os_thread_resume(info->thrd);
	info->pause = 0;
	info->state = STREAM_STATE_RUNNING;

}



/* 是否在用户执行函数中没有返回 */
int32_t mdu_stream_is_busy( void* stream )
{
	stream_info_t *info = (stream_info_t*)stream;
	return info->busy;
}

/* 线程执行函数 */
static int32_t mdu_stream_proc( uint32_t lpParam )
{
	stream_info_t *info = (stream_info_t *)lpParam;

	int timeout = 0;
    //int ret;

	if (info->state == STREAM_STATE_RUNNING 
				&& info->quit == 0
				&& info->skt > 0) 
	{
		info->busy = 1;
		pop(lpParam);    
	}
    else
    {
		return 1000;
	}
	return timeout;
}


static int32_t mdu_audio_proc(  uint32_t  lpParam )
{

	stream_info_t *info = (stream_info_t *)lpParam;

	int timeout = 0;

	//printf("\n==info->lp_media->media.media_type vlau is %d=====\n",info->media_type);
	if(info->media_type  == 0){

		if (info->state == STREAM_STATE_RUNNING 
					&& info->quit == 0
					&& info->skt > 0) 
		{

			info->busy = 1;
		//	push_audio(lpParam);
		}else{
			return 1000;
		}

		return timeout;

	}else{
		//printf("\n====mdu_stream_proc  IN  mdu_audio_proc =====\n");
		timeout = mdu_stream_proc(lpParam);

		return timeout;
	}
}


int32_t mdu_stream_socket_create(char* mdu_ip, unsigned int mdu_port)
{
	struct timeval timeo;
	int len = sizeof(timeo);
	int skt  = -1;

	skt = os_socket_create( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( skt < 0)
	{
		LOG("pu socket to mdu_invite failed ..........\n");
        printf("[mdu_stream_socket_create]skt < 0<<%d/%s>>\n",errno,strerror(errno));
		return -1;
	}

	/* set connect timeout */
	timeo.tv_sec = SOCKET_TIMEOUT;
	timeo.tv_usec = 0;
#if 1
	if(os_socket_opt(skt, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeo, len) < 0)
	{
		LOG("Set socket opt sendtimeout fail.");
        printf("Set socket opt sendtimeout fail.\n");
		return -1;
	}
#endif  

	if(os_socket_connect(skt, (char*)mdu_ip, mdu_port, /*5*1000*/-1) < 0)
	{
		LOG("[mdu_stream_socket_create]mdu stream socket connect[%s]-[%d] fail.", mdu_ip, mdu_port);
        printf("[mdu_stream_socket_create]mdu stream socket connect[%s]-[%d] fail.", mdu_ip, mdu_port);
		return -1;
	}
	//LOG("\n  create mdustream socket value %d \n",skt);
	return skt;
}


int32_t mdu_send_media(int skt, LPNET_MDU_START_MEDIA lp_media)
{
	char sip_pack[1024*2]={0};
	int  cnt = 0;

	switch(lp_media->request_type)
	{
		case REQUEST_REAL_STREAM:
          //printf("REQUEST_REAL_STREAM\n");
			if(build_media_info(lp_media->media.video_id, lp_media->media.media_type,lp_media->media.stream_type,sip_pack) > 0
						&& skt > 0)
			{
				cnt  = os_socket_send(skt, sip_pack, strlen(sip_pack), 1000);
                //printf("sip_pack info %s \n",sip_pack);
				if(cnt  < 0){
					LOG("\n [1]sockt use_2 select() errno:%d,%s",errno,strerror(errno));
                    printf("\n[1] sockt use_2 select() errno:%d,%s",errno,strerror(errno));
				}else if(cnt == 0 ){

					LOG("\n [2]sockt use_2 select() errno:%d,%s",errno,strerror(errno));
                    printf("\n [2]sockt use_2 select() errno:%d,%s",errno,strerror(errno));
				}else{

					LOG("\n[3]=====mdu_send_media_2==========\n");
                    //printf("\n[3]=====mdu_send_media_2==========\n");
				}

			}
            else
            {
                printf("build failed \n");
            }
			break;
		case REQUEST_FILE_STREAM:	
			if(build_fileback_info(lp_media->fileName, sip_pack) > 0
						&& skt > 0)
			{
				int ret;
				ret=os_socket_send(skt, sip_pack, strlen(sip_pack), 1000);
				if(ret<0)
				  LOG("\n sockt use_2 select() errno:%d,%s",errno,strerror(errno));
			}
			break;
		default:
			return -1;
	}

	return 0;
}


