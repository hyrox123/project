#ifndef __MS_PU_PARAM_H__
#define __MS_PU_PARAM_H__

//#include "stdafx.h"

/*******************************************************************************/
/* ----Register---- */
#define REG_PU_REGISTER							"PuRegister"				// 设备注册
#define REG_PU_KEEPALIVE						"PuKeepAlive"				// 设备心跳
/*******************************************************************************/

/*******************************************************************************/
/* ----Set---- */
#define SET_PU_BASICINFO						"SetPuBasicInfo"			// 设备基本参数
#define SET_PU_IPINFO							"SetPuIpInfo"				// 设备网络参数
#define SET_PU_USER								"SetPuUser"					// 设备用户参数
#define SET_PU_SERIALPORT						"SetPuSerialPort"			// 设备串口参数
#define SET_PU_OUTDEV							"SetPuOutDev"				// 设备输出接口参数
#define SET_PU_TIME								"SetPuTime"					// 设备时间参数

#define SET_PU_VIDEOFORMAT						"SetPuVideoFormat"			// 设备图像制式参数
#define SET_PU_IMAGEENCODEPARA					"SetPuImageEncodePara"		// 设备图像编码参数
#define SET_PU_IMAGEDISPLAYPARA					"SetPuImageDisplayPara"		// 设备图像显示参数
#define SET_PU_IMAGETEXTPARA					"SetPuImageTextPara"		// 设备图像相关的文字参数

#define SET_PU_ALARMPARAMS						"SetPuAlarmParams"			// 设备基本告警参数
#define SET_PU_GIPINALARM						"SetPuGpinAlarm"			// 设备Gpin告警参数

#define SET_PU_VIDEOLOSE						"SetPuVideoLose"			// 设备视频丢失告警参数
#define SET_PU_HIDEDETECTION					"SetPuHideDetection"		// 设备视频遮蔽告警参数
#define SET_PU_MOTIONDETECTION					"SetPuMotionDetection"		// 设备移动侦测告警初始化参数
#define SET_PU_IMAGEHIDEAREA					"SetPuImageHideArea"		// 设备图像遮挡区域

#define SET_PU_LOCALSTORAGETASK					"SetPuLocalStorageTask"		// 设备本地存储参数

#define SET_PU_PRESETPTZ						"SetPresetPTZ"				// 设备云台预置点
#define SET_PU_DEFAULTPRESETPTZ					"SetDefaulePresetPTZ"		// 设备默认云台预置点
#define SET_PU_CRUISETRACK						"SetCruiseTrack"			// 设备巡航轨迹
/*******************************************************************************/

/*******************************************************************************/
//Get
#define GET_PU_BASICINFO						"GetPuBasicInfo"			// 设备基本参数
#define GET_PU_IPINFO							"GetPuIpInfo"				// 设备网络参数
#define GET_PU_USER								"GetPuUser"					// 设备用户参数
#define GET_PU_SERIALPORT						"GetPuSerialPort"			// 设备串口参数
#define GET_PU_INOUTDEV							"GetPuInOutDev"				// 设备输出接口参数
#define GET_PU_TIME								"GetPuTime"					// 设备时间参数

#define GET_PU_VIDEOFORMAT						"GetPuVideoFormat"			// 设备图像制式参数
#define GET_PU_IMAGEENCODEPARA					"GetPuImageEncodePara"		// 设备图像编码参数
#define GET_PU_IMAGEDISPLAYPARA					"GetPuImageDisplayPara"		// 设备图像显示参数
#define GET_PU_IMAGETEXTPARA					"GetPuImageTextPara"		// 设备图像相关的文字参数

#define GET_PU_ALARMPARAMS						"GetPuAlarmParams"			// 设备基本告警参数
#define GET_PU_GIPINALARM						"GetPuGpinAlarm"			// 设备Gpin告警参数

#define GET_PU_VIDEOLOSE						"GetPuVideoLose"			// 设备视频丢失告警参数
#define GET_PU_HIDEDETECTION					"GetPuHideDetection"		// 设备视频遮蔽告警参数
#define GET_PU_MOTIONDETECTION					"GetPuMotionDetection"		// 设备移动侦测告警初始化参数
#define GET_PU_IMAGEHIDEAREA					"GetPuImageHideArea"		// 设备图像遮挡区域

#define GET_PU_LOCALSTORAGETASK					"GetPuLocalStorageTask"		// 设备本地存储参数

#define GET_PU_PRESETPTZ						"GetPresetPTZ"				// 设备云台预置点
#define GET_PU_DEFAULTPRESETPTZ					"GetDefaulePresetPTZ"		// 设备默认云台预置点
#define GET_PU_CRUISETRACK						"GetCruiseTrack"			// 设备巡航轨迹

