#include <pthread.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 


#include "ms_pu_lib.h"
#include "ms_pu_param.h" 
#include "time.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "arm.h" 


NET_STREAM_OPEN_PARAM realtime_param[100];
NET_STREAM_OPEN_PARAM playback_param[100];
NET_STREAM_OPEN_PARAM download_param[100];


//#include <ms_pu_stream.h>

//g_sleep_time�ǿ�ţ����ŵ�ʱ�򣬿��Ʒ������ʵģ�demo�м򵥵�ʵ��
int g_sleep_time=1000000/25;

//��������Demo
void *stream_proc(void* param); 
//�澯����demo
int alarm_proc(net_info_t info,int t,int select); 

//int send_data(char *buffer,int size,unsigned long param);


int on_login( int type, char* ip, unsigned int port, unsigned long error, unsigned long times, unsigned long param  ) 
{ 
	return 0; 
} 

int on_msg( char* cmd, char* buf, unsigned long* size, unsigned long param )//��Ϣ����,����size,param��ʱδ�� 
{
    //printf("on_msg cmd ++++++++++++%s\n",cmd);
	//�����豸ʱ����� 
	if(strcmp(cmd,SET_PU_TIME) == 0) 
	{ 
		//LPNET_CMU_DEV_TIME basic = (LPNET_CMU_DEV_TIME)buf; 
		/* 
		   typedef struct										 
		   { 
		   int hour;		// ʱ (0~23) 
		   int minute;		// �� (0~59) 
		   int second;		// �� (0~59) 
		   int date;		// �� (1~31) 
		   int month;		// �� (1~12) 
		   int year;		// �� nnnn 
		   }NET_CMU_DEV_TIME, *LPNET_CMU_DEV_TIME; 
		   */ 

		/*	printf("hour is %d\n",basic->hour); 
			printf("minute is %d\n",basic->minute); 
			printf("second is %d\n",basic->second); 
			printf("date is %d\n",basic->date); 
			printf("month is %d\n",basic->month); 
			printf("year is %d\n", basic->year); 
			*/
		printf("SET_PU_TIME %s ++++++\n",SET_PU_TIME);
	}

	//��ȡ�豸�汾
	else if(strcmp(cmd, GET_PU_SOFTWAREVERSION) == 0)
	{
		LPNET_CMU_SOFT_VERSION basic = (LPNET_CMU_SOFT_VERSION)buf;
		strncpy(basic->pu_id,"10002",32);
		strncpy(basic->version,"10.1.1",32);
		strncpy(basic->build_time,"2001-03-23 23:34:34",32);
	}

	//�����豸������� 
	else if(strcmp(cmd,SET_PU_IPINFO) == 0)	 
	{ 
		LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf; 
		/* 
		   typedef struct										 
		   {		 
		   int   net_id;				// ���ڱ�� (1,2,3...)  
		   char  ip_addr[15];			// IP��ַ 
		   int   ctrl_port;			    // ҵ����ƶ˿ں� 
		   char  net_mask[15];			// �������� 
		   char  dns_addr[2][15];		// DNS��������ַ,�����ж�� 
		   char  gate_way[15];			// ���ص�ַ 
		   }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO; 
		   */ 
		printf("netId is %d\n",basic->net_id); 
		printf("ip_addr is %s\n",basic->ip_addr); 
		printf("ctrl_port is %d\n",basic->ctrl_port); 
		printf("net_mask is %s\n",basic->net_mask); 
		printf("dns_addr is %s\n",basic->ip_addr); 
		printf("gate_way is %s\n",basic->gate_way); 
	} 

	//��ȡ�豸������� 
	else if(strcmp(cmd,GET_PU_IPINFO)== 0) 
	{ 
		LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf; 
		/* 
		   typedef struct										 
		   {		 
		   int   net_id;	            // ���ڱ�� (1,2,3...)  
		   char  ip_addr[15];           // IP��ַ 
		   int   ctrl_port;		        // ҵ����ƶ˿ں� 
		   char  net_mask[15];	        // �������� 
		   char  dns_addr[2][15];		// DNS��������ַ,�����ж�� 
		   char  gate_way[15];			// ���ص�ַ 
		   }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO; 
		   */ 

		basic->net_id = 2; 
		strncpy(basic->ip_addr, "192.168.2.72", 15); 
		basic->ctrl_port = 1001; 
		strncpy(basic->net_mask,"255.255.255.0",15); 
		strncpy(basic->dns_addr[0], "192.168.1.1", 15); 
		strncpy(basic->gate_way, "192.168.1.1", 15); 

	} 

	//������̨ 
	else if(strcmp(cmd, OPER_PU_CONTROLPTZ) == 0) 
	{ 
		LPNET_CMU_OPER_CTRL_PTZ basic = (LPNET_CMU_OPER_CTRL_PTZ)buf; 
		/* 
		   typedef struct								 
		   { 
		   int  ptz_id;	��̨��ʶ 

		   char cmd[32];							 
		   ��������/��Զ��ZIN/ZOUT�� 
		   �������/��Զ��FN/FR�� 
		   ��Ȧ�Զ�����/����/��С��IA_ON/IO/IC�� 
		   ����������WP_ON�� 
		   �ƹ⿪����LP_ON 
		   Ѳ��������AUTO_START/AUTO_STOP�� 
		   ��������� ��TU,��TD,��PL,��PR, ����TUPR , ����TUPL, ����TDPR, ����TDPL,  
		   ת��Ԥ��㣺GOTO_PRESET 
		   �켣Ѳ���� 
		   ��ʼѲ����START_TRACK_CRUISE 
		   ֹͣ��STOP  

		   char param[200];						 
		   ��cmd��GOTO_PRESETʱ���ò�����ʾԤ�����š� 
		   ��cmd��START_TRACK_CRUISE�ò�����Ѳ���켣������ 
		   ��cmd��STOPʱ���ò���ָ��Ҫֹͣ�Ŀ��������������̨�˶������ 

		   int  speed;// ��̨�ٶ�	1-8����8���ȼ�����ֵԽ���ٶ�Խ�졣		 

		   }NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ; 
		   */ 

		printf("ptz_id is%d \n",basic->ptz_id);//��̨��ʶ  
		printf("cmd is%s\n",basic->cmd); 
		printf("param is%s\n",basic->param); 
		printf("seep is%d\n",basic->speed);//��̨�ٶ�	 
	}


	//������̨Ԥ�õ�
	else if(strcmp(cmd,SET_PU_PRESETPTZ) == 0)
	{

		LPNET_CMU_PRESET_LIST basic = (LPNET_CMU_PRESET_LIST)buf;
		/*
		   typedef struct
		   {
		   int  preset_index;					// Ԥ�õ���
		   char preset_name[16];				// Ԥ�õ�����
		   }NET_CMU_PRESET, *LPNET_CMU_PRESET;

		   typedef struct						// �豸��̨Ԥ�õ�
		   {
		   int ptz_id;							// ��̨��ʶ
		   int preset_list_cnt;					// 
		   NET_CMU_PRESET *preset_lists;		 
		   }NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;
		   */
		printf("basic->ptz_id %d\n",basic->ptz_id);//��̨��ʶ
		printf("basic->preset_lists->preset_index%d\n",basic->preset_lists->preset_index);//Ԥ�õ�����
		printf("basic->preset_lists->preset_name%s\n",basic->preset_lists->preset_name);//Ԥ�õ�����

	}
    //ɾ����̨Ԥ�õ�
    else if(strcmp(cmd,OPER_PU_DELPRESETPTZ ) == 0)
    {
       // LPNET_CMU_OPER_DEL_PRESET basic = (LPNET_CMU_OPER_DEL_PRESET)buf;
        
    }
	//��ѯ¼���ļ�
	else if(strcmp(cmd, OPER_PU_QUERYPULOCALSTORAGEFILES) == 0)
	{
		LPNET_CMU_QUERY_STORE_FILE basic = (LPNET_CMU_QUERY_STORE_FILE)buf; 
		int cnt = 10;
		int num = 0;
		int i = 1;
		char fileName[64]={0};
		//char startTime[20]={0};
		//char endTime[20]={0};

		printf("video_id is %d \n",basic->video_id);//��ѯ�ļ�����Ƶͨ��
		printf("file_type is %d\n",basic->file_type);//��ѯ�ļ�������
		printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					basic->from_date.year,
					basic->from_date.month,
					basic->from_date.date,
					basic->from_date.hour,
					basic->from_date.minute,
					basic->from_date.second);//��ʼ�����ڼ�ʱ��

		printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					basic->to_date.year,
					basic->to_date.month,
					basic->to_date.date,
					basic->to_date.hour,
					basic->to_date.minute,
					basic->to_date.second);//��ʼ�����ڼ�ʱ��

		basic->record_file_lists = (LPNET_CMU_RECORD_FILE)malloc(cnt *sizeof(NET_CMU_RECORD_FILE));
        printf("+_++++++++++++++++++++++++++++++++++++++++++++%d  %d\n",cnt *sizeof(NET_CMU_RECORD_FILE),sizeof(NET_CMU_QUERY_STORE_FILE));
        //basic->record_file_size = 4096;
		basic->file_list_cnt = cnt;

		basic->total_size = 1000;//DISK ������С
		basic->free_size = 1000;//DISKʣ������
		basic->total_num = 100;//�ܹ��ļ�����
		basic->leave_num = 20;//ʣ���ļ�����
		basic->result_code = 0;//ע���� ��code��: 0��OK  1����֧�ֵ��ļ����� 48���ͻ��˴���	49������������ 50������ʧ��ԭ��	

		for(;num < cnt; num++,i++){
			sprintf(fileName,"../store-%d.txt",i);
			memset( basic->record_file_lists[num].file_name,0,sizeof(basic->record_file_lists[num].file_name));
			sprintf(basic->record_file_lists[num].file_name,"../store-%d.txt",i);
			basic->record_file_lists[num].start_time.year=2013;
			basic->record_file_lists[num].start_time.month=3;
			basic->record_file_lists[num].start_time.date=1;
			basic->record_file_lists[num].start_time.hour=15;
			basic->record_file_lists[num].start_time.minute=45;
			basic->record_file_lists[num].start_time.second=12;


			basic->record_file_lists[num].stop_time.year=2013;
			basic->record_file_lists[num].stop_time.month=3;
			basic->record_file_lists[num].stop_time.date=1;
			basic->record_file_lists[num].stop_time.hour=18;
			basic->record_file_lists[num].stop_time.minute=45;
			basic->record_file_lists[num].stop_time.second=12;
			//  basic->record_file_lists[num].size=100;

		}

	}
	//�豸Ŀ¼��ѯ
	else if(strcmp(cmd,OPER_PU_RECORD) == 0)
	{
		LPNET_CMU_OPER_RECORD_DEV  record_dev=(LPNET_CMU_OPER_RECORD_DEV)buf;
		printf("ResetAlarm pu_id:%s,cmd:%d\n",record_dev->pu_id,record_dev->control_action);
	}
	//״̬��ѯ
	else if(strcmp(cmd,OPER_PU_STATUS)==0)
	{
		//ǰ��״̬��ѯ��
		DEVICE_ARLARMSTATUS_INFO **alarm_status=(DEVICE_ARLARMSTATUS_INFO**)buf;
		int i=0;
		//�˴����ݸ澯�豸��ID�����澯�豸��״̬��
		while( i < *size)
		{
			printf("DEVICE_ARLARMSTATUS_INFO:%s %d\n",alarm_status[i]->deviceid,*size);
			strcpy(alarm_status[i]->dutystatus,"ONDUTY");
			i++;
		}
	}

	// ����/����
	else if(strcmp(cmd,OPER_PU_GUARD) == 0) 
	{
		//NET_CMU_OPER_GUARD_DEV guard_dev =(LPNET_CMU_OPER_GUARD_DEV)buf;
	}
	// �豸Զ������
	else if(strcmp(cmd, OPER_PU_CONTROLPU) == 0) 
	{
		LPNET_CMU_OPER_CTRL_DEV basic = (LPNET_CMU_OPER_CTRL_DEV)buf;
		printf("LPNET_CMU_OPER_CTRL_DEV %d\n",basic->control_action);//basic->control_action==0����==1�ػ�==2֪ͨ������==3֪ͨ������
	}
    //ʱ���ı�����
	else if(strcmp(cmd, SET_PU_IMAGETEXTPARA) == 0)
    {
    	LPNET_CMU_IMG_TEXT basic = (LPNET_CMU_IMG_TEXT)buf;
    	printf("video_id is %d\n",basic->video_id);
    	printf("time_enable is %s\n",basic->time_enable);
    	printf("time_X is %d\n",basic->time_X);
    	printf("time_Y is %d\n",basic->time_Y);
    	printf("text_enable is %s\n",basic->text_enable);
    	printf("text is %s\n",basic->text);
    	printf("text_X is %d\n",basic->text_X);
    	printf("text_Y is %d\n",basic->text_Y);
	
    }
    //�豸ͼ��������
	else if(strcmp(cmd, SET_PU_IMAGEENCODEPARA) == 0)
    {
    	LPNET_CMU_IMG_ENCODE basic = (LPNET_CMU_IMG_ENCODE)buf;
    	printf("video_id is %d\n",basic->video_id);
    	printf("encode_mode is %s\n",basic->encode_mode);
    	printf("pic_quality is %d\n",basic->pic_quality);
    	printf("bit_rate is %d\n",basic->bit_rate);
    	printf("bit_rate_type is %d\n",basic->bit_rate_type);
    	printf("frame_rate is %d\n",basic->frame_rate);
    	printf("image_size is %s\n",basic->image_size);
    	printf("stream_type is %d\n",basic->stream_type);
    	printf("iframe_interval is %d",basic->iframe_interval);

    }
    //���ڲ���
	else if(strcmp(cmd,SET_PU_SERIALPORT) == 0)
    {

    	LPNET_CMU_SERIAL_PORT basic = (LPNET_CMU_SERIAL_PORT)buf;
    	printf("for_decoder is %s\n",basic->for_decoder);
    	printf("serial_port is %d\n",basic->serial_port);
    	printf("video_id is %d\n",basic->video_id);
    	printf("baud_rate is %d\n",basic->baud_rate);
    	printf("data_bit is %d\n",basic->data_bit);
    	printf("parity is %d\n",basic->parity);
    	printf("stop_bit is %d\n",basic->stop_bit);
    	printf("mode is %s\n",basic->mode);
    	printf("time_out is %d\n",basic->time_out);
    	printf("flow_control is %d\n",basic->flow_control);
    	printf("decoder_type is %d\n",basic->decoder_type);
    	printf("decoder_address is %d\n",basic->decoder_address);
    	printf("work_mode is %d\n",basic->work_mode);
    }
    //ͼ����ʾ����
    else if(strcmp(cmd, SET_PU_IMAGEDISPLAYPARA) == 0)
    {
	
    	LPNET_CMU_IMG_DISPLAY basic = (LPNET_CMU_IMG_DISPLAY)buf;
    	printf("video_id is %d\n",basic->video_id);
    	printf("contrast is %d\n",basic->contrast);
    	printf("bright is %d\n",basic->bright);
    	printf("hue is %d\n",basic->hue);
    	printf("saturation is %d\n",basic->saturation);

    }
    //�豸ͼ����ʽ����
    else if(strcmp(cmd, SET_PU_VIDEOFORMAT) == 0)
    {

		LPNET_CMU_VIDEO_FORMAT basic = (LPNET_CMU_VIDEO_FORMAT)buf;
		printf("video_id is %d\n",basic->video_id);
		printf("video_format is %d\n",basic->video_format);

	}
    // �豸��Ƶ�ڱθ澯����
    else if(strcmp(cmd,SET_PU_HIDEDETECTION) == 0)
    {
	
		LPNET_CMU_HIDE_DETECT basic = (LPNET_CMU_HIDE_DETECT)buf;
	
	
		printf("video_id is %d\n",basic->video_id);
		printf("alarm_dnabled is %s\n",basic->alarm_enabled);
		printf("alarm_time is %s\n",basic->alarm_time);
		printf("sensitivity  is %d\n",basic->sensitivity);

		printf("hide_coord.x %d\n",basic->hide_coord.x);
		printf("hide_coord.y %d\n",basic->hide_coord.y);
		printf("hide_coord.width %d\n",basic->hide_coord.width);
		printf("hide_coord.height %d\n",basic->hide_coord.height);
        
		printf("alarm_output alarm_channel %d\n",basic->hide_alarm_output.alarm_channel);
		printf("alarm_output alarm_time %d\n",basic->hide_alarm_output.alarm_time);

		printf("hide_alarm_viod.alarm_record_channel  is %d\n",basic->hide_alarm_video.alarm_record_channel);
		printf("hide_alarm_viod.alam_short_channel is  %d\n",basic->hide_alarm_video.alarm_shot_channel);
	
	}
    //�ƶ����澯��������
    #if 0
    else if(strcmp(cmd,SET_PU_MOTIONDETECTION) == 0)
    {
	
		LPNET_CMU_MOTION_DETECT basic = (LPNET_CMU_MOTION_DETECT)buf;

		int cnt=0;
		int num =0;
		int result = 0;
	

		cnt = basic->block_list_cnt;
	
	
		printf("video_id is %d\n",basic->video_id);
		printf("alarm_dnabled is %s\n",basic->alarm_enabled);
		printf("alarm_time is %s\n",basic->alarm_time);
		printf("frequeny is %d\n",basic->frequency);
		printf("sensitivity is %d\n",basic->sensitivity);

		for(;num < cnt;num++){

			printf( "block_list[%d].x is %d\n",num,basic->block_lists[num].x);
			printf( "block_list[%d].y is %d\n",num,basic->block_lists[num].y);
			printf( "block_list[%d].height is %d\n",num,basic->block_lists[num].height);
			printf( "block_list[%d].width  is %d\n",num,basic->block_lists[num].width);

		}

		printf( "motion_alarm.channel is %d\n",basic->motion_alarm_output.alarm_channel);
		printf( "motion_alarm is time is %d\n",basic->motion_alarm_output.alarm_time);


		printf( "motion_video.channel is %d\n",basic->motion_alarm_video.alarm_record_channel);
		printf( "motion_video short_channle is %d\n",basic->motion_alarm_video.alarm_shot_channel);

		//�����ƶ����
		if(!(strcmp(basic->alarm_enabled,"true")) && (alarm_ChildInfo[CAM_MOTIONDETECT].child_status!=R_Worked))
        {

			char tmp[64] = {0};
			int video_id = 1;
			int err = 0;

			sprintf(tmp, "alarm_%d",video_id);
			alarm_ChildInfo[CAM_MOTIONDETECT].alarm_type = CAM_MOTIONDETECT;
			
			printf("\n--------start Motionndetect proc-----------\n");

			alarm_ChildInfo[CAM_MOTIONDETECT].child_pid= os_thread_create(tmp,100,65535,alarm_proc,&alarm_ChildInfo[CAM_MOTIONDETECT]);
			os_thread_resume(alarm_ChildInfo[CAM_MOTIONDETECT].child_pid);

			if( alarm_ChildInfo[CAM_MOTIONDETECT].child_pid == 0){
				
				alarm_ChildInfo[CAM_MOTIONDETECT].child_status = R_Error;
				result = -1;
			}
		}
		if(!(strcmp(basic->alarm_enabled,"false")) &&  (alarm_ChildInfo[CAM_MOTIONDETECT].child_status ==R_Worked))
        {
			if (alarm_ChildInfo[CAM_MOTIONDETECT].child_pid)
			{
				printf("\n--------close   Motionndetect proc-----------\n");
				os_thread_destroy(alarm_ChildInfo[CAM_MOTIONDETECT].child_pid, 20);
			}
			alarm_ChildInfo[CAM_MOTIONDETECT].child_pid = NULL;
			alarm_ChildInfo[CAM_MOTIONDETECT].child_status = R_NOCREATE;
			alarm_ChildInfo[CAM_MOTIONDETECT].alarm_type = -1;
		}	
    }	
    #endif
	return 0; 
}

