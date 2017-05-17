#include "ms_cmu_oper.h"
#include "ms_cmu_cmd.h"
#include "ms_pu_param.h"
#include "ms_pu_type.h"
#include "ms_mdu.h"
#include "ms_mdu_stream.h"

#include "stdafx.h"
#include "ms_pu_error.h"

#include "oslib.h"
#include "ms_pu_glob.h"
#include "mxml.h"
#include "md5.h"
#include "XmlLntercept.h"
#include "ms_mdu_stream_pool.h"
#include <string.h>

extern char Replacement_parts_preset[128];

int prase_oper_del_user_pack(char *buf, int result,   LPNET_CMU_DEL_USER lp_del_user);
int build_oper_del_user_pack(char *buf, int result,   LPNET_CMU_DEL_USER lp_del_user);

int prase_oper_clean_storage_pack(char *buf, int result,   LPNET_CMU_CLEAN_STORE_FILE lp_clean_storage);
int build_oper_clean_storage_pack(char *buf, int result,   LPNET_CMU_CLEAN_STORE_FILE lp_clean_storage);

int prase_oper_query_storage_pack(char *buf, int result,   LPNET_CMU_QUERY_STORE_FILE lp_query_storage);
int build_oper_query_storage_pack(char *buf, int result,   LPNET_CMU_QUERY_STORE_FILE lp_query_storage);

int prase_oper_ctrl_ptz_pack(char *buf, int result,			LPNET_CMU_OPER_CTRL_PTZ lp_ctrl_ptz);
int build_oper_ctrl_ptz_pack(char *buf, int result,			LPNET_CMU_OPER_CTRL_PTZ lp_ctrl_ptz);

int prase_oper_del_preset_pack(char *buf, int result,		LPNET_CMU_OPER_DEL_PRESET lp_del_preset);
int build_oper_del_preset_pack(char *buf, int result,		LPNET_CMU_OPER_DEL_PRESET lp_del_preset);

int prase_oper_ctrl_dev_pack(char *buf, int result,			LPNET_CMU_OPER_CTRL_DEV lp_ctrl_dev);
int build_oper_ctrl_dev_pack(char *buf, int result,			LPNET_CMU_OPER_CTRL_DEV lp_ctrl_dev);

int prase_oper_trans_channel_pack(char *buf, int result,   LPNET_CMU_OPER_TRANS_CHANNEL lp_trans_channel);
int build_oper_trans_channel_pack(char *buf, int result,   LPNET_CMU_OPER_TRANS_CHANNEL lp_trans_channel);

int prase_upload_alarm_pack(char *buf, int result,		LPNET_CMU_UPLOAD_ALARM lp_upload_alarm);
int prase_upload_digital_pack(char *buf, int result,	LPNET_CMU_UPLOAD_DIGITAL lp_upload_digital);
int prase_oper_control_file_playpack(char *buf, int result, LPNET_MDU_START_MEDIA media_oper);
int handle_upload_alarm(char* buf, unsigned long len);

int handle_ptz_oper_msg( char* buf, int len){

	unsigned long size = 0;
	int  result = 0;

	NET_CMU_OPER_CTRL_PTZ ctrl_ptz;
	if(prase_oper_ctrl_ptz_pack(buf, result,  &ctrl_ptz) == 0)
	{
		if(g_msg_proc)
		  g_msg_proc(OPER_PU_CONTROLPTZ, (char*)&ctrl_ptz, &size, g_param);

		return 0;

	}					
	return -1;

}