#define GET_PU_STATE							"GetPuState"				// 设备状态信息
#define GET_PU_LOG								"GetPuLog"					// 设备日志信息

#define GET_PU_SOFTWAREVERSION					"GetPuSoftwareVersion"		// 设备软件版本信息
/*******************************************************************************/
#define OPER_PU_STATUS							"DeviceStatus"  //设备状态查询

/*******************************************************************************/
//operation
#define OPER_PU_DEL_USER						"DeletePuUser"				// 删除设备用户信息
#define OPER_PU_CLEANPULOCALSTORAGEFILE			"CleanPuLocalStorageFile"	// 删除设备录像文件
#define OPER_PU_QUERYPULOCALSTORAGEFILES		"QueryPuLocalStorageFiles"	// 查询设备录像文件
#define OPER_PU_CONTROLPTZ						"ControlPTZ"				// 设备云镜控制请求
#define GET_LOCAL_IP                            "GetLocalIP"                //获取本地IP
#define OPER_PU_GUARD                           "GuardCmd"                  //撒放/布防
#define OPER_PU_RECORD                          "ReocrdCmd"                 //撒放/布防
#define OPER_PU_STOPRECORD                      "StopReocrdCmd"             //撒放/布防
#define OPER_PU_DELPRESETPTZ					"DelPresetPTZ"				// 设备删除云台预置点
#define OPER_PU_CONTROLPU						"ControlPU"					// 设备远程重启
#define OPER_PU_TRANSPARENTCHANNEL				"TransparentChannel"		// 设备通明通道
#define OPER_PU_TELEBOOT				        "TeleBoot"		            // 设备通明通道
#define OPER_PU_ARLRMCMD				        "AlarmCmd"		            // 设备通明通道
/*******************************************************************************/
#define OPER_PU_CONTROLFILEBACK                 "ControlFileBack"
/*******************************************************************************/
//upload
#define UPLOAD_PU_RAISEALARM					"RaiseAlarm"				// 设备告警上报
#define UPLOAD_PU_RAISEDIGITAL					"RaiseDigital"				// 设备数字量上报
#define OPER_PU_AlARMQUERY                      "AlarmQuery"                //报警查询
/*******************************************************************************/

//最大支持移动侦测区域数
#define 	MAX_MOTIONDETECTIONNUM	8

//最大支持遮挡区域数
#define	MAX_IMAGEHIDEAREANUM	8

//前端类型
#define PU_TYPE_DVS		0
#define PU_TYPE_DVR		1
#define PU_TYPE_IP_CAM	2
#define PU_TYPE_OTHER	3

//存储策略操作类型
#define STORE_POLICY_CLEAN		1
#define STORE_POLICY_SET		2

//告警类型
#define VS_DISKERROR		0
#define VS_DISKFULL			1
#define VS_DISCONN			2
#define VS_UPGRADE			3
#define VS_GPIN				4//2//2在、跟天视通，恩智暂时定为2
#define CAM_VIDEOLOSE		5
#define CAM_HIDEDETECT		6
#define CAM_MOTIONDETECT	7

#define DEFAULT_ALARM_TYPE     20




//请求类型
#define REQUEST_REAL_STREAM		0
#define REQUEST_FILE_STREAM		1
#define DOWNLOAD_FILE_STREAM     2
//传输方式
//码流类型
#define NIFRAME 0//非I帧
#define IFRAME 1//i帧
#define AUDIO  2 //音频数据

//目前只支持TCP



typedef struct										// 设备时间参数
{
	int hour;										// 时 (0~23)
	int minute;										// 分 (0~59)
	int second;										// 秒 (0~59)
	int date;										// 日 (1~31)
	int month;										// 月 (1~12)
	int year;										// 年 nnnn
}NET_CMU_DEV_TIME, *LPNET_CMU_DEV_TIME;


typedef struct								// 设备目录查询
{
    char pu_id[32];
	int control_action;						// 0 - 停止录像  1 - 开始录像		
}NET_CMU_OPER_RECORD_DEV, *LPNET_CMU_OPER_RECORD_DEV;//手动录像,结构体

/*******************************************************************************/
// MDU stream
/*******************************************************************************/
//MDU 视频流参数
typedef struct
{
	char pu_id[64];			// PU的标识
	int  video_id;			// 视频通道 (1,2...)
	int  media_type;		// 媒体类型： 0：音频，如音频对讲的会话 1：视频； 2：音频+视频
	int  stream_type;		// 码流类型： 0：主码流 1：子码流 	2：告警码流

    char server_id[64];
}NET_MDU_MEDIA, *LPNET_MDU_MEDIA;