//�澯�ϱ�
int alarm_proc(net_info_t info,int t,int select) 
{ 
	char *cmd = UPLOAD_PU_RAISEALARM;//�豸�澯�ϱ� 	

	NET_CMU_UPLOAD_ALARM  ptr_param;//�澯��Ϣ  

	int type = 0; 
	char pszDate[32]= "\0";//��ű���ʱ��Ļ�����
	char data[64];//��������ϸ��Ϣ
	struct tm *ptm; 
	time_t TimeFile; 

	//Ŀǰֻʵ�� VS_GPIN���������澯��CAM_VIDEOLOSE����Ƶ��ʧ ��CAM_HIDEDETECT���ڵ��澯


	//NET_CMU_UPLOAD_ALARM �ϴ��澯�Ľṹ��
	memset(&ptr_param,0,sizeof(NET_CMU_UPLOAD_ALARM)); 

	// �������� �澯ʱ��    
	memset(&pszDate,0,sizeof(pszDate)); 
	TimeFile = time(NULL); 
	ptm = localtime(&TimeFile); 
	sprintf(pszDate,"%04d-%02d-%02d %02d:%02d:%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);//������ʱ�� 
	strncpy(ptr_param.time_stamp,pszDate,sizeof(pszDate)); 

	if(MINGSOFT_PLATFORM == select)
	{
		strncpy(ptr_param.pu_id,info.ms_info.pu_id,sizeof(info.ms_info.pu_id));
		strncpy(ptr_param.pu_ip,info.ms_info.ip,sizeof(info.ms_info.ip));
	}
	else if (GB28181_PLATFORM == select)
	{
		strncpy(ptr_param.pu_id,info.sip_net.hcId,sizeof(info.sip_net.hcId));
		strncpy(ptr_param.pu_ip,info.sip_net.hcIp,sizeof(info.sip_net.hcIp));
	}
	else
	{ 
		printf("select = %d\n",select);
		return 0;
	}
	//Ȼ������澯����
	if(t==1)
	  type = VS_GPIN;//CAM_HIDEDETECT;//Ŀǰֻʵ�� VS_GPIN���������澯��CAM_VIDEOLOSE����Ƶ��ʧ ��CAM_HIDEDETECT���ڵ��澯CAM_MOTIONDETECT�ƶ����
	else if(t == 2)
	  type  = CAM_VIDEOLOSE;
	else if(t == 3)
	  type = CAM_HIDEDETECT;
    else
      type = CAM_MOTIONDETECT;

	ptr_param.alarm_type = type;//�������� 

	//�����Ӳ�̴�������Ӳ�̺�
	ptr_param.disk_number = 0; //������Ӳ�̺�

	strcpy(data,"1");
	/*�澯����ϸ��Ϣ����һ���ַ���
	  strcpy(data,"d:70%");
	  if((strlen(data) + 1) < 64)//������ϸ��Ϣ���Ȳ��ܳ���64���ֽ�
	  strcpy(ptr_param.data,data);
	//�澯���豸����
	strncpy(ptr_param.server_type,"CAM",32);//�澯�豸���� 

	//������net_send_alarm,�ϴ��澯��Ϣ*/
	strcpy(ptr_param.eliminated,"false");//�澯����
	ptr_param.data.integerValue=1;
    ptr_param.select_platform = MINGSOFT_PLATFORM;
	net_send_alarm(cmd,(char *)&ptr_param,sizeof(NET_CMU_UPLOAD_ALARM));//�澯�ϱ� 
	printf("net_send_alarm%d %s\n",ptr_param.alarm_type,ptr_param.eliminated);
    printf(" %s %s \n",ptr_param.pu_id,ptr_param.pu_ip);
	return 0; 
} 
int on_stream( unsigned long id, char* cmd, char* buf, unsigned long size, unsigned long param )
{
    int j=0;
		
	char tmp[64] = {0};
	int video_id = 0;
    //printf("*************%d\n",id);
    //printf("++++++++++++++++++++++++++++++++++++++++++||||||||||||||+++++++%d\n",id);
    printf("cmd = ++++++++++++++++++++++++%s\n",cmd);
    //sleep(1);
    while(j<100)
	{
		realtime_param[j].ptr_file=NULL;
		realtime_param[j].video_id=-1;
		playback_param[j].ptr_file=NULL;
		download_param[j].ptr_file=NULL;
		j++;
	}
	if(strcmp(cmd, "Start_MEDIA") == 0)
	{

		LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
		if(start_media->request_type==0)
		{ 
            int i=0;
            while(i<100)
			{
				if(realtime_param[i].ptr_file==NULL)
				{
                    printf("[on_stream] if\n");
					break;
				}
				i++;
			}
			video_id = start_media->media.video_id;
			strcpy(tmp,"./test.h264");
            printf("[on_stream] strcpy\n");
			realtime_param[i].ptr_file = fopen(tmp, "r+b");

			if(realtime_param[i].ptr_file == NULL)
			{
				printf("\n fopen error %d %s !\n",errno,strerror(errno));
                printf("[on_stream] ptr_file\n");
				return -1;
			}

			realtime_param[i].id   = id;		
			realtime_param[i].count =0;
			realtime_param[i].flag = 1;	
			realtime_param[i].video_id = video_id;	
			realtime_param[i].media_type = start_media->media.media_type;
			realtime_param[i].stream_type = start_media->media.stream_type;
          
            pthread_attr_t attr;
            //��������Ϊ��detached  state��
            pthread_attr_init(&attr);
            printf("[on_stream] init\n");
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    	/* ��ʼ�����߳� */
    	    if( pthread_create(&realtime_param[i].pid,&attr, stream_proc, (void *)&realtime_param[i] ) != 0 )
    	    {
                fprintf(stderr,"pthread_create %d %s \n",errno,strerror(errno));
                printf("pthread_create %d %s \n",errno,strerror(errno));
                //LOG("pthread_create %d %s \n",errno,strerror(errno));
                //LOG("thread create failed\n");
                 pthread_attr_destroy (&attr);
                return -1;
    	    }
             pthread_attr_destroy (&attr);
             printf("[on_stream] create\n");
        
		}
        
        #if 1
		else if(start_media->request_type==1)
		{
             printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					start_media->start_time.year,
					start_media->start_time.month,
					start_media->start_time.date,
					start_media->start_time.hour,
					start_media->start_time.minute,
					start_media->start_time.second);//��ʼ�����ڼ�ʱ��
			printf("Start File Play Back:%s\n",start_media->fileName);
			int i=0,err;
			while(i<100)
			{
				if(playback_param[i].ptr_file==NULL)
				{
					break;
				}
				i++;
			}
			strcpy(tmp,"./test.h264");
			playback_param[i].ptr_file = fopen(tmp, "r+b");//��ȡ������Ƶ�ļ����ļ������� 


			if(playback_param[i].ptr_file == NULL) 
			{ 
				printf("\n fopen error !\n"); 
				perror("fopen"); 
			} 

			strcpy(playback_param[i].filename,start_media->fileName); 

			playback_param[i].id = id; 		 

			playback_param[i].flag = 1;//��־λflag = 1��ȡ��Ƶ֡��flag = 0ֹͣ��ȡ��Ƶ֡ 	

			playback_param[i].video_id = start_media->media.video_id;//��Ƶͨ��	 
			playback_param[i].media_type = start_media->media.media_type;//ý������
			playback_param[i].stream_type = start_media->media.stream_type;//��������
            pthread_attr_t attr;
            //��������Ϊ��detached  state��
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    	    /* ��ʼ�����߳� */
    	    if( pthread_create((pthread_t *)&playback_param[i].pid,&attr,stream_proc,(void *)&playback_param[i]) != 0 )
    	    {
                fprintf(stderr,"pthread_create %d %s \n",errno,strerror(errno));
                //LOG("pthread_create %d %s \n",errno,strerror(errno));
                //LOG("thread create failed\n");
    	    }
            pthread_attr_destroy (&attr);

		}
		else if(start_media->request_type==2)
		{

			int i=0,err;
			while(i<100)
			{
				if(download_param[i].ptr_file==NULL)
				{
					break;
				}
				i++;
			}
			strcpy(tmp,"./test.h264");
			download_param[i].ptr_file = fopen(tmp, "r+b");//��ȡ������Ƶ�ļ����ļ������� 

			if(download_param[i].ptr_file == NULL) 
			{ 
				printf("\n fopen error !\n"); 
				perror("fopen"); 
			} 

			strcpy(download_param[i].filename,start_media->fileName); 

			download_param[i].id = id; 		 

			download_param[i].flag = 1;//��־λflag = 1��ȡ��Ƶ֡��flag = 0ֹͣ��ȡ��Ƶ֡ 	

			download_param[i].video_id = start_media->media.video_id;//��Ƶͨ��	 
			download_param[i].media_type = start_media->media.media_type;//ý������
			download_param[i].stream_type = start_media->media.stream_type;//��������
            pthread_attr_t attr;
            //��������Ϊ��detached  state��
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			if(pthread_create((pthread_t *)&download_param[i].pid,&attr,stream_proc,(void *)&download_param[i]) != 0) 
			{ 
				fprintf(stderr,"can't create thread %s\n",strerror(errno));
                //LOG("pthread_create %d %s \n",errno,strerror(errno));
                //LOG("thread create failed\n");
				//exit(1); 
			} 
            pthread_attr_destroy (&attr);
		}
        #endif
	}
    #if 1
	else if(strcmp(cmd, "Stop_MEDIA") == 0)
	{
		//printf("\n=========Stop_MEDIA_1================\n");
		LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
		if(start_media->request_type==0)
		{
			video_id = start_media->media.video_id;
            int i=0;
			int find_task=0;
			while(i<100)
			{

				if(realtime_param[i].ptr_file!=NULL &&
							realtime_param[i].video_id == start_media->media.video_id &&
							realtime_param[i].stream_type == start_media->media.stream_type)
				{
					find_task=1;  
					break;
				}
				i++;
			}
			if(find_task==0)
			  return 0;
			realtime_param[i].flag = 0;
            
			//pthread_join(realtime_param[i].pid,NULL);
			//sleep(1);

            #if 0
			if (realtime_param[i].ptr_file)
			{
				fclose(realtime_param[i].ptr_file);
                realtime_param[i].ptr_file = NULL;
			}
            #endif
		}
		else if(start_media->request_type==1)
		{
            printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					start_media->start_time.year,
					start_media->start_time.month,
					start_media->start_time.date,
					start_media->start_time.hour,
					start_media->start_time.minute,
					start_media->start_time.second);//��ʼ�����ڼ�ʱ��
			int i=0;
			int find_task=0;
			while(i<100)
			{

				if(playback_param[i].ptr_file!=NULL &&
							strcmp(playback_param[i].filename,start_media->fileName)==0)
				{
					find_task=1;  
					break;
				}
				i++;
			}
			if(find_task==0)
			  return 0;

			playback_param[i].flag = 0;

			//pthread_join(playback_param[i].pid,NULL);
            #if 0
			if (playback_param[i].ptr_file)
			{
				fclose(playback_param[i].ptr_file);
				playback_param[i].ptr_file=NULL;
			}
            #endif
		}
		else if(start_media->request_type==2)
		{
			int i=0;
			int find_task=0;
			while(i<100)
			{
				if(download_param[i].ptr_file!=NULL &&
							strcmp(download_param[i].filename,start_media->fileName)==0)
				{
					find_task=1;  
					break;
				}
				i++;
			}
			if(find_task==0)
			  return 0;

			download_param[i].flag = 0;

			//pthread_join(download_param[i].pid,NULL);
#if 0
			if (download_param[i].ptr_file)
			{
				fclose(download_param[i].ptr_file);
				download_param[i].ptr_file=NULL;
			}
         #endif
		}
	}
	else  if(strcmp(cmd, "Start_AUDIO") == 0)
	{

	}

	else if(strcmp(cmd, "Stop_AUDIO") == 0)
	{

	}
	else if(strcmp(cmd, "Control_MEDIA") == 0)//�طſ���
	{
		LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;

		video_id = start_media->media.video_id;//ͨ����
		float scale = start_media->scale;//��������

		if(scale==0)
		{
			//��ͣ  
		}
		else if(1.0 >scale&&scale>0) 
		{
			//����scale ��ֵ����0,��˵����ʼ����
		}
		else if(scale>1) //���
		{

		}
		//�ı�g_sleep_time ��ֵ�����Ʒ����ٶȣ������ȫ�ֱ�����ֻΪ�򵥵�ʵ��
		if(scale==1.0)
		  g_sleep_time=40000;
		else if(scale==2.0)
		  g_sleep_time=40000/2;
		else if(scale==4.0)
		  g_sleep_time=40000/4;
		else if(scale==8.0)
		  g_sleep_time=40000/8;
		else if(scale==0.5)
		  g_sleep_time=40000*2;
		else if(scale==0.25)
		  g_sleep_time=40000*4;
		else if(scale==0.125)
		  g_sleep_time=40000*4;
		else
		  g_sleep_time=40000;

	}
    #endif

	return 0;
}