int handle_contrl_oper_msg( int cmd ){


	unsigned long size = 0;
	//int  result = 0;

	NET_CMU_OPER_CTRL_DEV ctrl_dev;

	ctrl_dev.control_action = cmd;//0为重启
    //printf("ctrl_dev.control_action+++++++   111111   %d\n",cmd);
	if(g_msg_proc)
	{
        //printf("ctrl_dev.control_action+++++++  2222222:%d\n",cmd);
        g_msg_proc(OPER_PU_CONTROLPU, (char*)&ctrl_dev, &size, g_param);
    }
      
	  

	return 0;


}
//此处为了与GB28181的接口进行融合，加入了此接口转换，主要是铭眼的结构转换为
//GB28181的结构
float get_control_play_back(LPNET_MDU_START_MEDIA media_oper)
{
        float aa;
    	if(strcmp(media_oper->cmd,"RATE")==0)//速率
		{
        
			    int rate=atoi(media_oper->param);
				switch(rate)
				{
					case 1:
						aa=2.0;
					break;
					case 2:
						aa=4.0;
					break;
					case 3:
						aa=6.0;
					break;
					case 4:
						aa=8.0;
					break;
					case 0:
						aa=1.0;
					break;
					case -1:
						aa=0.5;
					break;
					case -2:
						aa=0.25;
					break;
					case -3:
						aa=0.125;
					break;
					case -4:
						aa=0.0625;
					break;
				};
                
			return  aa;
		
	}
	else if(!strcmp(media_oper->cmd,"STOP"))//停止
		{
	        //停止视频暂时没有实现
		}
	else if(!strcmp(media_oper->cmd,"SEEK"))//
		{

		    return 1.0;
		}
	else if(!strcmp(media_oper->cmd,"RESTART"))//播放
		{
            return 1.0;
		}
	else if(!strcmp(media_oper->cmd,"PAUSE"))//暂停
		{
			return 0;
		}
}
int handle_oper_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len)
{
	int ret = -1, result = 0;
	unsigned long size = 0;

	switch(cmd_id)
	{
       
		case CMD_OPER_DELUSER:						// 删除设备用户信息
			{
				NET_CMU_DEL_USER del_user;
				if(prase_oper_del_user_pack(buf, result,  &del_user) == 0)
				{
					if(g_msg_proc)
					  g_msg_proc(OPER_PU_DEL_USER, (char*)&del_user, &size, g_param);

					ret = build_oper_del_user_pack(buf, result,  &del_user);
				}					
				break;
			}
		case CMD_OPER_CLEANPULOCALSTORAGEFILE:		// 删除设备录像文件
			{
				NET_CMU_CLEAN_STORE_FILE clean_storage;
				if(prase_oper_clean_storage_pack(buf, result,  &clean_storage) == 0)
				{
					if(g_msg_proc)
					  g_msg_proc(OPER_PU_CLEANPULOCALSTORAGEFILE, (char*)&clean_storage, &size, g_param);
					ret = build_oper_clean_storage_pack(buf, result,  &clean_storage);
				}					
				break;
			}

		case CMD_OPER_QUERYPULOCALSTORAGEFILES:		// 查询设备录像文件
			{
                
				NET_CMU_QUERY_STORE_FILE query_storage;
				memset(&query_storage,0,sizeof(NET_CMU_QUERY_STORE_FILE));
                //strcpy
				if(prase_oper_query_storage_pack(buf, result,  &query_storage) == 0)
				{
					//get_param_from_PU(&query_storage);
					if(g_msg_proc)
					  result  = g_msg_proc(OPER_PU_QUERYPULOCALSTORAGEFILES, (char*)&query_storage, &size, g_param);

					printf("\n====query_storage.total_size value %ld \n",query_storage.total_size);
					printf("\n====query_storage.total_size value %ld \n",query_storage.total_size);
					printf("\n====query_storage.total_size value %ld \n",query_storage.total_size);
                    query_storage.record_file_size = 0;   
                    query_storage.record_file_size = 400+500*query_storage.file_list_cnt;
                    //printf("query_storage %d \n",strlen(&query_storage));
                    char *buffer = (char *)os_sys_malloc( query_storage.record_file_size, __FILE__, __LINE__ );
                    memset(buffer,0,query_storage.record_file_size);
					//memset(buffer,0,MAX_BUFFER_LEN*2);
					ret = build_oper_query_storage_pack(buffer, result, &query_storage);
					//memset(buf,0,MAX_BUFFER_LEN);
                    //ret = build_oper_query_storage_pack(buf, result, &query_storage);

					if(query_storage.record_file_lists){
						free(query_storage.record_file_lists);
						query_storage.record_file_lists = NULL;
					}
                    
                	if(ret == 0)
                	{
                		sock_header resp;
                		memset(&resp,0,sizeof(resp));

                		resp.cseq   = cseq;
                		resp.ver    = htons(0x0100);
                		resp.length = htons((unsigned short)strlen(buffer));
                        printf("Send to Cmu:%s\n",buffer);
                        printf("_+_++%d\n",strlen(buffer));
                		if( os_socket_send(skt, &resp, sizeof(resp), 1000) < 0)
                		{
                			LOG("cmu get_info send info header fail.errno:%d,%s",errno,strerror(errno));
                            if(buffer != NULL)
                            {
                                free(buffer);
                                printf("free");
                                buffer = NULL;
                            }
                			return -1;
                		}
                		else if(os_socket_send(skt, buffer,  strlen(buffer),1000) <0)
                		{
                			LOG("cmu get_info send info body xml fail.errno:%d,%s",errno,strerror(errno));
                            if(buffer != NULL)
                            {
                                free(buffer);
                                printf("free");
                                buffer = NULL;
                            }
                			return -1;
                		}
                	}
                    
#if 1
	                //printf("buffer = %s \n".buffer);
                    if(buffer != NULL)
                    {
                        free(buffer);
                        printf("free");
                        buffer = NULL;
                   }
#endif
                    return 0;
				}					
				break;
			}

		case CMD_OPER_CONTROLPTZ:					// 设备云镜控制请求
			{
				NET_CMU_OPER_CTRL_PTZ ctrl_ptz;
				if(prase_oper_ctrl_ptz_pack(buf, result,  &ctrl_ptz) == 0)
				{
					if(g_msg_proc)
					  g_msg_proc(OPER_PU_CONTROLPTZ, (char*)&ctrl_ptz, &size, g_param);
					ret = build_oper_ctrl_ptz_pack(buf, result,  &ctrl_ptz);
				}					
				break;
			}

		case CMD_OPER_DELPRESETPTZ:					// 设备删除云台预置点
			{
				NET_CMU_OPER_DEL_PRESET del_preset;
				if(prase_oper_del_preset_pack(buf, result,  &del_preset) == 0)
				{
					if(g_msg_proc)
					  g_msg_proc(OPER_PU_DELPRESETPTZ, (char*)&del_preset, &size, g_param);
					ret = build_oper_del_preset_pack(buf, result,  &del_preset);
				}					
				break;
			}
		case CMD_OPER_CONTROLPU:					// 设备远程控制
			{
				NET_CMU_OPER_CTRL_DEV ctrl_dev;
				if(prase_oper_ctrl_dev_pack(buf, result,  &ctrl_dev) == 0)
				{
					if(g_msg_proc)
					  g_msg_proc(OPER_PU_CONTROLPU, (char*)&ctrl_dev, &size, g_param);
					ret = build_oper_ctrl_dev_pack(buf, result,  &ctrl_dev);
				}					
				break;
			}
		case CMD_OPER_TRANSPARENTCHANNEL:			// 设备通明通道
			{
				NET_CMU_OPER_TRANS_CHANNEL trans_channel;
				if(prase_oper_trans_channel_pack(buf, result,  &trans_channel) == 0)
				{
					//get_param_from_PU(&trans_channel);
					ret = build_oper_trans_channel_pack(buf, result,  &trans_channel);
				}					
				break;
			}
         case CMD_OPER_CONTROLFILEBACK:
			{
                NET_MDU_START_MEDIA media_oper;
                
                prase_oper_control_file_playpack(buf,result,&media_oper);
                void *stream=NULL;
                media_oper.request_type=REQUEST_FILE_STREAM;
				stream = mdu_stream_pool_find(stream_pool, &media_oper, 1000);
                if(stream==NULL)
                {
                    return 0;
                }
       //由于CU控制的时候,暂停然后再开始，保存有原来的速度值，
       //此处首先记录原来的速度值，然后，播放后，按照cu显示的速度播放  
                 if(strcmp(media_oper.cmd,"RATE")==0)
                 {  
                    ((stream_info_t *)stream)->lp_media->scale=get_control_play_back(&media_oper);
                 }  
                media_oper.scale=get_control_play_back(&media_oper);

                if(strcmp(media_oper.cmd,"RESTART")==0)
                {
                    media_oper.scale=((stream_info_t *)stream)->lp_media->scale;
                }
       //回调给pu时，是按照GB28181的接口形式
                if(g_stream_proc)
					  g_stream_proc((uint32_t)stream,"Control_MEDIA", (char*)&media_oper, size, g_param);

                break;
			}
		default:
			break;
	}

	if(ret == 0)
	{
		sock_header resp;
		memset(&resp,0,sizeof(resp));

		resp.cseq   = cseq;
		resp.ver    = htons(0x0100);
		resp.length = htons((unsigned short)strlen(buf));
        printf("Send to Cmu:%s\n",buf);
		if( os_socket_send(skt, &resp, sizeof(resp), 1000) < 0)
		{
			LOG("cmu get_info send info header fail.errno:%d,%s",errno,strerror(errno));
			return -1;
		}
		else if(os_socket_send(skt, buf,  strlen(buf), 1000) <0)
		{
			LOG("cmu get_info send info body xml fail.errno:%d,%s",errno,strerror(errno));
			return -1;
		}
	}

	return 0;
}