//MDU 请求视频流信息
typedef struct
{
	NET_MDU_MEDIA media;	// 请求视频流基本信息
	int  link_mode;			// 媒体流发送方式： 0：TCP 	1：UDP 2：RTP/RTCP 3：组播
	char target_ip[32];     // 媒体流发送目的地址
	int  target_port;		// 媒体流发送目的端口
	NET_CMU_DEV_TIME start_time;	
	NET_CMU_DEV_TIME stop_time;	
	char fileName[64];
	int  size;
		
	int  image_width;		// 图像宽度
	int  image_hight;		// 图像高度
	int  video_format;		// 视频格式  0：PAL   1：NTSC
	int  frame_rate;		// 1fps, 2fps, 4fps, 6fps, 8fps, 10fps, 12fps, 16fps 20fps, 25fps
	char video_header[1024];// 视频头数据(Base64编码)
	int  result_code;		// 请求结果：	0：成功 1：不支持的媒体类型 2：不支持的传输方式 3：通道已占用(语音对讲) 50：其他错误
	int request_type;   //请求类型，0:实时视频 1:历史回放 2:下载
    
    float scale;
    /*
       控制速度
       8:快放八倍速 4:快放四倍速2:快放二倍速 1:正常播放，0:为暂停， 0.5:慢放二倍 0.25:慢放4倍 0.125:慢放8倍
       1.暂停操作，停止发送流
       2.快放，倍速发送流
       3.慢放，降低发送速度
    */
    char cmd[20];
    char param[20];

	long iSeq;
	unsigned long long  timestamp;
	int time_intival;
    
    int did;
}NET_MDU_START_MEDIA, *LPNET_MDU_START_MEDIA;

/*******************************************************************************/
//Get--Set
/*******************************************************************************/
typedef struct										// 设备基本参数
{
	char pu_id[32];									// PU的标识
	char pu_name[32];								// PU的名称
	char pu_passwd[32];								// PU的设备密码
}NET_CMU_BASIC_INFO, *LPNET_CMU_BASIC_INFO;

typedef struct										// 设备网络参数
{		
	int   net_id;									// 网口编号 (1,2,3...) 
	char  ip_addr[16];								// IP地址
	int   ctrl_port;								// 业务控制端口号
	char  net_mask[16];								// 子网掩码
	char  dns_addr[2][16];							// DNS服务器地址,可以有多个
	char  gate_way[16];								// 网关地址
}NET_CMU_IP_INFO, *LPNET_CMU_IP_INFO;

typedef struct
{
	char user_name[16];								// 访问PU的用户名
	char pass_word[32];								// 访问PU的密码
}NET_CMU_USER, *LPNET_CMU_USER;

typedef struct										// 设备用户参数
{
	int				user_cnt;
	LPNET_CMU_USER  user_list;
}NET_CMU_USER_LIST, *LPNET_CMU_USER_LIST;

typedef struct										// 设备串口参数
{
	char for_decoder[8];							// true：获取云台解码器信息 false：获取设置串口信息
	int  serial_port;								// 串口编号 (1,2...), 当forDecoder为false时，需要携带该字段
	int  video_id;									// 通道号(1,2...)，当forDecoder为true时，需要携带该字段
	int  baud_rate;									// 波特率(bps) (50, 75, 110, 150, 300, 600, 1200, 2400, 4800, 9600, 19200 38400, 57600, 115200)
	int  data_bit;									// 数据位  （5、6、7、8）
	int  parity;									// 校验 （0－无校验，1－奇校验，2－偶校验）
	int  stop_bit;									// 停止位 （1、2）
	char mode[16];									// (RS485，RS232)
	int  time_out;									// 接收数据超时 （0...65535） unit(100ms)
	int  flow_control;								// 0－无流控，1－软流控,2-硬流控
	int  decoder_type;								// 云台解码器类型, 当forDecoder为true时，需要携带该字段
	int  decoder_address;							// 云台解码器地址, 当forDecoder为true时，需要携带该字段
	int  work_mode;									// 当forDecoder为false时，需要携带该字段 1控制台 2透明通道
}NET_CMU_SERIAL_PORT, *LPNET_CMU_SERIAL_PORT;

typedef struct										// 通道开关量输入输出信息
{
	int channel_num;								// 通道编号
	int state;										// (非零-on , 0-off)有多组设备的状态
}NET_CMU_INOUT_INFO, *LPNET_CMU_INOUT_INFO;



typedef struct										// 开关量输入/输出接口参数
{
	int					in_list_cnt;				// 开关量输入数量			
	int					out_list_cnt;				// 开关量输出数量			

	LPNET_CMU_INOUT_INFO	in_list;					// 开关量输入
	LPNET_CMU_INOUT_INFO	out_list;					// 开关量输出
}NET_CMU_INOUT_DEV, *LPNET_CMU_INOUT_DEV;