void* stream_proc(void* param)
{
    #if 1
	LPNET_STREAM_OPEN_PARAM ptr_param = (LPNET_STREAM_OPEN_PARAM)param;
	int ret=-1;
	int iRet = -1;
	//int marker=0;
	//int frame=0;
	size_t size=0;
	//long long count = 0;
    int video_id = ptr_param->video_id;
    int stream_type = ptr_param->stream_type;
	char *buffer = NULL;
    buffer = (char *)malloc(65536);
    if(buffer == NULL)
    {
        printf("malloc failed %d %s ",errno,strerror(errno));
        //return -1;
        exit(1);
    }
    printf("buffer malloc ________________________________%d \n",sizeof(buffer));
    //int time = 0;

	NAL_HEADER nalheader;
	//unsigned int  port  = 9123; 
	int skt   =  ptr_param->audio_skt;
	ptr_param->flag = 1;
    
	while(ptr_param->flag)
	{
        #if 0
        if((++time) >= 350)
        {
            printf("+++++++++++++++++++++++++time+++++++++%d\n",time);
            net_stop_stream(ptr_param->id);
			// printf("read data error:%d,%d\n",ret,size);
			break;
        }
        #endif
		usleep(40000);
		memset(&nalheader,0,sizeof(NAL_HEADER));
		if(ptr_param->ptr_file==NULL)
		  break;

		ret=fread(&nalheader , 1,sizeof(NAL_HEADER), ptr_param->ptr_file);

		if(ret != sizeof(NAL_HEADER))
		{
			//��ȡ����ʧ��,�ر�������
			fseek(ptr_param->ptr_file,0,SEEK_SET);
			continue;
		}

		//����ͷ�д�����һ֡��Ƶ�Ĵ�С 
		size = nalheader.size; 
        //printf("size %d \n",size);
		//������ȡһ֡���� 
		if(ptr_param->ptr_file==NULL)
		  break;
		ret = fread(buffer, 1, size, ptr_param->ptr_file); 
		if(ret != size || buffer == NULL)
		{
			printf("read data error:%d,%d\n",ret,size);
            fseek(ptr_param->ptr_file,0,SEEK_SET);
			//��ȡ����ʧ��,�ر�������
			net_stop_stream(ptr_param->id);
			break;
		}	
         //printf("net_send_stream%s %d\n",buffer,ptr_param->ptr_file);
		ptr_param->count++;
		if(ptr_param->count%30==0)
		{
			iRet=net_send_stream(ptr_param->id, buffer,size,1);
            //printf("net_send_stream%s\n",buffer);
           
			ptr_param->count=0;
		}
		else
		{
            iRet=net_send_stream(ptr_param->id, buffer,size,0); 
            //printf("net_send_stream%s\n",buffer);

        }
		  
		//������Ƶ����
		if(iRet==-1)
		{
            fseek(ptr_param->ptr_file,0,SEEK_SET);
            printf("");
            net_stop_stream(ptr_param->id);
            break;
        }
		  

	}
	if (NULL != ptr_param->ptr_file && video_id == ptr_param->video_id && stream_type == ptr_param->stream_type)
	{
        printf("close ptr_param->ptr_file\n");
		fclose(ptr_param->ptr_file);
		ptr_param->ptr_file=NULL;
	}
    if(buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
        printf("free buffer_____________________________________________________\n");
    }
	//return 0;
    #endif
    #if 0
    LPNET_STREAM_OPEN_PARAM ptr_param = (LPNET_STREAM_OPEN_PARAM)param;
    char buffer[5000];
    memset(buffer,2,sizeof(buffer));
    int video_id = ptr_param->video_id;
    int stream_type = ptr_param->stream_type;
    ptr_param->flag = 1;
    
	while(ptr_param->flag)
	{
        usleep(10);
        int ret = fread(buffer, 1, sizeof(buffer), ptr_param->ptr_file); 
        if(ret != sizeof(buffer))
		{
			//��ȡ����ʧ��,�ر�������
			fseek(ptr_param->ptr_file,0,SEEK_SET);
			continue;
		}
        int iRet=net_send_stream(ptr_param->id, buffer,sizeof(buffer),0); 
    	if(iRet==-1)
    	{
            fseek(ptr_param->ptr_file,0,SEEK_SET);
            printf("");
            net_stop_stream(ptr_param->id);
            break;
        }
     
	}
   if (NULL != ptr_param->ptr_file && video_id == ptr_param->video_id && stream_type == ptr_param->stream_type)
	{
        printf("close ptr_param->ptr_file\n");
		fclose(ptr_param->ptr_file);
		ptr_param->ptr_file=NULL;
	}
   #endif
}