int handle_upload_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len)
{
	int result = 0;
	//int ret = -1;

	switch(cmd_id)
	{
		case CMD_UPLOAD_RAISEALARM:					// 设备告警上报
			{
				NET_CMU_UPLOAD_ALARM upload_alarm;
				if(prase_upload_alarm_pack(buf, result,  &upload_alarm) == 0)
				{
					//get_param_from_PU(&upload_alarm);
					//ret = build_oper_trans_channel_pack(buf, result,  &upload_alarm);
				}					
				break;
			}
		case CMD_UPLOAD_RAISEDIGITAL:				// 设备数字量上报
			{
				NET_CMU_UPLOAD_DIGITAL upload_digital;
				if(prase_upload_digital_pack(buf, result,  &upload_digital) == 0)
				{
					//get_param_from_PU(&upload_digital);
					//ret = build_oper_trans_channel_pack(buf, result,  &upload_digital);
				}					
				break;
			}
		default:
			break;
	}	

	return 0;
}


int handle_upload_alarm(char* buf, unsigned long len)
{

	LPNET_CMU_UPLOAD_ALARM  prt_param = (LPNET_CMU_UPLOAD_ALARM)buf;	

	//int flag = 0;	
	//int num = 0;
	//int alarmType = 0;
	
    os_mutex_lock(cmu_alarm->mutex,-1);

#if 0
    if(strstr(prt_param->eliminated, "false") )
    {
    	//zinuo_specific_start

    	if(prt_param->data.integerValue <= 0)
    	{
    		prt_param->data.integerValue = 1;
    	}
    	else if(prt_param->data.integerValue < 16 )
    	{
    		prt_param->data.integerValue+=1;
    	}
    	else
    	  prt_param->data.integerValue=16;
    	//zinuo_specific_end
    }
    else
    {
    	os_mutex_unlock(cmu_alarm->mutex);
    	return 0;
    }
#endif
    if(cmu_alarm == NULL)
    {
        printf("please register to cmu first\n");
        return -1;
    }
    if(cmu_alarm->sendsize+len > sizeof(cmu_alarm->sendbuf))
    {
    	cmu_alarm->sendsize = 0;
    	LOG("*********ALARM_PAKCET_QUEUE  OVER *********\n");
    }

    memcpy( cmu_alarm->sendbuf+cmu_alarm->sendsize,prt_param,len);

    cmu_alarm->sendsize += len;
    printf(" UPLOAD_ALARM TYPE %d    ALARM_PACLET  SIZE  %d\n",prt_param->alarm_type,cmu_alarm->sendsize);
    LOG(" UPLOAD_ALARM TYPE %d    ALARM_PACLET  SIZE  %d\n",prt_param->alarm_type,cmu_alarm->sendsize);
    
    os_mutex_unlock(cmu_alarm->mutex);
    return 0;
}