typedef struct						
{
	int video_id;									// 视频通道标识（1,2...）
	int video_format;								// 图像制式 0：PAL 1：NTSC
}NET_CMU_VIDEO_FORMAT, *LPNET_CMU_VIDEO_FORMAT;

typedef struct										// 设备图像制式参数		
{
	int video_id;									// 通道标识（1,2...）不输入时表明获取所有视频通道的图像制式。
	int vformat_cnt;								// 通道图像制式 数量
	LPNET_CMU_VIDEO_FORMAT channel_video_format;	// 通道图像制式
}NET_CMU_VIDEO_FORMAT_LIST, *LPNET_CMU_VIDEO_FORMAT_LIST;

typedef struct										// 设备图像编码参数
{
	int  video_id;									// 视频通道 (1,2...)
	char encode_mode[32];							// 编码模式MJPEG MPEG-1 MPEG-2 MPEG-4 H261 H263 H264 AVS
	int  pic_quality;								// 图像质量 （0-最好 1-次好 2-较好 3-一般 4-较差 5-差）
	int  bit_rate;									// 码率 unit(K/s)
	int  bit_rate_type;								// 码率类型0:定码率，1:变码率 
	int  frame_rate;								// 帧率1fps, 2fps, 4fps, 6fps, 8fps, 10fps, 12fps, 16fps 20fps, 25fps
	char image_size[32];							// DCIF,  CIF,  QCIF, 2CIF, 4CIF, 720I, 720P, 1080I, 1080P
	int  stream_type;								// 如果前端设备可以编码两种码流，则 0-主码流 1-子码流
	int  iframe_interval;							// I帧间隔
}NET_CMU_IMG_ENCODE, *LPNET_CMU_IMG_ENCODE;

typedef struct										// 设备图像显示参数
{
	int video_id;									// 视频通道 (1,2...)
	int contrast;									// 对比度（0-255）
	int bright;										// 亮度   (0-255）
	int hue;										// 色调  （0-255）
	int saturation;									// 饱和度（0-255）
}NET_CMU_IMG_DISPLAY, *LPNET_CMU_IMG_DISPLAY;

typedef struct										// 设备图像相关的文字参数
{
	int  video_id;									// 视频通道 (1,2...)
	char time_enable[8];//need chage to char		// 是否显示时间 false-hiden  true-show
	int  time_X;									// 时间戳的X坐标
	int  time_Y;									// 时间戳的Y坐标
	char text_enable[8];//need chage to char		// 是否显示文字 false-hiden  true-show
	char text[128];									// 文字内容
	int  text_X;									// 文字的X坐标
	int  text_Y;									// 文字的Y坐标
}NET_CMU_IMG_TEXT, *LPNET_CMU_IMG_TEXT;

typedef struct									// 联动到告警录像
{
//	char enable[5];								// 抓拍使能 抓拍：true; 不抓拍:false
	int	 record_channel;						// 录像通道1--16可多选
	int  record_time;							// 录像时长 单位分
	int  precord_time;							// 告警录像预录时间
}NET_CMU_ALARM_RECORD, *LPNET_CMU_ALARM_RECORD;

typedef struct									// 联动到告警输出
{
//	char enable[5];								// 告警使能 告警：true; 不告警:false
	int alarm_channel;							// 告警输出通道
	int alarm_time;								// 发生告警后，输出保持时间（秒）
}NET_CMU_ALARM_OUTPUT, *LPNET_CMU_ALARM_OUTPUT;

typedef struct									// 告警布防
{
	char active_start[8];						// 布防开始时间(hhmm-hhmm, 前面2位表示小时，后面2为表示分钟 )
	char active_end[8];							// 布防结束时间(hhmm-hhmm, 前面2位表示小时，后面2为表示分钟 )
}NET_CMU_ACTIVE_TIME, *LPNET_CMU_ACTIVE_TIME;

typedef struct									// 联动到抓拍
{
//	char enable[5];								// 抓拍使能 抓拍：true; 不抓拍:false
	int  shot_channel;							// 抓拍通道
	int  shot_times;							// 抓拍次数
}NET_CMU_SHOT_PHOTO, *LPNET_CMU_SHOT_PHOTO;

typedef struct									// 联动到云台预置位
{
//	char enable[5];								// 联动到云台预置位使能 联动：true; 不联动:false
	int ptz_channel;							// 云台通道
	int ptz_preset;								// 云台预置位
}NET_CMU_PTZ_PRESET, *LPNET_CMU_PTZ_PRESET;

