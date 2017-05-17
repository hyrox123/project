#include "stdafx.h"
#include "ms_pu_lib.h"


#include "ms_pu_glob.h"

#include "ms_cmu.h"
#include "ms_mdu.h"
#include "ms_pu_glob.h"

#include "ms_pu_param.h"

#include "ms_pu_glob.h"
//#include "gb_pu_debug.h"
//#include "gb_server.h"
//#include <gb_server_stream_pool.h>

#include "ms_mdu_stream.h"
#include "ms_mdu_stream_pool.h"
#include "oslib.h"
#include "ms_pu_lib.h"
#include "ms_cmu_oper.h"
long reginter_time=1377164551;


extern void* stream_pool;
extern char g_log_server[20];
extern char gb_g_log_server[20];
extern uint32_t	  g_param;

int net_initlib(net_info_t info, net_register_proc reg_proc, net_msg_proc msg_proc,net_stream_proc stream_proc, unsigned long param)
{
    #if 0
    long now_time=time(0);//授权
    if(now_time-reginter_time>60*60*24*30)
    {
       printf("Licesic is over\n");
       return 0; 
    }
    #endif
    int mdu_result = 0;
    int cmu_result = 0;
    signal( SIGPIPE, SIG_IGN);//屏蔽管道破裂信号
    select_param = info.select_platform;//释放资源的时候平台选择
    
    if(MINGSOFT_PLATFORM == info.select_platform)
    {
       #ifdef MINGSOFT_LIB
       memset(&ms_g_info, 0, sizeof(net_info_t));
       memcpy(&ms_g_info, &info, sizeof(net_info_t));
       strncpy(g_log_server,ms_g_info.ms_info.ip,sizeof(ms_g_info.ms_info.ip));
       //printf("g_log_server%s\n",g_log_server);
       //printf("ms_g_info.ms_info.ip%s\n",ms_g_info.ms_info.ip);
       LOG("+++++++++++++++++++++++++++++++++++++++++++++++++++mingsoft start dividing line+++++++++++++++++++++++++++++++++++++++++++++++++++\n");
       printf("mingsoft Platform is Startting\n");
       if(1 == ms_platform_status)
        {
            LOG("[LOG.%s.%d]MINGSOFT_PLATFORM Duplicate registration\n",__FILE__, __LINE__);
            printf("ms_platform_status already register\n");
            net_error_num = PU_DUPLICATE_REGISTER;//重复注册
            return -1;
        }
        ms_platform_status = 1;
        
        g_reg_proc	    = reg_proc;
        g_msg_proc		= msg_proc;	
        g_stream_proc	= stream_proc;
        g_param			= param;    
        thrd_pool = os_thread_pool_create(50);//创建线程池
       
        
        //LOG("cmu initial start\n");

        cmu_result = ms_cmu_initial(ms_g_info.ms_info.ip, ms_g_info.ms_info.port);//cmu初始化
        if(cmu_result != 0)
        {
            LOG("[net_initlib]cmu initial failed\n");
            printf("cmu init %d\n",cmu_result);
            return -1;
        }
        
        mdu_result = ms_mdu_initial();//mdu初始化
        if(mdu_result != 0)
        {
            LOG("[net_initlib]mdu initial failed\n");
            printf("mdu_initial failed\n");
            //return -1;
        }
       
       #endif
    }

    else if(GB28181_PLATFORM == info.select_platform)
    {
        //return 0;
        #if 0
        long now_time=time(0);//授权
        if(now_time-reginter_time>60*60*24*30)
        {
           printf("Licesic is over++++++++++++\n");
           return 0; 
        }
        #endif
        #ifdef GB28181_LIB
        pu_initiative_cancellation = 0;
        memset(&gb_g_info, 0, sizeof(net_info_t));
        memcpy(&gb_g_info, &info, sizeof(net_info_t));
        strcpy(gb_g_log_server,gb_g_info.sip_net.hcIp);
        LOG("++++++++++++++++++++++++++++++++++++++++++++++++++++gb28181 start dividing line+++++++++++++++++++++++++++++++++++++++++++++++++++\n");

        printf("GB28181 Platform is Startting\n");

        if(1 == gb_platform_status)
        {
            LOG("[net_initlib]GB28181_PLATFORM Duplicate registration\n");
            printf("gb_platform_status already register\n");
            net_error_num = PU_DUPLICATE_REGISTER;//重复注册
            return -1;
        }
        gb_platform_status = 1;
       

        gb_g_reg_proc	    = reg_proc;
        gb_g_msg_proc		= msg_proc;	
        gb_g_stream_proc	= stream_proc;
        gb_g_param			= param;    
        gb_thrd_pool = os_thread_pool_create(50);//创建线程池


        
    	gb_system_initial();//国标初始化
        
        #endif
    }
    else
    {
        printf("select_platform error\n");
    }
    
    return 0;
}