// 删除设备用户信息
int prase_oper_del_user_pack(char *buf, int result,   LPNET_CMU_DEL_USER lp_del_user)
{

	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *userName;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		userName= mxmlFindElement(parameters, xml, "username", NULL, NULL, MXML_DESCEND);
		if(userName != NULL && userName->type == MXML_ELEMENT)
		{
			pNode = userName->child;
			//printf("<<<  userName = %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_del_user->user_name, pNode->value.text.string,16);
		}

	}

	mxmlDelete(xml);
	return ret;

}

int build_oper_del_user_pack(char *buf, int result,   LPNET_CMU_DEL_USER lp_del_user)
{

	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_DEL_USER);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}


int prase_oper_clean_storage_pack(char *buf, int result,   LPNET_CMU_CLEAN_STORE_FILE lp_clean_storage)
{
	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *fileName,*videoId, *fromDate, *toDate;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		fileName= mxmlFindElement(parameters, xml, "fileName", NULL, NULL, MXML_DESCEND);
		if(fileName != NULL && fileName->type == MXML_ELEMENT)
		{
			pNode = fileName->child;
			//printf("<<<  fileName = %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_clean_storage->file_name, pNode->value.text.string,200);
		}

		videoId= mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			lp_clean_storage->video_id = atoi(pNode->value.text.string);
		}

		fromDate= mxmlFindElement(parameters, xml, "fromDate", NULL, NULL, MXML_DESCEND);
		if(fromDate != NULL && fromDate->type == MXML_ELEMENT)
		{
			pNode = fromDate->child;
			//printf("<<<  fromDate = %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_clean_storage->from_date, pNode->value.text.string,20);
		}

		toDate= mxmlFindElement(parameters, xml, "toDate", NULL, NULL, MXML_DESCEND);
		if(toDate != NULL && toDate->type == MXML_ELEMENT)
		{
			pNode = toDate->child;
			//printf("<<<  toDate = %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_clean_storage->to_date, pNode->value.text.string,20);
		}

	}

	mxmlDelete(xml);
	return ret;
}