typedef struct									// 设备基本告警参数
{
	int  alarm_in_channel;						// 告警输入通道
	char alarm_enabled[8];						// 告警使能 true : false
	char is_copy_toall[8];						// 复制到其他通道 true : false

	NET_CMU_ALARM_RECORD  alarm_record ;		// 联动到告警录像
	NET_CMU_ALARM_OUTPUT  alarm_output ;		// 联动到告警输出
	NET_CMU_SHOT_PHOTO    shot_photo;			// 联动到抓拍
	NET_CMU_PTZ_PRESET    ptz_preset;			// 联动到云台控制
	NET_CMU_ACTIVE_TIME   active_times;			// 布防时间
}NET_CMU_BASIC_ALARM, *LPNET_CMU_BASIC_ALARM;

typedef struct 
{
	int	alarm_inout_channel;					// 告警输入输出通道号 
	int	alarm_inout_state;						// 告警输入输出端口的状态,0-没有报警,1-有报警
}NET_CMU_ALARM_INOUT, *LPNET_CMU_ALARM_INOUT;

typedef struct 
{
	int	alarm_record_channel;					// 报警触发的录象通道号 
	int	alarm_shot_channel;						// 报警触发的抓拍通道号
}NET_CMU_ALARM_VIDEO, *LPNET_CMU_ALARM_VIDEO;


typedef struct
{
	int x;										// X坐标；左上角为原点
	int y;										// Y坐标
	int width;									// 宽度
	int height;									// 高度
}NET_CMU_COORDINATE, *LPNET_CMU_COORDINATE;

/************************************************************************/
typedef struct									// 设备Gpin告警参数
{
	int  gpin_id;
	char alarm_enabled[8];
	int  alarm_status;
	NET_CMU_ALARM_OUTPUT gpin_alarm_output;
	NET_CMU_ALARM_VIDEO  gpin_alarm_video;
}NET_CMU_GPIN_ALARM_INFO, *LPNET_CMU_GPIN_ALARM_INFO;
/************************************************************************/

typedef struct											// 设备视频丢失告警参数
{
	int video_id;
	int alarm_enabled;
	int alarm_output;
	int alarm_state;
}NET_CMU_VIDEO_LOSE, *LPNET_CMU_VIDEO_LOSE;

typedef struct									// 设备视频遮蔽告警参数
{
	int	 video_id;								// 视频通道 (1,2...)
	char alarm_enabled[8];						// 是否使能视频通道的遮挡检测。 true:表示检测 false：表示不检测
	char alarm_time[16];						// 布防时间(hhmm-hhmm, 前面2位表示小时，后面2为表示分钟 )
	NET_CMU_COORDINATE hide_coord;				// 
	int  sensitivity;							// 灵敏度。 0：关闭 1：低 2：中 3：高
		
	NET_CMU_ALARM_OUTPUT   hide_alarm_output;	// 联动到告警输出
	NET_CMU_ALARM_VIDEO    hide_alarm_video;    // 联动到告警录像
}NET_CMU_HIDE_DETECT, *LPNET_CMU_HIDE_DETECT;


typedef struct									// 设备移动侦测告警初始化参数
{
	int  video_id;								// 视频通道 (1,2...)
	char alarm_enabled[8];						// 是否使能视频通道的遮挡检测。true:表示检测 false：表示不检测	
	char alarm_time[16];						// 布防时间(hhmm-hhmm, 前面2位表示小时，后面2为表示分钟 )
	
	int block_list_cnt;
	LPNET_CMU_COORDINATE block_lists;  
		
	int frequency;								// 频率
	int sensitivity;							// 灵敏度。 灵敏度。 0：关闭 1：低 	2：中 	3：高
		
	NET_CMU_ALARM_OUTPUT  motion_alarm_output;  // 联动到告警输出
	NET_CMU_ALARM_VIDEO   motion_alarm_video;   // 联动到告警录像
}NET_CMU_MOTION_DETECT, *LPNET_CMU_MOTION_DETECT;

typedef struct									// 设备图像遮挡区域
{
	int  video_id;								// 视频通道 (1,2...)
	char enabled[8];							// 是否使能视频通道的遮挡。 true:表示启动 false：表示不启动
	
	int area_list_cnt;
	LPNET_CMU_COORDINATE  area_lists;
}NET_CMU_IMG_HIDE_AREA, *LPNET_CMU_IMG_HIDE_AREA;

typedef struct 
{
	int week_day;							// weekday: 1-7
	int record_type;						// recordType:: 0=定时录像，1=移动侦测, 2=报警触发
	int start_hour;							// startHour: 0-23
	int start_min;							// startMin: 0-59
	int stop_hour;							// stopHour: 0-23
	int stop_min;							// stopMin: 0-59 全天表示为：0，0，23，0
}NET_CMU_STORAGE_SCHEDULE, *LPNET_CMU_STORAGE_SCHEDULE;

