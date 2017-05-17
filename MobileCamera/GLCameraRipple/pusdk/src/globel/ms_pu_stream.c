#include <stdio.h>
#include "osqueue_g.h"
#include "ms_mdu_stream.h"
#include "ms_pu_type.h"
#include "ms_mdu_stream.h"
#include "ms_mdu_stream_pool.h"
#include "ms_pu_lib.h"
#include "ms_pu_glob.h"
#include "ossys.h"
#include "help.h"

extern uint32_t	g_param;

int32_t mdu_stream_post_data( void* stream, char* buffer, int32_t size,int frame_type)
{
	stream_info_t *info = (stream_info_t *)stream;
	int iRet = -1;

	if (info->state == STREAM_STATE_RUNNING 
				&& info->quit == 0
				&& info->skt > 0) 
	{
		iRet  = push(stream, buffer, size,frame_type);
        //printf("***************************%02x *********%dbuffer \n",buffer,size);
		return  iRet;
	}
   
	return iRet;
}

int32_t mdu_stream_receive_data( void* stream, char* buffer,  size_t * size )
{
	stream_info_t *info = (stream_info_t *)stream;
	//int i = 0 ;
	int iRet = -1;


	if (info->state == STREAM_STATE_RUNNING 
				&& info->quit == 0
				&& info->skt > 0) 
	{

	//	iRet =  pop_audio(info,buffer,size) ;

		return  iRet;
	}


	return iRet;
}

int push(void* stream, char* buffer, int32_t size,int frame_type)
{
    if(buffer == NULL || stream == NULL)
    {
        printf("in push buffer == NULL || stream == NULL\n");
        LOG("[push] push buffer == NULL || stream == NULL\n");
        return 0;
    }
    
	//struct RtpHeader * p ;
	//char * utLen  = NULL;

	stream_info_t *info = (stream_info_t *)stream;

	data_pkt_info_t data_pkt_pt;//point

	data_pkt_pt.pkt_size = size;
	//unsigned char *pkt = (unsigned char *)os_sys_malloc(size, __FILE__, __LINE__ );
	unsigned char *pkt = (unsigned char *)malloc(size);
    //printf("[push] +++++++++++++++%d \n",size);
	data_pkt_pt.pkt_pt = pkt;
	data_pkt_pt.frame_type = frame_type;
   
	memcpy(pkt,buffer,size);
	pkt = NULL;
	int iRet = -1;
    
    //音频数据，塞入音频队列
    if(frame_type == AUDIO)
    {
        if(info->audio_data_pkt_queue != NULL)
             iRet= os_queue_data_post(info->audio_data_pkt_queue,&data_pkt_pt, sizeof(data_pkt_info_t));
        //printf("push audio\n");
    }
    else
        iRet= os_queue_data_post(info->data_pkt_queue,&data_pkt_pt, sizeof(data_pkt_info_t));

	if (-1 == iRet)
	{
        if(NULL != data_pkt_pt.pkt_pt)
        {
            free(data_pkt_pt.pkt_pt);
		    data_pkt_pt.pkt_pt=NULL;
		    data_pkt_pt.pkt_size = 0;
        }
				
	}
	return 0;
}

/* 是否正在运行 */
int32_t mdu_stream_is_running( void* stream )
{
	stream_info_t *info = (stream_info_t*)stream;

	if(info->pause != 0 && info->quit != 0){
	}

	return info->pause == 0 && info->quit == 0;
}
int32_t mdu_stop_stream(void* stream)
{
    if(select_param==GB28181_PLATFORM)
    {
        #ifdef GB28181_LIB
        if(mdu_stream_is_running(stream))
        {

            server_stream_pool_try_close(thrd_pool, stream, 1000);
        }
        #endif
    }
    else if(select_param==MINGSOFT_PLATFORM)
    {
        #ifdef MINGSOFT_LIB
        if(mdu_stream_is_running(stream))
        {
            
            mdu_stream_pool_try_close(thrd_pool, stream, 1000);
        }
        #endif
    }
    return 0;
}