int build_oper_clean_storage_pack(char *buf, int result,   LPNET_CMU_CLEAN_STORE_FILE lp_clean_storage)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_CLEANPULOCALSTORAGEFILE);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}



int prase_oper_query_storage_pack(char *buf, int result,   LPNET_CMU_QUERY_STORE_FILE lp_query_storage)
{
	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *fileType,*videoId, *fromDate, *toDate;

	char *xml_romDate = NULL ;
	char *xml_toDate = NULL ;

	xml_romDate = prase_oper_query_storage_pack_fromDate(buf);
	xml_toDate = prase_oper_query_storage_pack_toDate(buf);
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		videoId= mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			lp_query_storage->video_id = atoi(pNode->value.text.string);
		}

		fileType= mxmlFindElement(parameters, xml, "fileType", NULL, NULL, MXML_DESCEND);
		if(fileType != NULL && fileType->type == MXML_ELEMENT)
		{
			pNode = fileType->child;
			//printf("<<<  fileType = %s  >>>.\n", pNode->value.text.string);
			lp_query_storage->file_type= atoi(pNode->value.text.string);
		}

		fromDate= mxmlFindElement(parameters, xml, "fromDate", NULL, NULL, MXML_DESCEND);
		if(fromDate != NULL && fromDate->type == MXML_ELEMENT)
		{
			pNode = fromDate->child;
			//printf("<<<  fromDate = %s  >>>.\n", pNode->value.text.string);
			//strncpy(lp_query_storage->from_date, pNode->value.text.string,20);
		//	strncpy(lp_query_storage->from_date,	xml_romDate,19);


              sscanf(xml_romDate,"%04d-%02d-%02d %02d:%02d:%02d",
            &(lp_query_storage->from_date.year),
            &(lp_query_storage->from_date.month),
            &(lp_query_storage->from_date.date),
            &(lp_query_storage->from_date.hour),
            &(lp_query_storage->from_date.minute),
            &(lp_query_storage->from_date.second)
            );
			printf("<<<  fromDate = %s||  >>>.\n",xml_romDate);
		}

		toDate= mxmlFindElement(parameters, xml, "toDate", NULL, NULL, MXML_DESCEND);
		if(toDate != NULL && toDate->type == MXML_ELEMENT)
		{
			pNode = toDate->child;
			//printf("<<<  toDate = %s  >>>.\n", pNode->value.text.string);
			//strncpy(lp_query_storage->to_date, pNode->value.text.string,20);
			sscanf(xml_toDate,"%04d-%02d-%02d %02d:%02d:%02d",
            &(lp_query_storage->to_date.year),
            &(lp_query_storage->to_date.month),
            &(lp_query_storage->to_date.date),
            &(lp_query_storage->to_date.hour),
            &(lp_query_storage->to_date.minute),
            &(lp_query_storage->to_date.second));
			printf("<<<  toDate = %s||  >>>.\n", xml_toDate);
		}
        if(xml_toDate != NULL)
        {
            free(xml_toDate);
            xml_toDate = NULL;
        }

        if(xml_romDate!= NULL)
        {
            free(xml_romDate);
            xml_romDate = NULL;
        }
	}

	mxmlDelete(xml);
	return ret;
}
int build_oper_query_storage_pack(char *buf, int result,   LPNET_CMU_QUERY_STORE_FILE lp_query_storage)
{
	int i;
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *totalSize, *freeSize;
	mxml_node_t *totalNumber, *leaveNumber, *resultCode, *recordFile, *startTime, *filename, *stopTime,*size;
	char cValue[32] = "\0";
    char time[40];
    
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_QUERYPULOCALSTORAGEFILES);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	

	if(result == PU_SUCCESS){

		parameters = mxmlNewElement(response, "parameters");

		videoId = mxmlNewElement(parameters, "videoId");
		mxmlNewInteger(videoId, lp_query_storage->video_id);


		totalSize = mxmlNewElement(parameters, "totalSize");
		mxmlNewInteger(totalSize, lp_query_storage->total_size);


		freeSize = mxmlNewElement(parameters, "freeSize");
		mxmlNewInteger(freeSize, lp_query_storage->free_size);

		totalNumber = mxmlNewElement(parameters, "totalNumber");
		mxmlNewInteger(totalNumber, lp_query_storage->total_num);

		leaveNumber = mxmlNewElement(parameters, "leaveNumber");
		mxmlNewInteger(leaveNumber, lp_query_storage->leave_num);

		resultCode = mxmlNewElement(parameters, "result");
		mxmlNewInteger(resultCode, lp_query_storage->result_code);


		for(i = 0; i < lp_query_storage->file_list_cnt; i++)
		{ 
   
            
			recordFile = mxmlNewElement(parameters, "recordFile");

			filename = mxmlNewElement(recordFile, "filename");
			mxmlNewText(filename, 0, lp_query_storage->record_file_lists[i].file_name);	

            memset(time,0,sizeof(time));
            sprintf(time,"%04d-%02d-%02d %02d:%02d:%02d",
            lp_query_storage->record_file_lists[i].start_time.year,\
            lp_query_storage->record_file_lists[i].start_time.month,\
            lp_query_storage->record_file_lists[i].start_time.date,\
            lp_query_storage->record_file_lists[i].start_time.hour,\
            lp_query_storage->record_file_lists[i].start_time.minute,\
            lp_query_storage->record_file_lists[i].start_time.second);
            
			startTime = mxmlNewElement(recordFile, "startTime");
			mxmlNewText(startTime, 0,time);		

            
            memset(time,0,sizeof(time));
            sprintf(time,"%04d-%02d-%02d %02d:%02d:%02d",
            lp_query_storage->record_file_lists[i].stop_time.year,\
            lp_query_storage->record_file_lists[i].stop_time.month,\
            lp_query_storage->record_file_lists[i].stop_time.date,\
            lp_query_storage->record_file_lists[i].stop_time.hour,\
            lp_query_storage->record_file_lists[i].stop_time.minute,\
            lp_query_storage->record_file_lists[i].stop_time.second);
            
			stopTime = mxmlNewElement(recordFile, "stopTime");
			mxmlNewText(stopTime, 0, time);	

			size = mxmlNewElement(recordFile, "size");
			//mxmlNewInteger(size, lp_query_storage->record_file_lists[i].size);
			mxmlNewInteger(size, lp_query_storage->record_file_lists[i].size);

		}

	}

	mxmlSaveString(xml, buf,lp_query_storage->record_file_size, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;

}