typedef struct								 // 设备本地存储参数
{
	int video_id;							 // 视频通道 (1,2...)
	int record_enable;						 // 0:停止录像 1:开始录像，
	int recycle_enable;						 // 0=否，录满后不覆盖 1=是，录满后覆盖，循环录像
	int record_policy;						 // 0=全部存储，1=选择存储
	int frame_interval;						 // 选择存储时，每秒存储帧数
	
	int schedule_list_cnt;
	LPNET_CMU_STORAGE_SCHEDULE schedule_lists;
}NET_CMU_STORAGE_TASK, *LPNET_CMU_STORAGE_TASK;

typedef struct
{
	int  preset_index;						// 预置点索引
	char preset_name[16];					// 预置点名称
}NET_CMU_PRESET, *LPNET_CMU_PRESET;

typedef struct								// 设备云台预置点
{
	int ptz_id;								// 云台标识
	int preset_list_cnt;					// 
	NET_CMU_PRESET *preset_lists;			// 
}NET_CMU_PRESET_LIST, *LPNET_CMU_PRESET_LIST;

typedef struct								// 设备默认云台预置点
{
	int  ptz_id;							// 云台标识
	int  preset_index;                      // 预置点索引
	char preset_name[16];                   // 预置点名称
	int  restore_time;                      // 云台自动归位时间，单位"秒"
}NET_CMU_DEFAULT_PRESET, *LPNET_CMU_DEFAULT_PRESET;

typedef struct								// 设备巡航轨迹
{
	int  ptz_id;							// 云台标识
	int  cruise_num;						// 巡航数量
	char cruise_point[200];					// 巡航点
}NET_CMU_CRUISE_TRACK, *LPNET_CMU_CRUISE_TRACK;

/************************************************************************/
typedef struct								// 设备巡航轨迹
{
	int  ptz_id;							// 云台标识
	int  cruise_track_cnt;
	LPNET_CMU_CRUISE_TRACK cruise_lists;
}NET_CMU_CRUISE_TRACK_LIST, *LPNET_CMU_CRUISE_TRACK_LIST;
/************************************************************************/

typedef struct 
{
	int disk_id;							// 1，2，3...
	int total_size;						    // 硬盘的容量
	int free_size;							// 硬盘的剩余空间
	int status;							    // 硬盘的状态,0-正常， 1-休眠，2-不正常
}NET_CMU_DISK_STATUS, *LPNET_CMU_DISK_STATUS;

typedef struct 
{
	int video_id;							// 1,2,3...
	int record_status;						// 视频通道是否在录像,0-不录像,1-录像
	int signal_status;						// 连接的视频信号状态,0-正常,1-信号丢失
	int hardware_status;					// 通道硬件状态,0-正常,1-异常,例如DSP死掉
	int bit_rate;						    // 实际码率
}NET_CMU_VIDEO_CHANNEL, *LPNET_CMU_VIDEO_CHANNEL;


typedef struct								// 设备状态信息
{
	int device_status;						// 设备的状态,0-正常,1-CPU占用率太高,超过85%,2-硬件错误

	int disk_list_cnt;
	int video_channel_list_cnt;
	int alarm_in_list_cnt;
	int alarm_out_list_cnt;

	LPNET_CMU_DISK_STATUS   disk_status_lists;
	LPNET_CMU_VIDEO_CHANNEL video_channel_lists;
	LPNET_CMU_ALARM_INOUT   alarm_in_lists;
	LPNET_CMU_ALARM_INOUT   alarm_out_lists;
}NET_CMU_DEV_STATE, *LPNET_CMU_DEV_STATE;
	
typedef struct								// 设备日志信息
{
	int  log_type;							// 日志类型，1-报警; 2-异常; 3-操作; 0xff-全部
	char log_time[32];						// 日志时间
	char net_user[32];						// 网络用户名
	int  video_id;							// 视频通道号
	int  disk_num;							// 硬盘号
	int  alarm_in_port;						// 告警输入端口
	int  alarm_out_port;					// 告警输出端口
}NET_CMU_LOG_INFO, *LPNET_CMU_LOG_INFO;

typedef struct								// 设备日志信息
{
	int  log_type;							// 日志类型，1-报警; 2-异常; 3-操作; 0xff-全部
	char start_log_date[32];				// 起始时间：YYYY-MM-DD hh:mm:ss
	char end_log_date[32];					// 终止时间：YYYY-MM-DD hh:mm:ss
	int  start_index;						// 起始记录，从0开始
	int  max_results;						// 最大返回记录条数
	int  log_list_cnt;						// 日志总数
	LPNET_CMU_LOG_INFO log_lists;			// 日志详细信息
}NET_CMU_DEV_LOG, *LPNET_CMU_DEV_LOG;

