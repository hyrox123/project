#include "stdafx.h"
#include "ms_mdu.h"
#include "ms_pu_glob.h"

#include "ms_mdu_msg.h"

#include "ms_mdu_stream_pool.h"
#include "oslib.h"

#define HOST_TRACE_DEBUG(x...)	 do{printf("[DBG .%s.%d][%s]",__FUNCTION__,__LINE__,HOSTMGR_NAME);printf(x);}while(0)

/***************************************************************/
handle_grp* mdu_hgrp  = NULL;

//unit_info*  mdu_info  = NULL;
/***************************************************************/


/***************************************************************/

int ms_mdu_initial()
{
	mdu_hgrp = (handle_grp*)os_sys_malloc( sizeof(handle_grp), __FILE__, __LINE__ );//[20]32
	mdu_skt  = (sock_info*)os_sys_malloc( sizeof(sock_info), __FILE__, __LINE__ );//[21]40
	mdu_info = (unit_info*)os_sys_malloc( sizeof(unit_info), __FILE__, __LINE__ );//[22]84	
    stop_mutex = os_mutex_create();
	mdu_hgrp->hdl_mutex = os_mutex_create();//[23]24
	if (mdu_hgrp->hdl_mutex == NULL)
	{
		//		LOG("mq mutex mdu create failure.");
		
		return -1;
	}

	/* create mdu socket message queue*/
	mdu_hgrp->hdl_mq[0] = os_queue_create(MQ_MSG_MAX_SIZE, 10);//[24]20664
	if(mdu_hgrp->hdl_mq[0] == NULL)
	{
		LOG("Message queue mdu create failure.");
        net_error_num = -17;
		return -1;
	}

	/* create mdu socket info receive thread*/

	mdu_hgrp->hdl_thrd_sock  = os_thread_pool_alloc(thrd_pool, thrd_sock_mdu_fun, (unsigned int)mdu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
    if(NULL == 	mdu_hgrp->hdl_thrd_sock)
	{
        LOG("[ms_mdu_initial]create pthread failed \n");
        net_error_num = -18;
        return -1;
    }

	/* create mdu register and keepalive thread*/

	mdu_hgrp->hdl_thrd_reg = os_thread_pool_alloc(thrd_pool, thrd_reg_mdu_fun, (unsigned int)mdu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
    if(NULL == 	mdu_hgrp->hdl_thrd_reg)
	{
        LOG("[ms_mdu_initial]create pthread failed \n");
        net_error_num = -19;
        return -1;
    }
	/* create mdu socket msg handle thread*/

	mdu_hgrp->hdl_thrd_msg = os_thread_pool_alloc(thrd_pool, thrd_msg_mdu_fun, (unsigned int)mdu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
    if(NULL == 	mdu_hgrp->hdl_thrd_msg)
	{
        LOG("[ms_mdu_initial]create pthread failed \n");
        net_error_num = -20;
        return -1;
    }

	/* initialize cmu info and local client socket info*/
	memset(mdu_info, 0, sizeof(unit_info));
	memset(mdu_skt, 0, sizeof(sock_info));

	mdu_info->is_reg   = REGISTER_OFF;
	mdu_info->last_kpl = time(0);

	mdu_skt->cli_skt = -1;

	os_thread_resume(mdu_hgrp->hdl_thrd_sock);
	os_thread_resume(mdu_hgrp->hdl_thrd_reg);
	os_thread_resume(mdu_hgrp->hdl_thrd_msg);

	stream_pool = mdu_stream_pool_create(16);//[31]88
	if(stream_pool == NULL)
	{
		LOG("[ms_mdu_initial]stream pool handle create failure.");
        net_error_num  = -21;
		return -1;		
	}

	return 0;
}

void ms_mdu_release(void)
{
    
        
	os_thread_pool_free(thrd_pool, mdu_hgrp->hdl_thrd_sock, THREADPOOL_TIMEOUT);
	os_thread_pool_free(thrd_pool, mdu_hgrp->hdl_thrd_reg, THREADPOOL_TIMEOUT);
	os_thread_pool_free(thrd_pool, mdu_hgrp->hdl_thrd_msg, THREADPOOL_TIMEOUT);
   
    if(NULL != mdu_hgrp->hdl_mutex)
	    os_mutex_destroy(mdu_hgrp->hdl_mutex);

	os_queue_clear(mdu_hgrp->hdl_mq[0]);

	os_queue_destroy(mdu_hgrp->hdl_mq[0]);
    mdu_hgrp->hdl_mq[0] = NULL;


	if(stream_pool != NULL)
	{
		mdu_stream_pool_destroy(stream_pool, 1000);
	}
   
    if(mdu_skt->cli_skt > 0)
    {
        close(mdu_skt->cli_skt);
        //printf("close(mdu_skt->cli_skt)\n");
    }
   
	os_sys_free( mdu_hgrp, __FILE__, __LINE__ );
    mdu_hgrp = NULL;
	os_sys_free( mdu_skt, __FILE__, __LINE__ );
    mdu_skt = NULL;
	os_sys_free( mdu_info, __FILE__, __LINE__ );
     mdu_info = NULL;
     os_sys_free( cmu_info, __FILE__, __LINE__ );
    cmu_info = NULL;
    os_sys_free( cmu_skt, __FILE__, __LINE__ );
    cmu_skt = NULL;
	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

int32_t thrd_sock_mdu_fun(uint32_t param)
{
	void *mq_handle = (void*)param;
	int* skt = &mdu_skt->cli_skt;

	int rc = -1;

	char ip[16] = {0};
	unsigned int port = 0;

	unsigned char buf[MAX_BUFFER_LEN]={0};

	char local_ip[16] = {0};
	unsigned int local_port = 0;
	memcpy(local_ip, cmu_skt->cli_ip, sizeof(local_ip));
    

	if(cmu_info->is_reg == REGISTER_OFF)
	{

		return 1000;
	}
    
	if (*skt > 0 )
	{

		rc = os_socket_recvfrom(*skt, ip, (size_t*)&port, buf, MAX_BUFFER_LEN,1000);

		if(rc==0 )
		{
           //  printf("mdu post message to mq fail11111111.%d,%s\n",errno,strerror(errno));
		     return 5;
		}
		else if(rc < 0)
		{
            printf("mdu post message to mq fail222222222.%d,%s\n",errno,strerror(errno));
            LOG("recvfrom mdu failed \n");
			os_socket_destroy(*skt);
			os_mutex_lock(mdu_hgrp->hdl_mutex, -1);
			*skt = -1;	
			mdu_info->is_reg = REGISTER_OFF;	/**/
			os_mutex_unlock(mdu_hgrp->hdl_mutex);
			return 1000;
		}
		else
		{
			buf[rc]='\0';
            //printf("os_socket_recvfrom+++++++++++++++buf = %s\n",buf);
			if(os_queue_post(mq_handle, 0, buf, rc) < 0)
			{
				LOG("mdu post message to queue fail.");
               // printf("mdu post message to mq fail2222.%d,%s\n",errno,strerror(errno));
			}
		}
	}
	else
	{
		//LOG("mdu-ip-[%s], port-[%d].\n", local_ip, local_port);
		if(mdu_socket_create(local_ip, local_port) < 0)
		{
			return 1000;
		}
	}
	return 1000;
}



int32_t thrd_reg_mdu_fun(uint32_t param)
{
	int* skt         = &mdu_skt->cli_skt;
	int* is_reg      = &mdu_info->is_reg;
	int* kpl_period  = &mdu_info->kpl_period;
	time_t* last_kpl = &mdu_info->last_kpl;
	int * kpl_num = &mdu_info->kpl_num;

	char* remote_ip   = mdu_info->sip_ip;
	unsigned int remote_port = mdu_info->sip_port;

	if(cmu_info->is_reg == REGISTER_OFF)
	{
		//LOG("cmu is unregister, please wait a moment...");
		return 100;
	}

	if(*skt > 0 && *is_reg == REGISTER_OFF )
	{
        printf("register_to_mdu\n");
		register_to_mdu(*skt, remote_ip, remote_port);
		//LOG("send mdu sip register packet");
	}

	if (*skt > 0 && *is_reg == REGISTER_ON )
	{
		if(time(NULL) - (*last_kpl) >= *kpl_period)
		{
			keepalive_to_mdu(*skt, remote_ip, remote_port);

			*last_kpl = time(NULL);
			mdu_info->kpl_num--;

			if(*kpl_num <= 0)
			{
				LOG("\n  send MDU keep live Failed\n");
				*is_reg = REGISTER_OFF;
				mdu_info->cmd = 1;
				mdu_stream_pool_clear(stream_pool, 30);	
			}
		}
	}


	return 1000;
}


int32_t thrd_msg_mdu_fun(uint32_t param)
{
	void *mq_handle = (void*)param;
	int* skt        = &mdu_skt->cli_skt;
	char buf[MAX_BUFFER_LEN] = {0};

	int type = -1;
	if( os_queue_read_timeout(mq_handle, (size_t*)&type,  buf, MAX_BUFFER_LEN, MQ_TIMEOUT) == 0)
	{   
        //printf("{{{{{{{{{{{{{{{{{{{{os_queue_read_timeout%s \n",buf);
		handle_mdu_msg(*skt, buf, strlen(buf));//Segmentation fault.
	}
    else
    {
        //printf("____________________________queue failed\n");
    }

	return 1000;
}



int mdu_socket_create(char* mdu_ip, unsigned int mdu_port)
{
	int skt = -1;
	char ip[16] = {0};
	unsigned int port  = 0;

	struct sockaddr_in sock_name;
#ifdef WIN32
	int name_len = sizeof(sock_name);
#else
	socklen_t name_len = sizeof(sock_name);
#endif


	skt = os_socket_create( AF_INET, SOCK_DGRAM, 0 );
	if( skt < 0)
	{
		LOG("sip socket create failure.");
		return -1;
	}
    //printf("mdu_ip _+++++++++++++++++++++++++++++++++++++++%d\n",mdu_port);
	if(os_socket_bind(skt, mdu_ip, htons(mdu_port)) < 0)
	{
		LOG("sip Socket bind[%s]-[%d] fail.", mdu_ip, mdu_port);
		return -1;
	}

	/* get local ip and port */
	getsockname(skt, (struct sockaddr*)&sock_name, &name_len);
	memcpy(ip, (char *)inet_ntoa(sock_name.sin_addr), sizeof(ip));
	port = ntohs(sock_name.sin_port);

	/* assign to global socket info */
	memcpy(mdu_skt->cli_ip, ip, sizeof(mdu_skt->cli_ip));
	memcpy(mdu_skt->ser_ip, cmu_info->sip_ip, sizeof(cmu_info->sip_ip));
	mdu_skt->cli_port = port;
	mdu_skt->ser_port = cmu_info->sip_port;
	mdu_skt->cli_skt  = skt;

	memset(mdu_info, 0, sizeof(unit_info));
	memcpy(mdu_info->sip_ip, cmu_info->sip_ip, sizeof(mdu_info->sip_ip));
	mdu_info->sip_port   = cmu_info->sip_port;
	mdu_info->is_reg     = REGISTER_OFF;
	mdu_info->kpl_period = KPL_MDU_PERIOD;
	mdu_info->last_kpl   = time(0);

	//LOG("Success bind loacal [%s]-[%d] success, remote [%s]-[%d].",ip, port, mdu_skt->ser_ip, mdu_skt->ser_port);

	return 0;
}


void register_to_mdu(int skt, char* ip, unsigned int port)
{
	int ret = 0;
	static long g_seq = 0;
	char senddata[1024];
	memset(senddata, 0, sizeof(senddata));

	ret = sprintf(senddata, "REGISTER sip:%s SIP/2.0\r\n"\
				"To: %s\r\n"\
				"From: %s\r\n"\
				"CSeq: %ld REGISTER\r\n"
				"Call-ID: 323eaa-850ee-bf8797\r\n"\
				"Max-Forwards: 70\r\n"\
				"Via: SIP/2.0/UDP %s:%d\r\n"\
				"Contact: <sip:%s>\r\n"\
				"Content-Length: 0\r\n\r\n",
				cmu_info->sip_ip,
				cmu_info->pu_id,
				cmu_info->pu_id,
				g_seq++,
				cmu_skt->cli_ip,
				cmu_info->sip_port,
				cmu_skt->cli_ip);

    //printf("send register %s \n",senddata);
	//LOG("thrd_reg_mdu_fun ip-port=[%s]-[%d] register packet -[\n%s\n]", ip, port, senddata);

	if( os_socket_sendto(skt, ip, port, senddata, strlen(senddata), 1000) != (int32_t)strlen(senddata) )
	{
		LOG("thrd_reg_mdu_fun register packet send to mdu fail errno:%d,%s.",errno,strerror(errno));
	}
}

void keepalive_to_mdu(int skt, char* ip, unsigned int port)
{
	char buf[MAX_BUFFER_LEN];
    memset(buf,0,sizeof(MAX_BUFFER_LEN));
	if( build_kpl_packet(buf) > 0 )
	{
        //printf("send keepalive %s \n",buf);
		//		LOG("thrd_reg_mdu_fun keepalive packet -[\n%s\n]", buf);
		if( os_socket_sendto(skt, ip, port, buf, strlen(buf), 1000) != (int32_t)strlen(buf))
		{
			LOG("thrd_reg_mdu_fun keepalive packet send to mdu fail.errno:%d,%s",errno,strerror(errno));
		}
	}
}

int handle_mdu_msg(int skt, char* buf, int len)
{
    
	void* stream = NULL;
	//printf("Get Mdu buf =%s\n",buf);
	if(strstr(buf, "REGISTER"))//处理注册信息
	{
		if(parse_reg_packet(buf) == 0)//parse解析
		{
			mdu_info->is_reg = REGISTER_ON;
			mdu_info->kpl_num = 1;//心跳一次


			//LOG("<<<  register to mdu success ooxxx >>>.\n");
			keepalive_to_mdu(skt, mdu_info->sip_ip, mdu_info->sip_port);
		}
        else
        {
            LOG("[handle_mdu_msg] register parse failed\n");
        }
	}
	else if(strstr(buf, "SIP/2.0 200 OK") && strstr(buf, "OPTIONS"))//处理心跳信息
	{
		if(parse_kpl_packet(buf) == 0)
		{
			mdu_info->kpl_num++;
		}
        else
        {
            LOG("[handle_mdu_msg] keepalive parse failed\n");
        }
	}

	else if(strstr(buf, "INVITE"))
	{
		//LOG("<<<  open stream success ooxxx_start >>>.\n");	
		//printf("|||||||||||||||||Get Mdu buf =%s\n",buf);
		NET_MDU_START_MEDIA start_media;
		memset(&start_media, 0, sizeof(NET_MDU_START_MEDIA));
		if( parse_invite_packet(buf, &start_media) == 0)
		{
             /*printf("INVITE from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					start_media.start_time.year,
					start_media.start_time.month,
					start_media.start_time.date,
					start_media.start_time.hour,
					start_media.start_time.minute,
					start_media.start_time.second);//开始的日期及时间
			*/
       //   printf("<<<  open stream success ooxxx_start __\n");
			//LOG("<<<  open stream success ooxxx_start ___%d >>>.\n",start_media.media.media_type);
			if(stream_pool == NULL)
			{
                printf("<<<  stream_pool   IS NULL%d >>>.\n",start_media.media.media_type);
                LOG("stream_pool create failed stream_pool is NULL\n");
            }
			//printf("start_media.media.stream_type%d\n",start_media.media.stream_type);
			void * stream = mdu_stream_pool_open(stream_pool, &start_media, 1000);//info
			//回调	
            //  void *stream = "ahd";
            if(strcmp(start_media.cmd,"download")==0)
            {
                printf("down load down load\n");
                start_media.request_type=2;
            }
            
			if(stream == NULL)
            {
                printf("<<<  open stream fail  >>>.\n");
				LOG("<<<  open stream fail  >>>.\n");	
				start_media.result_code = 1;
			}
            #if 1
			else if (g_stream_proc){
                 //printf("g_stream_proc  g_stream_proc  g_stream_proc\n");
				if(start_media.media.media_type != 0)
				{
                    //printf("[111111111111111111111]Start_MEDIA+++++++++++++\n");
                    //printf("[INVITE]video_id <<<%d>>>\n",start_media.media.video_id);
                    //printf("[INVITE] <<<%s>>>\n",start_media.media.pu_id);
                    //printf("[INVITE]video_id/stream_type/skt<<<%d/%d/>>>\n",start_media.media.video_id,start_media.media.stream_type);
                    //printf("[INVITE]media_type <<<%d>>>\n",start_media.media.media_type);
                    //printf("[INVITE]request_type<<<%d>>>\n",start_media.request_type);
                    //printf("[INVITE]stream<<<%d>>>\n",(unsigned int)stream);
                    g_stream_proc((unsigned long)stream, "Start_MEDIA", (char*)&start_media, sizeof(start_media), g_param);//回调请求包Segmentation fault.
                    //printf("[2]Start_MEDIA+++++++++++++\n");
                }
				  
				else
				  g_stream_proc((unsigned long)stream, "Start_AUDIO", (char*)&start_media, sizeof(start_media), g_param);	

			}
            else
            {
                LOG("stream_proc is NULL\n");
                printf("22222222222222222222222\n");
            }
              #endif
			memset(buf, 0, sizeof(buf));
			if(build_invite_resp_packet(buf, &start_media) > 0)//回复 200 OK
			{
				int ret;
                //printf("send rtsp :%s\n",buf);
				ret=os_socket_sendto(skt, mdu_info->sip_ip, mdu_info->sip_port, buf, strlen(buf), 1000);//往mdu发包
				if(ret<0)
				  LOG("send invite packet error  errno:%d,%s.",errno,strerror(errno));
			}
            else 
			{
                  LOG("build_invite_resp_packet failed\n");
                  printf("333333333333333\n");

			}
           
		}
        else
        {
            LOG("[handle_mdu_msg] invite parse failed\n");
        }
       
	}
	else if(strstr(buf,"BYE"))
	{
      # if 1
        printf("++++++++++++++++++%s\n",buf);
		NET_MDU_START_MEDIA stop_media;
     
		if( parse_bye_packet(buf, &stop_media) == 0
					&& build_bye_resp_packet(buf) > 0
					&& os_socket_sendto(skt, mdu_info->sip_ip, mdu_info->sip_port, buf, strlen(buf), 1000) == (int32_t)strlen(buf))
		{
             //printf("Get BYE packet CH:%d\n",stop_media.media.video_id);

             
			//NET_MDU_START_MEDIA param;

			//memset(&param, 0, sizeof(NET_MDU_START_MEDIA));
		//	memcpy(&param.media, &stop_media.media, sizeof(NET_MDU_MEDIA));

			//memcpy(&param, &stop_media, sizeof(NET_MDU_START_MEDIA));

            //printf("[BYE] video_id<<<%d>>>\n",stop_media.media.video_id);
            //printf("[BYE] <<<%s>>>\n",stop_media.media.pu_id);
            //printf("[BYE] MediaType<<<%d>>>\n",stop_media.media.media_type);
            //printf("[BYE] StreamType<<<%d>>>\n",stop_media.media.stream_type);
            //printf("[BYE] request_type<<<%d>>>\n",stop_media.request_type);
            //if()
           #if 1
            os_mutex_lock(stop_mutex,-1);
			stream = mdu_stream_pool_find(stream_pool, &stop_media, 1000);
            //printf("[BYE] <<<%d>>>\n",(unsigned int)stream);
            if(stream == NULL)
			{
              LOG("didn't find stream\n");
			  printf("stream is null\n");
              os_mutex_unlock(stop_mutex);
              return -1;
			}
            
            stream_info_t *info=(stream_info_t *)stream;
            //stream->
			if(stream)
			{
				if(stop_media.media.media_type != 0)
				{
					//LOG("<<< ====CLOSE  VOID_STREAM=====>>>.\n");	
					
                    //printf("Type Close is voder\n");
					//回调	
					
					if (g_stream_proc)
					{
						g_stream_proc((uint32_t)stream, "Stop_MEDIA", (char*)&stop_media, sizeof(stop_media), g_param);
                        //printf("[BYE] video_id stream_type<<<%d/%d>>>",stop_media.media.video_id,stop_media.media.stream_type);
                        //printf(" call back success\n");
					}
                    mdu_stream_pool_try_close(stream_pool, stream, 1000);
				}
				else
				{

					//LOG("<<< ====CLOSE  AUDIO_STREAM=====>>>.\n");	
					mdu_stream_pool_try_close(stream_pool, stream, 1000);
					//回调	
					if (g_stream_proc)
					{
						g_stream_proc((uint32_t)stream, "Stop_AUDIO", (char*)&stop_media, sizeof(stop_media), g_param);
					}
				}
			}
            os_mutex_unlock(stop_mutex);

            #endif

			//LOG("<<<  Close Stream Success ooxxx >>>.\n");	
		}
        else
        {
            LOG("[handle_mdu_msg] bye parse failed\n");
        }
#endif

	}
	else if(strstr(buf,"ACK"))
	{
        //printf("++++++++++++++++++%s\n",buf);
		if(parse_ack_packet(buf) == 0)
		{
			//LOG("<<<  ACK message success ooxxx >>>.\n");	
		}
        else
        {
            LOG("[handle_mdu_msg] ack parse failed\n");
        }
	}
	return  100;
}
