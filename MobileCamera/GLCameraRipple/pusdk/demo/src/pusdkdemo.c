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

//g_sleep_time是快放，慢放的时候，控制发送速率的，demo中简单的实现
int g_sleep_time=1000000/25;

//码流操作Demo
void *stream_proc(void* param); 
//告警操作demo
int alarm_proc(net_info_t info,int t,int select); 

//int send_data(char *buffer,int size,unsigned long param);


int on_login( int type, char* ip, unsigned int port, unsigned long error, unsigned long times, unsigned long param  ) 
{ 
	return 0; 
} 

int on_msg( char* cmd, char* buf, unsigned long* size, unsigned long param )//消息处理,参数size,param暂时未用 
{
    //printf("on_msg cmd ++++++++++++%s\n",cmd);
	//设置设备时间参数 
	if(strcmp(cmd,SET_PU_TIME) == 0) 
	{ 
		//LPNET_CMU_DEV_TIME basic = (LPNET_CMU_DEV_TIME)buf; 
		/* 
		   typedef struct										 
		   { 
		   int hour;		// 时 (0~23) 
		   int minute;		// 分 (0~59) 
		   int second;		// 秒 (0~59) 
		   int date;		// 日 (1~31) 
		   int month;		// 月 (1~12) 
		   int year;		// 年 nnnn 
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

	//获取设备版本
	else if(strcmp(cmd, GET_PU_SOFTWAREVERSION) == 0)
	{
		LPNET_CMU_SOFT_VERSION basic = (LPNET_CMU_SOFT_VERSION)buf;
		strncpy(basic->pu_id,"10002",32);
		strncpy(basic->version,"10.1.1",32);
		strncpy(basic->build_time,"2001-03-23 23:34:34",32);
	}

	//设置设备网络参数 
	else if(strcmp(cmd,SET_PU_IPINFO) == 0)	 
	{ 
		LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf; 
		/* 
		   typedef struct										 
		   {		 
		   int   net_id;				// 网口编号 (1,2,3...)  
		   char  ip_addr[15];			// IP地址 
		   int   ctrl_port;			    // 业务控制端口号 
		   char  net_mask[15];			// 子网掩码 
		   char  dns_addr[2][15];		// DNS服务器地址,可以有多个 
		   char  gate_way[15];			// 网关地址 
		   }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO; 
		   */ 
		printf("netId is %d\n",basic->net_id); 
		printf("ip_addr is %s\n",basic->ip_addr); 
		printf("ctrl_port is %d\n",basic->ctrl_port); 
		printf("net_mask is %s\n",basic->net_mask); 
		printf("dns_addr is %s\n",basic->ip_addr); 
		printf("gate_way is %s\n",basic->gate_way); 
	} 

	//获取设备网络参数 
	else if(strcmp(cmd,GET_PU_IPINFO)== 0) 
	{ 
		LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf; 
		/* 
		   typedef struct										 
		   {		 
		   int   net_id;	            // 网口编号 (1,2,3...)  
		   char  ip_addr[15];           // IP地址 
		   int   ctrl_port;		        // 业务控制端口号 
		   char  net_mask[15];	        // 子网掩码 
		   char  dns_addr[2][15];		// DNS服务器地址,可以有多个 
		   char  gate_way[15];			// 网关地址 
		   }NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO; 
		   */ 

		basic->net_id = 2; 
		strncpy(basic->ip_addr, "192.168.2.72", 15); 
		basic->ctrl_port = 1001; 
		strncpy(basic->net_mask,"255.255.255.0",15); 
		strncpy(basic->dns_addr[0], "192.168.1.1", 15); 
		strncpy(basic->gate_way, "192.168.1.1", 15); 

	} 

	//设置云台 
	else if(strcmp(cmd, OPER_PU_CONTROLPTZ) == 0) 
	{ 
		LPNET_CMU_OPER_CTRL_PTZ basic = (LPNET_CMU_OPER_CTRL_PTZ)buf; 
		/* 
		   typedef struct								 
		   { 
		   int  ptz_id;	云台标识 

		   char cmd[32];							 
		   焦距拉近/拉远：ZIN/ZOUT； 
		   焦点调近/调远：FN/FR； 
		   光圈自动调整/增大/缩小：IA_ON/IO/IC； 
		   擦拭启动：WP_ON； 
		   灯光开启：LP_ON 
		   巡航启动：AUTO_START/AUTO_STOP； 
		   方向调整： 上TU,下TD,左PL,右PR, 右上TUPR , 左上TUPL, 右下TDPR, 左下TDPL,  
		   转到预设点：GOTO_PRESET 
		   轨迹巡航： 
		   开始巡航：START_TRACK_CRUISE 
		   停止：STOP  

		   char param[200];						 
		   当cmd是GOTO_PRESET时，该参数表示预设点序号。 
		   当cmd是START_TRACK_CRUISE该参数是巡航轨迹索引。 
		   当cmd是STOP时，该参数指出要停止的控制命令（即导致云台运动的命令） 

		   int  speed;// 云台速度	1-8，共8个等级，数值越高速度越快。		 

		   }NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ; 
		   */ 

		printf("ptz_id is%d \n",basic->ptz_id);//云台标识  
		printf("cmd is%s\n",basic->cmd); 
		printf("param is%s\n",basic->param); 
		printf("seep is%d\n",basic->speed);//云台速度	 
	}


	//设置云台预置点
	else if(strcmp(cmd,SET_PU_PRESETPTZ) == 0)
	{

		LPNET_CMU_PRESET_LIST basic = (LPNET_CMU_PRESET_LIST)buf;
		/*
		   typedef struct
		   {
		   int  preset_index;					// 预置点编号
		   char preset_name[16];				// 预置点名称
		   }NET_CMU_PRESET, *LPNET_CMU_PRESET;

		   typedef struct						// 设备云台预置点
		   {
		   int ptz_id;							// 云台标识
		   int preset_list_cnt;					// 
		   NET_CMU_PRESET *preset_lists;		 
		   }NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;
		   */
		printf("basic->ptz_id %d\n",basic->ptz_id);//云台标识
		printf("basic->preset_lists->preset_index%d\n",basic->preset_lists->preset_index);//预置点索引
		printf("basic->preset_lists->preset_name%s\n",basic->preset_lists->preset_name);//预置点名称

	}
    //删除云台预置点
    else if(strcmp(cmd,OPER_PU_DELPRESETPTZ ) == 0)
    {
       // LPNET_CMU_OPER_DEL_PRESET basic = (LPNET_CMU_OPER_DEL_PRESET)buf;
        
    }
	//查询录像文件
	else if(strcmp(cmd, OPER_PU_QUERYPULOCALSTORAGEFILES) == 0)
	{
		LPNET_CMU_QUERY_STORE_FILE basic = (LPNET_CMU_QUERY_STORE_FILE)buf; 
		int cnt = 10;
		int num = 0;
		int i = 1;
		char fileName[64]={0};
		//char startTime[20]={0};
		//char endTime[20]={0};

		printf("video_id is %d \n",basic->video_id);//查询文件的视频通道
		printf("file_type is %d\n",basic->file_type);//查询文件的类型
		printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					basic->from_date.year,
					basic->from_date.month,
					basic->from_date.date,
					basic->from_date.hour,
					basic->from_date.minute,
					basic->from_date.second);//开始的日期及时间

		printf("from_date is %04d-%02d-%02d %02d:%02d:%02d\n",
					basic->to_date.year,
					basic->to_date.month,
					basic->to_date.date,
					basic->to_date.hour,
					basic->to_date.minute,
					basic->to_date.second);//开始的日期及时间

		basic->record_file_lists = (LPNET_CMU_RECORD_FILE)malloc(cnt *sizeof(NET_CMU_RECORD_FILE));
        printf("+_++++++++++++++++++++++++++++++++++++++++++++%d  %d\n",cnt *sizeof(NET_CMU_RECORD_FILE),sizeof(NET_CMU_QUERY_STORE_FILE));
        //basic->record_file_size = 4096;
		basic->file_list_cnt = cnt;

		basic->total_size = 1000;//DISK 容量大小
		basic->free_size = 1000;//DISK剩余容量
		basic->total_num = 100;//总共文件条数
		basic->leave_num = 20;//剩余文件条数
		basic->result_code = 0;//注册结果 （code）: 0：OK  1：不支持的文件类型 48：客户端错误	49：服务器错误 50：其他失败原因	

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
	//设备目录查询
	else if(strcmp(cmd,OPER_PU_RECORD) == 0)
	{
		LPNET_CMU_OPER_RECORD_DEV  record_dev=(LPNET_CMU_OPER_RECORD_DEV)buf;
		printf("ResetAlarm pu_id:%s,cmd:%d\n",record_dev->pu_id,record_dev->control_action);
	}
	//状态查询
	else if(strcmp(cmd,OPER_PU_STATUS)==0)
	{
		//前段状态查询。
		DEVICE_ARLARMSTATUS_INFO **alarm_status=(DEVICE_ARLARMSTATUS_INFO**)buf;
		int i=0;
		//此处根据告警设备的ID，填充告警设备的状态。
		while( i < *size)
		{
			printf("DEVICE_ARLARMSTATUS_INFO:%s %d\n",alarm_status[i]->deviceid,*size);
			strcpy(alarm_status[i]->dutystatus,"ONDUTY");
			i++;
		}
	}

	// 撒放/布防
	else if(strcmp(cmd,OPER_PU_GUARD) == 0) 
	{
		//NET_CMU_OPER_GUARD_DEV guard_dev =(LPNET_CMU_OPER_GUARD_DEV)buf;
	}
	// 设备远程重启
	else if(strcmp(cmd, OPER_PU_CONTROLPU) == 0) 
	{
		LPNET_CMU_OPER_CTRL_DEV basic = (LPNET_CMU_OPER_CTRL_DEV)buf;
		printf("LPNET_CMU_OPER_CTRL_DEV %d\n",basic->control_action);//basic->control_action==0重启==1关机==2通知上线了==3通知掉线了
	}
    //时间文本设置
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
    //设备图像编码参数
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
    //串口参数
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
    //图像显示参数
    else if(strcmp(cmd, SET_PU_IMAGEDISPLAYPARA) == 0)
    {
	
    	LPNET_CMU_IMG_DISPLAY basic = (LPNET_CMU_IMG_DISPLAY)buf;
    	printf("video_id is %d\n",basic->video_id);
    	printf("contrast is %d\n",basic->contrast);
    	printf("bright is %d\n",basic->bright);
    	printf("hue is %d\n",basic->hue);
    	printf("saturation is %d\n",basic->saturation);

    }
    //设备图像制式参数
    else if(strcmp(cmd, SET_PU_VIDEOFORMAT) == 0)
    {

		LPNET_CMU_VIDEO_FORMAT basic = (LPNET_CMU_VIDEO_FORMAT)buf;
		printf("video_id is %d\n",basic->video_id);
		printf("video_format is %d\n",basic->video_format);

	}
    // 设备视频遮蔽告警参数
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
    //移动侦测告警参数设置
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

		//启动移动侦测
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

//告警上报
int alarm_proc(net_info_t info,int t,int select) 
{ 
	char *cmd = UPLOAD_PU_RAISEALARM;//设备告警上报 	

	NET_CMU_UPLOAD_ALARM  ptr_param;//告警信息  

	int type = 0; 
	char pszDate[32]= "\0";//存放报警时间的缓冲区
	char data[64];//报警的详细信息
	struct tm *ptm; 
	time_t TimeFile; 

	//目前只实现 VS_GPIN：开关量告警，CAM_VIDEOLOSE：视频丢失 ，CAM_HIDEDETECT：遮挡告警


	//NET_CMU_UPLOAD_ALARM 上传告警的结构体
	memset(&ptr_param,0,sizeof(NET_CMU_UPLOAD_ALARM)); 

	// 首先填入 告警时间    
	memset(&pszDate,0,sizeof(pszDate)); 
	TimeFile = time(NULL); 
	ptm = localtime(&TimeFile); 
	sprintf(pszDate,"%04d-%02d-%02d %02d:%02d:%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);//报警的时间 
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
	//然后填入告警类型
	if(t==1)
	  type = VS_GPIN;//CAM_HIDEDETECT;//目前只实现 VS_GPIN：开关量告警，CAM_VIDEOLOSE：视频丢失 ，CAM_HIDEDETECT：遮挡告警CAM_MOTIONDETECT移动侦测
	else if(t == 2)
	  type  = CAM_VIDEOLOSE;
	else if(t == 3)
	  type = CAM_HIDEDETECT;
    else
      type = CAM_MOTIONDETECT;

	ptr_param.alarm_type = type;//报警类型 

	//如果是硬盘错误，填入硬盘号
	ptr_param.disk_number = 0; //报警的硬盘号

	strcpy(data,"1");
	/*告警的详细信息，是一个字符串
	  strcpy(data,"d:70%");
	  if((strlen(data) + 1) < 64)//报警详细信息长度不能超过64个字节
	  strcpy(ptr_param.data,data);
	//告警的设备类型
	strncpy(ptr_param.server_type,"CAM",32);//告警设备类型 

	//最后调用net_send_alarm,上传告警信息*/
	strcpy(ptr_param.eliminated,"false");//告警发生
	ptr_param.data.integerValue=1;
    ptr_param.select_platform = MINGSOFT_PLATFORM;
	net_send_alarm(cmd,(char *)&ptr_param,sizeof(NET_CMU_UPLOAD_ALARM));//告警上报 
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
            //这里设置为“detached  state”
            pthread_attr_init(&attr);
            printf("[on_stream] init\n");
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    	/* 开始运行线程 */
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
					start_media->start_time.second);//开始的日期及时间
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
			playback_param[i].ptr_file = fopen(tmp, "r+b");//获取请求视频文件的文件描述符 


			if(playback_param[i].ptr_file == NULL) 
			{ 
				printf("\n fopen error !\n"); 
				perror("fopen"); 
			} 

			strcpy(playback_param[i].filename,start_media->fileName); 

			playback_param[i].id = id; 		 

			playback_param[i].flag = 1;//标志位flag = 1读取视频帧，flag = 0停止读取视频帧 	

			playback_param[i].video_id = start_media->media.video_id;//视频通道	 
			playback_param[i].media_type = start_media->media.media_type;//媒体类型
			playback_param[i].stream_type = start_media->media.stream_type;//码流类型
            pthread_attr_t attr;
            //这里设置为“detached  state”
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    	    /* 开始运行线程 */
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
			download_param[i].ptr_file = fopen(tmp, "r+b");//获取请求视频文件的文件描述符 

			if(download_param[i].ptr_file == NULL) 
			{ 
				printf("\n fopen error !\n"); 
				perror("fopen"); 
			} 

			strcpy(download_param[i].filename,start_media->fileName); 

			download_param[i].id = id; 		 

			download_param[i].flag = 1;//标志位flag = 1读取视频帧，flag = 0停止读取视频帧 	

			download_param[i].video_id = start_media->media.video_id;//视频通道	 
			download_param[i].media_type = start_media->media.media_type;//媒体类型
			download_param[i].stream_type = start_media->media.stream_type;//码流类型
            pthread_attr_t attr;
            //这里设置为“detached  state”
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
					start_media->start_time.second);//开始的日期及时间
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
	else if(strcmp(cmd, "Control_MEDIA") == 0)//回放控制
	{
		LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;

		video_id = start_media->media.video_id;//通道号
		float scale = start_media->scale;//操作参数

		if(scale==0)
		{
			//暂停  
		}
		else if(1.0 >scale&&scale>0) 
		{
			//慢放scale 的值大于0,就说明开始播放
		}
		else if(scale>1) //快放
		{

		}
		//改变g_sleep_time 的值，控制发送速度，这里的全局变量，只为简单的实现
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
			//读取数据失败,关闭流任务
			fseek(ptr_param->ptr_file,0,SEEK_SET);
			continue;
		}

		//数据头中存着下一帧视频的大小 
		size = nalheader.size; 
        //printf("size %d \n",size);
		//继续读取一帧数据 
		if(ptr_param->ptr_file==NULL)
		  break;
		ret = fread(buffer, 1, size, ptr_param->ptr_file); 
		if(ret != size || buffer == NULL)
		{
			printf("read data error:%d,%d\n",ret,size);
            fseek(ptr_param->ptr_file,0,SEEK_SET);
			//读取数据失败,关闭流任务
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
		  
		//结束视频保存
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
			//读取数据失败,关闭流任务
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