int net_fililib(int select_platform)
{
   
   if(select_platform==MINGSOFT_PLATFORM)
   {
     #ifdef MINGSOFT_LIB
        //printf("Ming Soft Reasele  begin\n");
        ms_cmu_release();

        ms_mdu_release();
        if(NULL != stop_mutex)
            os_mutex_destroy(stop_mutex);
        //printf("Ming Soft Reasele  end \n");
        if(thrd_pool)
        {

            os_thread_pool_destroy( thrd_pool, -1);

            thrd_pool = NULL;
        }
        ms_platform_status = 0;
        LOG("+++++++++++++++++++++++++++++++++++++++++++++++mingsoft end dividing line++++++++++++++++++++++++++++++++++++++++++++++++++\n");

     #endif
   }
    if(select_platform==GB28181_PLATFORM)
   {

        #ifdef GB28181_LIB
        //pu_initiative_cancellation = 1;
         printf("[gb_system_release]\n");
        gb_system_release();
       
        if(gb_stream_pool != NULL)
	    {
		    server_stream_pool_destroy(gb_stream_pool, 1000);
            gb_stream_pool = NULL;
	    }
        if(gb_thrd_pool)
        {
            os_thread_pool_destroy( gb_thrd_pool, -1);

            gb_thrd_pool = NULL;
        }
        gb_platform_status = 0;
        printf("gb_platform_status \n");
        LOG("++++++++++++++++++++++++++++++++++++++++++++++++gb28181 end dividing line++++++++++++++++++++++++++++++++++++++++++++++++++\n");

        #endif
   }
    os_sys_exit();
    return 0;
}



int net_send_alarm(char* cmd, char* buf, unsigned long size)
{
    //printf("handle_upload_alarm\n");
    LPNET_CMU_UPLOAD_ALARM	prt_param = (LPNET_CMU_UPLOAD_ALARM)buf; 
    int* ms_is_reg = &cmu_info->is_reg;
    int* gb_is_reg = &gb_cmu_info->is_reg;
    int ret = -1;
  
    if(0 == strcmp(cmd, UPLOAD_PU_RAISEALARM) ) // 告警上报
    {
        if(prt_param->select_platform == MINGSOFT_PLATFORM)
        {
        
            if(*ms_is_reg == REGISTER_ON)
            {
                ret = handle_upload_alarm(buf,size);
                printf("handle_upload_alarm\n");
                if( ret != 0)
                return ret;

            }
            else
                return ret;
        }
        else if(prt_param->select_platform == GB28181_PLATFORM)
        {
            if(*gb_is_reg == REGISTER_ON)
            {
                //ret = handle_upload_gb_alarm(buf,size);
                //printf("handle_upload_alarm\n");
                if( ret != 0)
                return ret;

            }
            else
                return ret;  
        }
    }
    else if(0 == strcmp(cmd, UPLOAD_PU_RAISEDIGITAL) ) // 数字量上报
    {
        return 0;
    }
    return 0;
}

int net_send_stream(unsigned long id, char* buf, unsigned long size,int frame_type)
{
    #if 1
    int iRet = -1;
    if (id != 0)
    {
        void* stream = (void*)id;
        if(mdu_stream_is_running(stream) == 1)
        {
            iRet = mdu_stream_post_data(stream,buf,size,frame_type);
            return iRet;	
        }
    }
    return iRet;
    #endif
    //printf("push++++++++++++++++++++++++++\n");
    return 0;
}
//frame_type  1: I帧  0:非I帧  2:音频
int net_send_audio_stream(unsigned long id, char* buf, unsigned long size,int frame_type)
{
    int iRet = -1;
    if (id != 0)
    {
        void* stream = (void*)id;
        if(mdu_stream_is_running(stream) == 1)
        {
            iRet = mdu_stream_post_data(stream,buf,size,frame_type);
            return iRet;	
        }
    }
    return iRet;
}
int net_stop_stream(unsigned long id)
{
    #if 1
    int iRet = -1;
    if (id != 0)
    {
        
        void* stream = (void*)id;
        if(mdu_stream_is_running(stream) == 1)
        {
            if(select_param ==MINGSOFT_PLATFORM )
            {
               #ifdef MINGSOFT_LIB
               mdu_stream_pool_try_close(stream_pool,stream, 1000);
               #endif
            }
            else if(select_param ==GB28181_PLATFORM)
            {
               #ifdef  GB28181_LIB
               server_stream_pool_try_close(gb_stream_pool, stream, 1000); 
               #endif
            }
            return iRet;	
        }
    }
    return iRet;
    #endif
}
int net_receive_stream(unsigned long id, char* buf, unsigned int * size)
{
    int iRet = -1;

    if (id != 0)
    {

        void* stream = (void*)id;
        if(mdu_stream_is_running(stream) == 1)
        {

            iRet  = mdu_stream_receive_data(stream, buf, (size_t*)size);

            return iRet;
        }
    }
    return iRet;
}