int prase_oper_ctrl_ptz_pack(char *buf, int result,   LPNET_CMU_OPER_CTRL_PTZ lp_ctrl_ptz)
{
	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *ptzId,*cmd, *param, *speed;
	//int cnt = 0;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		ptzId= mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;
            if(pNode != NULL)
			//printf("<<<  ptzId = %s  >>>.\n", pNode->value.text.string);
			lp_ctrl_ptz->ptz_id = atoi(pNode->value.text.string);
		}

		cmd= mxmlFindElement(parameters, xml, "cmd", NULL, NULL, MXML_DESCEND);
		if(cmd != NULL && cmd->type == MXML_ELEMENT)
		{
			pNode = cmd->child;
            if(pNode != NULL)
			//printf("<<<  cmd = %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_ctrl_ptz->cmd,pNode->value.text.string,32);
		}

		param= mxmlFindElement(parameters, xml, "param", NULL, NULL, MXML_DESCEND);
		if(param != NULL && param->type == MXML_ELEMENT)
		{
			pNode = param->child;
            if(pNode != NULL)
			//printf("<<<  param= %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_ctrl_ptz->param,pNode->value.text.string,200);
		}


		speed= mxmlFindElement(parameters, xml, "speed", NULL, NULL, MXML_DESCEND);
		if(speed != NULL && speed->type == MXML_ELEMENT)
		{
			pNode = speed->child;
			//printf("<<<  speed = %s  >>>.\n", pNode->value.text.string);
			lp_ctrl_ptz->speed = atoi(pNode->value.text.string);
		}




	}

	mxmlDelete(xml);
	return ret;
}
int build_oper_ctrl_ptz_pack(char *buf, int result,   LPNET_CMU_OPER_CTRL_PTZ lp_ctrl_ptz)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_CONTROLPTZ);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

