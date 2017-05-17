#include "arm.h"
#include "ms_pu_lib.h"
#include "ms_pu_param.h"
#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>

CHILD_PROCESS_INFO_S alarm_ChildInfo[8];

char local_ip[20];
int fill_channel_info(void *info_t)
{
    int number =0;
    DEVICE_CATALOG_INFO *info = (DEVICE_CATALOG_INFO *)info_t;
	sprintf(info->deviceid,"3401000000131000000%d",number);
	number++;
	strcpy(info->name,"Camera 1");
	strcpy(info->manufacturer,"Abell");
	strcpy(info->model,"Abell");
	strcpy(info->owner,"Abell");

	strcpy(info->civilcode,"Abell");

	strcpy(info->address,"Abell");

	info->parental=0;

	info->registerway=1;

	info->secrecy=0;
	strcpy(info->status,"Status 1");
    return 0;

}
int main(int argc, char *argv[])
{
	//daemon(1,1);
	printf("****************please input***************\n0 :MingSoft \n1 :GB28181\n");
	printf("*******************************************\n");
    
	struct rlimit r;
    r.rlim_cur = -1;
    r.rlim_max = -1;     
    if (setrlimit(RLIMIT_CORE,&r)<0)  //�ó������ʱ�����һ��core�ļ������ڷ��� 
    {
    	fprintf(stderr,"setrlimit error\n");
    	exit(1);
    } 
    
    net_info_t info;
	//char cmd[256], *p, c;
	//int len;
    int result = 0;
	//unsigned long now, sec, minute, hour;			
	memset(&info,0,sizeof(net_info_t));
    #if 1
    //info.select_platform = GB28181_PLATFORM;//����ƽ̨ע��
    if(GB28181_PLATFORM == info.select_platform)
    {
        /*�����¼��gb2818ƽ̨����Ϣ,����ID,����,�û���,ip��*/
        
        net_gb_init_info(&info);//�������ļ���ȡ��Ϣ�����info
        
		info.sip_net.channel_num=8;
		info.sip_net.alarm_num=1;

		int i=0;
		//����ͨ����Ϣ
		info.sip_net.channel_info=(DEVICE_CATALOG_INFO **)malloc(info.sip_net.channel_num*sizeof(char *));
		while(i<info.sip_net.channel_num)
		{
			info.sip_net.channel_info[i]=(DEVICE_CATALOG_INFO *)malloc(sizeof(DEVICE_CATALOG_INFO ));
			fill_channel_info((void *)info.sip_net.channel_info[i]);
			i++;
		}

		i = 0;
		info.sip_net.alarmstatus_info=(DEVICE_ARLARMSTATUS_INFO **)malloc(info.sip_net.alarm_num*sizeof(char*));

		//����澯ͨ����Ϣ
		while(i<info.sip_net.alarm_num)
		{
			info.sip_net.alarmstatus_info[i] = (DEVICE_ARLARMSTATUS_INFO *)malloc(sizeof(DEVICE_ARLARMSTATUS_INFO));
			memset(info.sip_net.alarmstatus_info[i], 0,sizeof(DEVICE_ARLARMSTATUS_INFO));
			if (i == 0)
			{
				sprintf(info.sip_net.alarmstatus_info[i]->deviceid, "34010000001340000001");
				sprintf(info.sip_net.alarmstatus_info[i]->dutystatus, "OFFDUTY");
			}
			i++;
		}

		info.sip_net.lcPort = 15062;
		sprintf(info.sip_net.lcPwd, "123456");
		info.sip_net.expires = 3600;
		info.sip_net.keeplive= 15;//����ʱ��
		info.sip_net.keeplivenum= 3;//����ʧ�ܺ�,���Դ���

		//����ص�����, on_login,on_msg,on_stream
		#if 1//60��ע����ѹ������
		while(1)
		{
            result = net_initlib(info,NULL,on_msg,on_stream,0);//��ʼ��SDK 
            if(0 != result  )
		    {
			    printf("error num:%d\n",net_last_error());//��ӡ������
		    }
            sleep(120);
          
            net_fililib(GB28181_PLATFORM);//��Դ�ͷ�
            sleep(120);
            #if 0
            free(info.sip_net.channel_info);
            while(i<info.sip_net.channel_num)
	        {
		        free(info.sip_net.channel_info[i]);
		        i++;
	        }
            free(info.sip_net.alarmstatus_info);
            while(i<info.sip_net.alarm_num)
            {
                free(info.sip_net.alarmstatus_info[i]);
                i++;
            }
            sleep(10);
            #endif
        }
        #endif 
        
        #if 0
		result = net_initlib(info,NULL,on_msg,on_stream,1);//��ʼ��SDK
		if(0 != result  )
		{
			printf("error num:%d\n",net_last_error());//��ӡ������
		}
        #endif
    }
    #endif
  
    info.select_platform = MINGSOFT_PLATFORM;//����ƽ̨ע��
	if(MINGSOFT_PLATFORM == info.select_platform)
	{
		net_init_info(&info);//�������ļ���ȡ��Ϣ�����info
		strncpy(local_ip,info.ms_info.local_ip,sizeof(local_ip));
		
        #if 0//120��ע����ѹ������
        while(1)
		{
            result = net_initlib(info,NULL,on_msg,on_stream,0);//��ʼ��SDK 
            if(0 != result  )
		    {
			printf("error num:%d\n",net_last_error());//��ӡ������
		    }
            sleep(120);
            net_fililib(MINGSOFT_PLATFORM);//��Դ�ͷ�
            sleep(10);
        }
        #endif
       
	
        #if 1
		result = net_initlib(info,NULL,on_msg,on_stream,0);//��ʼ��SDK
		if(0 != result  )
		{
			printf("error num:%d\n",net_last_error());//��ӡ������
		}
        #endif
    }

	int input = 2;//�澯����
	while( 1 )
	{	
		 //printf("Please Input Alarm\n");
		 //scanf("%d",&input);
		 //alarm_proc(info,input,MINGSOFT_PLATFORM);
		 sleep(5);
	}
  
	net_fililib(MINGSOFT_PLATFORM);//��Դ�ͷ�
	return 0;
}