typedef struct								// 设备软件版本信息
{
	char pu_id[32];							// PU的标识
	char version[32];						// 软件版本号
	char build_time[32];					// 包括日期和时间，格式为: YYYY-MM-DD hh:mm:ss
}NET_CMU_SOFT_VERSION, *LPNET_CMU_SOFT_VERSION;

/*******************************************************************************/
// operation
/*******************************************************************************/
typedef struct								// 删除设备用户信息
{
	char user_name[16];                    // 用户名 	
}NET_CMU_DEL_USER, *LPNET_CMU_DEL_USER;

typedef struct 
{
	char file_name[260];
	NET_CMU_DEV_TIME start_time;
	NET_CMU_DEV_TIME stop_time;
	int  size;
    char  type[16];//TIMER定时录像 GPIN告警录像 MOTION_DETECTION移动侦测
}NET_CMU_RECORD_FILE, *LPNET_CMU_RECORD_FILE;

typedef struct								// 查询设备录像文件
{
	int  video_id;							// 视频通道 (1,2...)
	int  file_type;							// 255=全部（缺省），0=定时录像，1=移动侦测，2=报警触发，3=视频丢失	
	NET_CMU_DEV_TIME from_date;			// 开始的日期及时间。 格式：YYYY-MM-DD hh:mm:ss
	NET_CMU_DEV_TIME to_date;			// 结束的日期及时间。 格式：YYYY-MM-DD hh:mm:ss
	long total_size;						// DISK 容量大小 单位：Bytes
	long free_size;							// DISK剩余容量 单位：Bytes
	
	int file_list_cnt;
	int  total_num;							// 总共文件条数
	int  leave_num;							// 剩余文件条数
	int  result_code;						// 注册结果 （code）: 0：OK  1：不支持的文件类型 48：客户端错误	49：服务器错误 50：其他失败原因
	LPNET_CMU_RECORD_FILE record_file_lists;

    char name[32];
	char file_path[32];
	char address[32];
	int secrecy;
	char type[16];
	char record_id[32];
    int record_file_size;//查询到的总得文件的长度
}NET_CMU_QUERY_STORE_FILE, *LPNET_CMU_QUERY_STORE_FILE;

typedef struct								// 删除设备录像文件
{
	int  video_id;							// 视频通道号：1，2，…
	char file_name[260];					// 要删除的文件名
	char from_date[20];						// 开始的日期及时间。格式：YYYY-MM-DD hh:mm:ss	
	char to_date [20]; 						// 结束的日期及时间。格式：YYYY-MM-DD hh:mm:ss		
}NET_CMU_CLEAN_STORE_FILE, *LPNET_CMU_CLEAN_STORE_FILE;

typedef struct								// 设备云镜控制请求
{
	int  ptz_id;				//云台标识
	char cmd[32];					/*
											焦距拉近/拉远：ZIN/ZOUT
											焦点调近/调远：FN/FR
											光圈自动调整/增大/缩小：IA_ON/IO/IC；
											擦拭启动：WP_ON
											灯光开启：LP_ON
											
											方向调整： 上TU, 下TD, 左PL, 右PR, 右上TUPR, 左上TUPL, 右下TDPR, 左下TDPL
											
											预置点操作：
											设置预置点：SET_PRESET
											删除预置点：DEL_PRESET
											转到预设点：GOTO_PRESET
											
											轨迹巡航：
											加入巡航点：SET_CRUISE_POINT
											删除巡航点：DEL_CRUISE_POINT
											设置巡航速度：SET_CRUISE_SPEED
											设置巡航停留时间：SET_CRUISE_TIME
											开始巡航：START_TRACK_CRUISE
											
											自动扫描：
											设置自动扫描边界：SET_SCAN_EDGE
											设置自动扫描速度：SET_SCAN_SPEED
											开始自动扫描：START_SCAN
											
											停止：STOP
											*/																					
	char param[200];			//当cmd是STOP时，该参数指出要停止的控制命令（即导致云台运动的命令）
	int param1;						/*
											当cmd为预置点相关操作时，该参数表示预设点序号
											当cmd为轨迹巡航相关操作时，该参数表示巡航轨迹索引
											当cmd为自动扫描相关操作时，该参数表示自动扫描组号
											*/
	int param2;						/*
											当cmd为加入、删除巡航点时，该参数表示预置点值
											当cmd为设置巡航速度时，该参数表示巡航速度值
											当cmd为设置巡航停留时间时，该参数表示巡航停留时间
											当cmd为设置自动扫描边界时，该参数表示边界类型，1为左边界，2为右边界
											当cmd为设置自动扫描速度时，该参数表示速度值
											*/
	int  speed;						// 云台速度	1-8，共8个等级，数值越高速度越快。
}NET_CMU_OPER_CTRL_PTZ, *LPNET_CMU_OPER_CTRL_PTZ;