// 设备删除云台预置点
int prase_oper_del_preset_pack(char *buf, int result,   LPNET_CMU_OPER_DEL_PRESET lp_del_preset)
{
	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *ptzId,*presetIndex,*presetName;
	//int cnt = 0;
	char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	//char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
	sscanf(buf,"%[^>]",p) ;
	strcat(firstbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , firstbuf , 1024) ;
	char *tmp = NULL;

	tmp = Contrary_XmlLntercept(buf) ;

	xml = mxmlLoadString(NULL, tmp, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		ptzId= mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;
			//printf("<<<  ptzId = %s  >>>.\n", pNode->value.text.string);
			lp_del_preset->ptz_id = atoi(pNode->value.text.string);
		}

		presetIndex= mxmlFindElement(parameters, xml, "presetIndex", NULL, NULL, MXML_DESCEND);
		if(presetIndex != NULL && presetIndex->type == MXML_ELEMENT)
		{
			pNode = presetIndex->child;
			//printf("<<<  presetIndex = %s  >>>.\n", pNode->value.text.string);
			lp_del_preset->preset_index = atoi(pNode->value.text.string);
		}

		presetName= mxmlFindElement(parameters, xml, "presetName", NULL, NULL, MXML_DESCEND);
		if(presetName != NULL && presetName->type == MXML_ELEMENT)
		{
			pNode = presetName->child;
			//printf("<<<  presetName= %s  >>>.\n", pNode->value.text.string);
			strncpy(lp_del_preset->preset_name,Replacement_parts_preset,16);
			memset(Replacement_parts_preset , 0 , 128) ;
		}

	}
	return 0;
}
int build_oper_del_preset_pack(char *buf, int result,   LPNET_CMU_OPER_DEL_PRESET lp_del_preset)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_DELPRESETPTZ);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");


	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	sscanf(buf,"%[^>]",p) ;
	strcat(secondbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , secondbuf , 1024) ;	sscanf(buf,"%[^>]",p) ;
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