int net_init_info(void *info)
{
    char line[100];
    FILE* file;
     net_info_t *s_info = (net_info_t *)info;
    file=fopen("./ms_pusdk.conf","r");
    while(fgets(line,100,file))
    {
        line[strlen(line)-1]='\0';//add a flag of string end
        if(!memcmp("pu_id=",line,strlen("pu_id=")))//if find username
        {
            memcpy((*s_info).ms_info.pu_id,(line+sizeof("pu_id")),sizeof((*s_info).ms_info.pu_id));
            //printf("++++++++++++++++++++%s\n",(*s_info).ms_info.pu_id);
            continue;
        }
        if(!memcmp("name=",line,strlen("name=")))
        {
            memcpy((*s_info).ms_info.name,(line+sizeof("name")),sizeof((*s_info).ms_info.name));
            //printf("********\n");
            continue;
        }
        if(!memcmp("passwd=",line,strlen("passwd=")))
        {
            memcpy((*s_info).ms_info.passwd,(unsigned char *)(line+sizeof("passwd")),sizeof((*s_info).ms_info.passwd));
            continue;
        }
        if(!memcmp("ip=",line,strlen("ip=")))
        {
            memcpy((*s_info).ms_info.ip,(unsigned char *)(line+sizeof("ip")),sizeof((*s_info).ms_info.ip));
            continue;
        }
        if(!memcmp("port=",line,strlen("port=")))
        {
            (*s_info).ms_info.port = (unsigned int)atoi((const char *)(line+sizeof("port")));
            continue;
        }
        if(!memcmp("local_ip=",line,strlen("local_ip=")))
        {
            memcpy((*s_info).ms_info.local_ip,(unsigned char *)(line+sizeof("local_ip")),sizeof((*s_info).ms_info.local_ip));
            continue;
        }

        if(!memcmp("pu_type=",line,strlen("pu_type=")))
        {
            (*s_info).ms_info.pu_type= (unsigned int)atoi((const char *)(line+sizeof("pu_type")));
            continue;
        }
        if(!memcmp("company=",line,strlen("company=")))
        {
            (*s_info).ms_info.company= (unsigned int)atoi((const char *)(line+sizeof("company")));
            break;
        }
        
    }
    fclose(file);
    file = NULL;
    return 0;
}

int net_gb_init_info(void *info)
{
    char line[200];
    FILE* file;
    net_info_t *s_info = (net_info_t *)info;
    file=fopen("./gb_pusdk.conf","r");
    while(fgets(line,200,file))
    {
        line[strlen(line)-1]='\0';//add a flag of string end
        if(!memcmp("hcId=",line,strlen("hcId=")))//if find username
        {
            memcpy((*s_info).sip_net.hcId,(line+sizeof("hcId")),sizeof((*s_info).sip_net.hcId));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.hcId);
            continue;
        }
        if(!memcmp("hcIp=",line,strlen("hcIp=")))
        {
            memcpy((*s_info).sip_net.hcIp,(line+sizeof("hcIp")),sizeof((*s_info).sip_net.hcIp));
            //printf("********\n");
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.hcIp);
            continue;
        }
        if(!memcmp("hcName=",line,strlen("hcName=")))
        {
            memcpy((*s_info).sip_net.hcName,(unsigned char *)(line+sizeof("hcName")),sizeof((*s_info).sip_net.hcName));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.hcName);
            continue;
        }
        if(!memcmp("hcPwd=",line,strlen("hcPwd=")))
        {
            memcpy((*s_info).sip_net.hcPwd,(unsigned char *)(line+sizeof("hcPwd")),sizeof((*s_info).sip_net.hcPwd));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.hcPwd);
            continue;
        }
        if(!memcmp("hcPort=",line,strlen("hcPort=")))
        {
            (*s_info).sip_net.hcPort = (unsigned int)atoi((const char *)(line+sizeof("hcPort")));
            printf("++++++++++++++++++++%d\n",(*s_info).sip_net.hcPort);
            continue;
        }

        if(!memcmp("lcId=",line,strlen("lcId=")))
        {
            memcpy((*s_info).sip_net.lcId,(unsigned char *)(line+sizeof("lcId")),sizeof((*s_info).sip_net.lcId));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.lcId);
            continue;
        }
        if(!memcmp("lcIp=",line,strlen("lcIp=")))
        {
            memcpy((*s_info).sip_net.lcIp,(unsigned char *)(line+sizeof("lcIp")),sizeof((*s_info).sip_net.lcIp));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.lcIp);
            continue;
        }
        if(!memcmp("lcName=",line,strlen("lcName=")))
        {
            memcpy((*s_info).sip_net.lcName,(unsigned char *)(line+sizeof("lcName")),sizeof((*s_info).sip_net.lcName));
            printf("++++++++++++++++++++%s\n",(*s_info).sip_net.lcName);
            break;
        }
    }
    fclose(file);
    file = NULL;
    return 0;

}

int net_last_error(void)
{
	return net_error_num;
}