typedef struct								// 设备删除云台预置点
{
	int  ptz_id;							// 云台标识
	int  preset_index;						// 预置点索引
	char preset_name[16];					// 预置点名称
}NET_CMU_OPER_DEL_PRESET, *LPNET_CMU_OPER_DEL_PRESET;

typedef struct								// 设备远程控制
{
	int control_action;						// 0 - reboot  1 - shutdown		
}NET_CMU_OPER_CTRL_DEV, *LPNET_CMU_OPER_CTRL_DEV;

typedef struct								// 设备通明通道
{
	char user_name[16];                    // 用户名 	
}NET_CMU_OPER_TRANS_CHANNEL, *LPNET_CMU_OPER_TRANS_CHANNEL;

/*******************************************************************************/
// upload
/*******************************************************************************/
typedef union
{
	char* stringValue;
	int   integerValue;
	/*
	当为DISK_FULL告警时,值为"磁盘路径:磁盘使用率"
	当GPIN告警时,此项值为通道号
	当CAM告警时,此项值为videoId
	当VS_UPGRADE告警时，此项值为"userId, 升级前的软件版本， 升级后的软件版本号，升级结果码" 其中升级结果码（0. 成功，1超时，2升级文件语言版本不匹配，3获取升级文件失败，4未知）
	当告警为智能分析告警时候，值为通道号
	*/		
}NET_CMU_ALARM_DATA, *LPNET_CMU_ALARM_DATA;

typedef struct								// 设备告警上报
{
	char pu_id[32];   //视频服务器的id
	char pu_ip[16];   //cmu的ip
	int  alarm_type;		//告警类型
								/*	
								VS_DISKERROR：硬盘错误
								VS_DISKFULL：硬盘满
								VS_DISCONN：前端设备断开
								VS_UPGRADE：前端设备升级
								VS_GPIN：开关量告警
								CAM_VIDEOLOSE：视频丢失
								CAM_HIDEDETECT：遮挡告警
								CAM_MOTIONDETECT：移动侦测

								以下为智能分析
								VCA_TRAVERSE_PLANE : 跨越警戒线
								VCA_ENTER_AREA: 进入区域
								VCA_EXIT_AREA: 离开区域
								VCA_INTRUSION: 入侵 
								VCA_LOITER: 徘徊
								VCA_LEFT_TAKE: 丢包捡包
								VCA_PARKING: 停车
								VCA_RUN: 奔跑
								VCA_HIGH_DENSITY: 人员密度
								*/				  
	char server_type[32];		//VS：视频服务器 （包括DVS/DVR的开关量告警） 	CAM：摄像机  （包括移动侦测，遮挡等图像告警,智能分析）
	int  disk_number;			//发生告警的硬盘号
	char time_stamp[32];			//包括日期和时间，格式为:	YYYY-MM-DD hh:mm:ss
	NET_CMU_ALARM_DATA data;
	char eliminated[8];			//true:警报消除 false：警报发生,缺省值	
	unsigned int select_platform;//平台选择
}NET_CMU_UPLOAD_ALARM, *LPNET_CMU_UPLOAD_ALARM;

typedef struct								// 设备数字量上报
{
	char pu_id[32];
	char pu_ip[16];
	char time_stamp[32];	//包括日期和时间，格式为:YYYY-MM-DD hh:mm:ss		
	int  digital_in_id;		//温湿度设备ID
	char value[32];			//温度、湿度等数字量
}NET_CMU_UPLOAD_DIGITAL, *LPNET_CMU_UPLOAD_DIGITAL;

typedef struct{
          char pu_id[32];
          int startAlarmPriority;
          int endAlarmPriority;
          int alarmMethod;
          NET_CMU_DEV_TIME startTime;
          NET_CMU_DEV_TIME endTime;
}NET_CMU_QUERY_AlARM;

typedef struct								// 设备远程控制
{
	char pu_id[32];
	int control_action;						//1//1:报警复位	
}NET_CMU_OPER_ALARMREST_DEV, *LPNET_CMU_OPER_ALARMREST_DEV;


typedef struct								// 设备远程控制
{
    char pu_id[32];
	int control_action;						// 0 - 撤防  1 - 布防		
}NET_CMU_OPER_GUARD_DEV, *LPNET_CMU_OPER_GUARD_DEV;
enum
{
    PU_DUPLICATE_REGISTER,//重复注册
    PU_REGISTER_FAILD,//向SERVER注册失败
    PU_KEEPALIVE_FAILD,//向SERVER发送心跳失败
    PU_INIT_FAILED//SERVER通讯模块初始化失败
    
};
#endif