// 设备远程控制
int prase_oper_ctrl_dev_pack(char *buf, int result,   LPNET_CMU_OPER_CTRL_DEV lp_ctrl_dev)
{
	int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *controlAction;
	//int cnt = 0;


	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{

		ret = 0;
		controlAction= mxmlFindElement(parameters, xml, "controlAction", NULL, NULL, MXML_DESCEND);
		if(controlAction != NULL && controlAction->type == MXML_ELEMENT)
		{
			pNode = controlAction->child;
			//printf("<<<  controlAction = %s  >>>.\n", pNode->value.text.string);
			lp_ctrl_dev->control_action =  atoi(pNode->value.text.string);
            printf("prase_oper_ctrl_dev_packlp_ctrl_dev->control_action %d\n",lp_ctrl_dev->control_action);

		}
	}
	return 0;
}
int build_oper_ctrl_dev_pack(char *buf, int result,   LPNET_CMU_OPER_CTRL_DEV lp_ctrl_dev)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", OPER_PU_CONTROLPU);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	


	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

// 设备通明通道
int prase_oper_trans_channel_pack(char *buf, int result,   LPNET_CMU_OPER_TRANS_CHANNEL lp_trans_channel)
{

	int ret = -1;
	mxml_node_t	*xml, *parameters; //*pNode,*controlAction
	//int cnt = 0;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		/*
		   controlAction= mxmlFindElement(parameters, xml, "controlAction", NULL, NULL, MXML_DESCEND);
		   if(controlAction != NULL && controlAction->type == MXML_ELEMENT)
		   {
		   pNode = controlAction->child;
		   printf("<<<  controlAction = %s  >>>.\n", pNode->value.text.string);
		   lp_trans_channel =  atoi(pNode->value.text.string);
		   }
		   */
	}
	return 0;
}
int prase_oper_control_file_playpack(char *buf, int result, LPNET_MDU_START_MEDIA media_oper)
{
    int ret = -1;
	mxml_node_t	*xml,*pNode, *parameters, *ptzId,*presetIndex,*presetName,*param;
	//int cnt = 0;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	//char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	//char p[1024];
	//char FinalDest[1024] ;
/*	sscanf(buf,"%[^>]",p) ;
	strcat(firstbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , firstbuf , 1024) ;
	char *tmp = NULL;
	tmp = Contrary_XmlLntercept(buf) ;
*/
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		ptzId= mxmlFindElement(parameters, xml, "puId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;
            strcpy(media_oper->media.pu_id,pNode->value.text.string);
			//printf("<<<  puId = %s >>>.\n", pNode->value.text.string);

		}

		presetIndex= mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(presetIndex != NULL && presetIndex->type == MXML_ELEMENT)
		{
			pNode = presetIndex->child;
            media_oper->media.video_id=atoi(pNode->value.text.string);
		//	printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
		}

		presetName= mxmlFindElement(parameters, xml, "fileName", NULL, NULL, MXML_DESCEND);
		if(presetName != NULL && presetName->type == MXML_ELEMENT)
		{
			pNode = presetName->child;
		//	printf("<<<  fileName= %s  >>>.\n", pNode->value.text.string);
			strcpy(media_oper->fileName,pNode->value.text.string);
			memset(Replacement_parts_preset , 0 , 128) ;
		}
        param= mxmlFindElement(parameters, xml, "param", NULL, NULL, MXML_DESCEND);
		if(param != NULL && param->type == MXML_ELEMENT)
		{
			pNode = param->child;
		//	printf("<<<  scanf= %s  >>>.\n", pNode->value.text.string);
            strcpy(media_oper->param,pNode->value.text.string);
			memset(Replacement_parts_preset,0,128);
		}
        param= mxmlFindElement(parameters, xml, "cmd", NULL, NULL, MXML_DESCEND);
		if(param != NULL && param->type == MXML_ELEMENT)
		{
			pNode = param->child;
		//	printf("<<<  cmd= %s  >>>.\n", pNode->value.text.string);
            strcpy(media_oper->cmd,pNode->value.text.string);
			memset(Replacement_parts_preset,0,128);
		}

	}
	return 0;
}
int build_oper_trans_channel_pack(char *buf, int result,   LPNET_CMU_OPER_TRANS_CHANNEL lp_trans_channel)
{
	return 0;
}


int prase_upload_alarm_pack(char *buf, int result,	 LPNET_CMU_UPLOAD_ALARM lp_upload_alarm)
{
	return 0;
}

int prase_upload_digital_pack(char *buf, int result, LPNET_CMU_UPLOAD_DIGITAL lp_upload_digital)
{
	return 0;
}


