#include "stdafx.h"
#include "ms_cmu.h"
#include "ms_cmu_cmd.h"
#include "ms_cmu_get.h"
#include "ms_cmu_oper.h"
#include "ms_cmu_set.h"
#include "ms_cmu_reg.h"

#include "ms_pu_glob.h"

#include "ms_pu_param.h"



#include "mxml.h"
#include "oslib.h"
#include <unistd.h>

/***************************************************************/
//

/***************************************************************/

/***************************************************************/
int ms_cmu_initial(char* cmu_ip, unsigned int cmu_port)//传入参数ip port
{
    //g_msg_proc(GET_LOCAL_IP,(char *)gb_g_info.sip_net.lcIp,(long unsigned int *)sizeof(gb_g_info.sip_net.lcIp),g_param);
    //printf("++++++++++++++++++++++++++++++++++++++++++%s+++++++++++===\n",gb_g_info.sip_net.lcIp);
	cmu_hgrp = (handle_grp*)os_sys_malloc( sizeof(handle_grp), __FILE__, __LINE__ );//[3]32
	cmu_skt  = (sock_info*)os_sys_malloc( sizeof(sock_info), __FILE__, __LINE__ );//[4]40
	cmu_info = (unit_info*)os_sys_malloc( sizeof(unit_info), __FILE__, __LINE__ );//[5]84
	cmu_alarm = (alram_data*)os_sys_malloc(sizeof(alram_data),__FILE__, __LINE__ );//[6]2936	

	cmu_hgrp->hdl_mutex = os_mutex_create();//创建互斥量[7]24

	memset(cmu_info,0,sizeof(unit_info));	
	cmu_info->is_reg   = REGISTER_OFF;//enum 0
	cmu_info->last_kpl = time(NULL);//得到机器的日历时间 单位为秒
	cmu_info->last_time = time(NULL); 	 	
	cmu_info->kpl_timing_count = 0;

	memset(cmu_alarm,0,sizeof(alram_data));
	cmu_alarm->mutex = os_mutex_create();//[8]24
	cmu_alarm->alarm_interval_time = 30;//interval间隔
	cmu_alarm->max_interval_time = 40;
	cmu_alarm->alarm_end_time = time(NULL);	//得到机器的日历时间
	cmu_alarm->alarm_start_time= time(NULL);

	/* create cmu socket message queue*/

	cmu_hgrp->hdl_mq[0] = os_queue_create(MQ_MSG_MAX_SIZE, MQ_MSG_MAX_NUM);//消息队列[9]41304
	if(NULL == cmu_hgrp->hdl_mq[0])
	{
		LOG("[ms_cmu_initial]Message queue cmu create failure.");
        net_error_num = -10;
        
		return -1;
	}

	cmu_hgrp->hdl_mq[1] = os_queue_create(MQ_MSG_MAX_SIZE, MQ_MSG_MAX_NUM);//云台的消息队列[12]41304
	if( NULL == cmu_hgrp->hdl_mq[1])
	{
		LOG("[ms_cmu_initial]Message queue cmu create failure.");
        net_error_num = -11;
		return -1;
	}

	/* create cmu socket info receive thread*/

	cmu_hgrp->hdl_thrd_sock = os_thread_pool_alloc(thrd_pool, thrd_sock_cmu_fun, (uint32_t)cmu_hgrp,THREADPOOL_TIMEOUT);//cmu_hgrq里面放着消息队列
	if(NULL == cmu_hgrp->hdl_thrd_sock)
	{
        LOG("[ms_cmu_initial]create pthread failed \n");
        net_error_num = -12;
        return -1;
    }
	/* create cmu register and keepalive thread*///接受thrd_pool创建线程池的返回值

	cmu_hgrp->hdl_thrd_reg = os_thread_pool_alloc(thrd_pool, thrd_reg_cmu_fun,(uint32_t)cmu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
	/* create cmu socket msg handle thread*///注册和心跳
    if(NULL == cmu_hgrp->hdl_thrd_reg)
	{
        LOG("[ms_cmu_initial]create pthread failed \n");
        net_error_num = -13;
        return -1;
    }
	cmu_hgrp->hdl_thrd_msg = os_thread_pool_alloc(thrd_pool, thrd_msg_cmu_fun,(uint32_t)cmu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
	//处理消息
    if(NULL == cmu_hgrp->hdl_thrd_msg)
	{
        LOG("[ms_cmu_initial]create pthread failed \n");
        net_error_num = -14;
        return -1;
    }
	cmu_hgrp->hdl_thrd_operate_msg = os_thread_pool_alloc(thrd_pool, thrd_ptzmsg_cmu_fun,(uint32_t)cmu_hgrp->hdl_mq[1],  THREADPOOL_TIMEOUT);
	//云台
    if(NULL == cmu_hgrp->hdl_thrd_operate_msg)
	{
        LOG("[ms_cmu_initial]create pthread failed \n");
        net_error_num = -15;
        return -1;
    }
	cmu_hgrp->hdl_thrd_alarm = os_thread_pool_alloc(thrd_pool, thrd_alarm_fun,(unsigned int)cmu_hgrp->hdl_mq[0],  THREADPOOL_TIMEOUT);
	//告警

    if(NULL == cmu_hgrp->hdl_thrd_alarm)
	{
        LOG("[ms_cmu_initial]create pthread failed \n");
        net_error_num = -16;
        return -1;
    }

	/* initialize cmu info and local client socket info*/
	memset(cmu_info, 0, sizeof(unit_info));
	memset(cmu_skt, 0, sizeof(sock_info));

	cmu_info->is_reg   = REGISTER_OFF;
	cmu_info->last_kpl = time(0);
	cmu_skt->cli_skt = -1;

	memcpy(cmu_skt->ser_ip, cmu_ip, sizeof(cmu_skt->ser_ip));
	cmu_skt->ser_port = cmu_port;

	//	LOG("cmu-ip-[%s], port-[%d].\n", cmu_skt->ser_ip, cmu_skt->ser_port);
	//LOG("ms cmu initial success.");

	os_thread_resume(cmu_hgrp->hdl_thrd_sock);//让线程继续执行
	os_thread_resume(cmu_hgrp->hdl_thrd_reg);
	os_thread_resume(cmu_hgrp->hdl_thrd_msg);
	os_thread_resume(cmu_hgrp->hdl_thrd_operate_msg);
	os_thread_resume(cmu_hgrp->hdl_thrd_alarm);

	return 0;
}

int ms_cmu_release()
{
        
	os_thread_pool_free(thrd_pool, cmu_hgrp->hdl_thrd_sock, THREADPOOL_TIMEOUT);//THREADPOOL_TIMEOUT);

	os_thread_pool_free(thrd_pool, cmu_hgrp->hdl_thrd_reg,  THREADPOOL_TIMEOUT);

	os_thread_pool_free(thrd_pool, cmu_hgrp->hdl_thrd_msg,  THREADPOOL_TIMEOUT);

	os_thread_pool_free(thrd_pool, cmu_hgrp->hdl_thrd_alarm,  THREADPOOL_TIMEOUT);
	os_thread_pool_free(thrd_pool, cmu_hgrp->hdl_thrd_operate_msg,  THREADPOOL_TIMEOUT);

    if(cmu_hgrp->hdl_mutex != NULL)
	    os_mutex_destroy(cmu_hgrp->hdl_mutex);
    if(cmu_alarm->mutex != NULL)
        os_mutex_destroy(cmu_alarm->mutex);
    

	os_queue_clear(cmu_hgrp->hdl_mq[0]);
	os_queue_destroy(cmu_hgrp->hdl_mq[0]);
    cmu_hgrp->hdl_mq[0] = NULL;
	os_queue_clear(cmu_hgrp->hdl_mq[1]);
	os_queue_destroy(cmu_hgrp->hdl_mq[1]);
    cmu_hgrp->hdl_mq[1] = NULL;
   
    #if 1
     if(cmu_skt->cli_skt > 0)
     {
        close(cmu_skt->cli_skt);
        //printf("close(cmu_skt->cli_skt)\n");
     }
     #endif
	os_sys_free( cmu_hgrp, __FILE__, __LINE__ );
    cmu_hgrp = NULL;
	
	os_sys_free(cmu_alarm, __FILE__, __LINE__ );
    cmu_alarm = NULL;

	return 0;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/

int32_t thrd_sock_cmu_fun(uint32_t param)//cmu_hgrq
{
	handle_grp*  mq_handle = (handle_grp*)param;
	int* skt = &cmu_skt->cli_skt;//tcp连接的文件描述符
	char ip[16] = {0};
	unsigned int port = 0;
	char name[128] = {0};
	unsigned short msg_id = 0, cmd_id = 0;

	memcpy(ip,cmu_skt->ser_ip,sizeof(ip));
	port = cmu_skt->ser_port;

	if (*skt > 0)//skt是socket 的返回值
	{
		int rc = -1;

		sock_header hdr;//指令头
		memset(&hdr, 0, sizeof(hdr));
		rc = os_socket_recv(*skt,(unsigned char*)&hdr,sizeof(sock_header),SOCKET_TIMEOUT*1000);

		if (rc == 0)
		{

			//超时处理
			//printf("[thrd_sock_cmu_fun]+++++++++\n");
			return 1000;
		}
		else if(rc < 0)
		{
    		os_socket_destroy(*skt);
			os_mutex_lock(cmu_hgrp->hdl_mutex, -1);
			*skt = -1;											
			cmu_info->is_reg = REGISTER_OFF;					
			cmu_info->cmd = 0;
			os_mutex_unlock(cmu_hgrp->hdl_mutex);
			LOG("cmu socket disconnect, please re-connect.");
            //printf("cmu socket disconnect, please re-connect.");
			return 5000;
		}
		else
		{
			int len = ntohs(hdr.length);

			//#char* buf = (char* )os_sys_malloc( len+1, __FILE__, __LINE__ );
			char buf[MAX_BUFFER_LEN] = {0};
			rc = os_socket_recv(*skt, buf, len, SOCKET_TIMEOUT*1000);
			if(rc < 0)
			{
				//os_sys_free( buf, __FILE__, __LINE__ );

				os_socket_destroy(*skt);

				os_mutex_lock(cmu_hgrp->hdl_mutex, -1);
				*skt = -1;											/**/
				cmu_info->is_reg = REGISTER_OFF;					/**/
				cmu_info->cmd = 0;
				os_mutex_unlock(cmu_hgrp->hdl_mutex);
				LOG("cmu socket disconnect, please re-connect.");
				return 5000;
			}
			else if(rc == len)
			{
				buf[len]='\0';
                //printf("get_msg_name%s\n",buf);
				if(get_msg_name(buf, name) < 0){
					return 2000;//name传入参数，getpuipinfo
				}

				if(get_msg_cmd_id(&msg_id, &cmd_id, name) < 0){
					return 5000;

				}

				if(cmd_id == CMD_OPER_CONTROLPTZ)
                {

					if(os_queue_post(mq_handle->hdl_mq[1], hdr.cseq, buf, len) < 0)
					{
						LOG("cmu post ptz_message to mq fail.");
					}
				}	
				else
                {

					if(os_queue_post(mq_handle->hdl_mq[0], hdr.cseq, buf, len) < 0)
					{
						LOG("cmu post message to mq fail.");
					}
				}
			}
		}
	}
	else
	{
		//LOG("cmu-ip-[%s], port-[%d].\n", ip, port);
		if(cmu_socket_create(ip, port) < 0)
		{
			LOG("cmu socket create fail.\n");
            printf("cmu socket create fail.\n");
			return 5000;
		}
	}

	return 1000;
}

int32_t thrd_reg_cmu_fun(uint32_t param)
{
    
	int* skt = &cmu_skt->cli_skt;
	int* is_reg = &cmu_info->is_reg;//是否有注册
	int* kpl_period = &cmu_info->kpl_period;
	//int* kpl_timing_count = &cmu_info->kpl_timing_count;

	time_t* last_kpl = &cmu_info->last_kpl;
	time_t* last_time = &cmu_info->last_time;

    if(*skt <0)
         return 1000;
    
	if(*skt > 0 && *is_reg == REGISTER_OFF )
	{   
        //printf("register cmu\n");
		cmu_register(*skt);
		*last_kpl = time(NULL);
		*last_time = time(NULL);

	}

	if (*skt > 0 && *is_reg == REGISTER_ON )
	{
		if(time(NULL) - (*last_kpl) >= *kpl_period)
		{
            //printf("keepalive cmu\n");
			cmu_keepalive(*skt);
			*last_kpl = time(NULL);
			//kpl_timing_count++;
			cmu_info->kpl_timing_count++;	
		}
        return 1000;
	}
	return 1000*5;
}

int32_t thrd_msg_cmu_fun(uint32_t param)//心跳
{
	void *mq_handle = (void*)param;
	int* skt  = &cmu_skt->cli_skt;

	unsigned int cseq = -1;
	//char *buf = os_sys_malloc( MAX_BUFFER_LEN, __FILE__, __LINE__ );
	char buf[MAX_BUFFER_LEN] = {0};
	if(os_queue_read_timeout(mq_handle,(size_t*)&cseq,buf,sizeof(buf),MQ_TIMEOUT) == 0)//从消息队列提取消息
	{
        //printf("cmu message %s \n",buf);
		handle_cmu_msg(*skt,cseq,buf,strlen(buf));
	}

	return 1000;
}

/*云台*/
int32_t thrd_ptzmsg_cmu_fun(uint32_t param)
{
	static int server_connect_status=0;//0:没有连接上,1:连接上了    
	void *mq_handle = (void*)param;
	//int* skt        = &cmu_skt->cli_skt;
	unsigned int cseq = -1;
	char buf[MAX_BUFFER_LEN] = {0};
	if( os_queue_read_timeout(mq_handle, (size_t*)&cseq,  buf, sizeof(buf), MQ_TIMEOUT) == 0)
	{
		//LOG("recv xml seq-[%d]-[%d]-[\n%s\n]", cseq, strlen(buf), buf);
		handle_ptz_oper_msg( buf, strlen(buf));
	}
	//    printf("\n =====time - cmu_info->last_time = %d=====\n",time(NULL) -cmu_info->last_time);
#if 1//用于通知pu端，连接的状态
	if((time(NULL) - cmu_info->last_time) >= 15){

		if( ((cmu_info->is_reg == REGISTER_OFF) &&(mdu_info->kpl_num > 0)))
		{                                                          
			mdu_info->loop_num++;
			if(mdu_info->loop_num > 2 ){

				LOG("[mdu_info->loop_num > 2]cmu server and mdu server off_line  >> equipment reboot");
				//handle_contrl_oper_msg(4);	
				//	printf("cmu server and mdu server off_line  >> equipment reboot");
				if(server_connect_status==1)
				{
                    //printf("")
					handle_contrl_oper_msg(3);
				}

			}
            else
            {
				LOG("[1]cmu server and mdu server off_line  >> equipment don't reboot");
				//   printf("cmu server and mdu server off_line  >> equipment don't reboot");
				if(server_connect_status==1)
				{
					server_connect_status=0;
					handle_contrl_oper_msg(3);
				}
			}

		}
		else if( ( cmu_info->cmd == 0  && (cmu_info->is_reg == REGISTER_OFF) &&(mdu_info->kpl_num <=0))){								
			LOG("[2]cmu server and mdu server off_line  >> equipment don't reboot");
			//printf("cmu server and mdu server off_line  >> equipment don't reboot");
			if(server_connect_status==1)
			{
				server_connect_status=0;
				handle_contrl_oper_msg(3);
			}

		}
        else if((  cmu_info->cmd == 1  && (cmu_info->is_reg == REGISTER_OFF) &&(mdu_info->kpl_num <=0) )){									
			LOG("[3]cmu server and mdu server off_line  >> equipment don't reboot");
			//printf("cmu server and mdu server off_line  >> equipment don't reboot");
			if(server_connect_status==1)
			{
				server_connect_status=0;
				handle_contrl_oper_msg(3);
			}

		}
        else if(( ( mdu_info->cmd == 0) &&(cmu_info->is_reg == REGISTER_ON) &&(mdu_info->kpl_num <= 0)) ){								
			LOG("[4]cmu server and mdu server off_line  >> equipment don't reboot");
			//printf("cmu server and mdu server off_line  >> equipment don't reboot");
			if(server_connect_status==1)
			{
				server_connect_status=0;
				handle_contrl_oper_msg(3);
			}
		}
		else if(( ( mdu_info->cmd == 1) &&(cmu_info->is_reg == REGISTER_ON) &&(mdu_info->kpl_num <= 0)) ){                                  
			LOG("[5]cmu server and mdu server off_line  >> equipment reboot");
			//printf("cmu server and mdu server off_line  >> equipment reboot");
			if(server_connect_status==1)
			{
				server_connect_status=0;
				handle_contrl_oper_msg(3);//没有连接上
			}

		}
		
		else
		{
			//printf("cmu server and mdu server online ");
		    //	LOG("cmu server and mdu server online ");
			if(server_connect_status==0)
			{
				server_connect_status=1;
				handle_contrl_oper_msg(2);//连接上了
			}
		}
		if(mdu_info->loop_num > 2)
		  mdu_info->loop_num = 0;
		cmu_info->last_time = time(NULL);
	}
#endif
	return 1000;
}

int32_t thrd_alarm_fun(uint32_t param)
{
	int* skt = &cmu_skt->cli_skt;
	int* is_reg = &cmu_info->is_reg;
	//int ret = 0;
	int size = sizeof(NET_CMU_UPLOAD_ALARM);//136
    //printf("da+++++++++++++++++++++++++++++++++++++++++++++++++++++%d\n",size);

	os_mutex_lock(cmu_alarm->mutex,-1);

	if(cmu_alarm->sendsize <= 0 || ((*skt < 0) && (*is_reg ==REGISTER_OFF )))
	{
		cmu_alarm->sendsize = 0;
		os_mutex_unlock(cmu_alarm->mutex);
		return 100;
	}

	char exchangeBuf[244] = {0};
    memset(exchangeBuf,0,sizeof(exchangeBuf));

	char *buf = cmu_alarm->sendbuf;

	memcpy(exchangeBuf,buf,size);

	LPNET_CMU_UPLOAD_ALARM prt_param = (LPNET_CMU_UPLOAD_ALARM)exchangeBuf;
#if 0
	printf("pu_id:%s\n",prt_param->pu_id);
	printf("pu_ip:%s\n",prt_param->pu_ip);
	printf("server_type:%s\n",prt_param->server_type);
	printf("alarm_type:%d\n",prt_param->alarm_type);
	printf("disk_num%d:\n",prt_param->disk_number);
	printf("time_stamp:%s\n",prt_param->time_stamp);
	printf("data.integerValue:%d\n",prt_param->data.integerValue);
	printf("eliminated:%s\n",prt_param->eliminated);
#endif
	buf += size;
	cmu_alarm->sendsize -= size;

	if(cmu_alarm->sendsize > 0)
    {
		memmove(cmu_alarm->sendbuf,buf,cmu_alarm->sendsize);
	}
	os_mutex_unlock(cmu_alarm->mutex);

	
	   //printf(" thrd_alarm_fun  info.sendsize is value	%d\n",cmu_alarm->sendsize);
	   //printf("\n=====alarm_type is %d=================\n",prt_param->alarm_type);
	   //printf("\n=====alarm_type is %s=================\n",prt_param->server_type);
	   
	cmu_upLoad_alarm(*skt,(char *)prt_param);

	return 100;
}

int cmu_socket_create(char* cmu_ip, unsigned int cmu_port)
{
	int skt = -1;
	struct timeval timeo;// 其中返回的timeval值为Epoch（00:00:00 1970-01-01 UTC）到创建struct timeval时的时间，tv_sec为秒数部分，tv_usec为微秒数部分（10的-6次方秒）
	int len = sizeof(timeo);
	char ip[16] = {0};
	unsigned int port  = 0;
	struct sockaddr_in sock_name;
#ifdef WIN32
	int name_len = sizeof(sock_name);
#else
	socklen_t name_len = sizeof(sock_name);
#endif

	skt = os_socket_create( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( skt < 0)
	{
		LOG("cmu socket failed ..........\n");
		close(skt);
		return -1;
	}

	/* set connect timeout */
	timeo.tv_sec = SOCKET_TIMEOUT;
	timeo.tv_usec = 0;
	//设置发送超时
	if(os_socket_opt(skt, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeo, len) < 0)
	{
		LOG("Set socket opt sendtimeout fail.");
		close(skt);
		return -1;
	}

	if(os_socket_connect(skt, (char*)cmu_ip, cmu_port, 5*1000) < 0)
	{
		LOG("cmu Socket connect[%s]-[%d] fail.", cmu_ip, cmu_port);
		close(skt);
		return -1;
	}

	/* get local ip and port */
	getsockname(skt, (struct sockaddr*)&sock_name, &name_len);
	memcpy(ip, (char *)inet_ntoa(sock_name.sin_addr), sizeof(ip));
	port = ntohs(sock_name.sin_port);
    //printf("cli_port %d \n",port);


	/* assign to global socket info */
	memcpy(cmu_skt->cli_ip, ip, sizeof(cmu_skt->cli_ip));
    //printf("+++++++++++++++++++++++++++++++++++++++++%s ++++++++++++++++++++++++++++\n",cmu_skt->cli_ip);
	memcpy(cmu_skt->ser_ip, cmu_ip, sizeof(cmu_skt->ser_ip));
	cmu_skt->cli_port = port;
	cmu_skt->ser_port = cmu_port;
	cmu_skt->cli_skt  = skt;
	/*memset(cmu_info, 0, sizeof(unit_info));
	  cmu_info->is_reg   = REGISTER_OFF;
	  cmu_info->last_kpl = time(0);
	  cmu_info->last_time = time(0);

*/
	//LOG("Success connect to server[%s]-[%d] from client[%s]-[%d] success.", cmu_ip, cmu_port, ip, port);
	return 0;
}

int get_msg_name(char* xml, char* msg_name)
{
	char *pos1 = NULL, *pos2=NULL;

	if(!xml)
	  return -1;

	pos1 = strstr(xml,"command=\"");
	if(!pos1||!pos1[0])
	  return -1;
	pos1 += strlen("command=\"");
	pos2  = strstr(pos1,"\">");
	*pos2='\0';

	strncpy(msg_name, pos1, strlen(pos1));
	*pos2='\"';


	//	LOG("<<<<Msg Name %s>>>>", msg_name);

	return 0;
}

int handle_cmu_msg(int skt, unsigned int cseq, char* buf, int len)
{

	char name[128] = {0};
	unsigned short msg_id = 0, cmd_id = 0;

	if(get_msg_name(buf, name) < 0)
	{
        LOG("cmu get_msg_name failed \n ");
        return -1;
    }
	  

	if(get_msg_cmd_id(&msg_id, &cmd_id, name) < 0)
	{
        LOG("cmu get_msg_cmd_id failed\n");
        return -1;
    }
	  

	switch(msg_id)
	{
		case MSG_REG:       
			return handle_reg_msg(cmd_id, skt, cseq, buf, len);//处理注册心跳消息
		case MSG_GET:         
			return handle_get_msg(cmd_id, skt, cseq, buf, len);//获得基本参数
		case MSG_SET:
			return handle_set_msg(cmd_id, skt, cseq, buf, len);//设置基本参数
		case MSG_OPER:
			return handle_oper_msg(cmd_id, skt, cseq, buf, len);
		case MSG_UPLOAD:
			return handle_upload_msg(cmd_id, skt, cseq, buf, len);
		default:
			break;
	}

	return 0;
}

