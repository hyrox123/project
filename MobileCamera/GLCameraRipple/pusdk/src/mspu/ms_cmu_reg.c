#include "stdafx.h"
#include "ms_cmu_reg.h"
#include "ms_cmu_cmd.h"
#include "ms_pu_glob.h"

#include "ms_pu_param.h"




#include "mxml.h"
#include "md5.h"
#include "oslib.h"

/***************************************************************/

/***************************************************************/


/***************************************************************/

/************************************************************************/
/*                                                                      */
/************************************************************************/
int cmu_register(int skt)//pu往cmu注册
{
	unsigned char passwd[64] = {0};
	char buf[MAX_BUFFER_LEN] = {0};//最大4个字节
	memset(buf,0,sizeof(buf));
	sock_header reg;	
/*typedef struct指令头
{
	unsigned short ver;
	unsigned short length;
	unsigned int   cseq;
}sock_header, *lp_sock_header;*/
	mxml_node_t	*xml, *request, *parameters, *puId, *puIp, *puSigPort, *puName, *puPassword,  *puType;
	
	xml = mxmlNewXML("1.0");
	request = mxmlNewElement(xml, "request");
	mxmlElementSetAttrf(request, "command", "PuRegister");

	parameters = mxmlNewElement(request, "parameters");

	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, ms_g_info.ms_info.pu_id);
	strcpy(cmu_info->pu_id, ms_g_info.ms_info.pu_id);
    //printf("******************************************************\n");
    //printf("[cmu_register] %s\n",ms_g_info.ms_info.pu_id);

	puIp = mxmlNewElement(parameters, "puIp");
	mxmlNewText(puIp, 0, cmu_skt->cli_ip);

	puSigPort = mxmlNewElement(parameters, "puSigPort");
	mxmlNewInteger(puSigPort, cmu_skt->cli_port);

	puName = mxmlNewElement(parameters, "puName");
	mxmlNewText(puName, 0, ms_g_info.ms_info.name);
   // printf("******************************************************\n");
    //printf("[cmu_register]name <<<<<<<%s>>>>>>>\n",ms_g_info.ms_info.name);
    
    //memset(ms_g_info.ms_info.passwd,0,sizeof(ms_g_info.ms_info.passwd));
    //sprintf(ms_g_info.ms_info.passwd,"\r");
    //printf("******************************************************\n");
    //printf("<<<<[cmu_register]passwd <<<<<<<%s>>>>>\n",ms_g_info.ms_info.passwd);
    memset(passwd,0,sizeof(passwd));
    //printf("******************************************************\n");
    //printf("[cmu_register]passwd length <<<<<<%d>>>>>>\n",strlen(ms_g_info.ms_info.passwd));
	mingsoft_md5_passwd(ms_g_info.ms_info.passwd, passwd,strlen((const char *)ms_g_info.ms_info.passwd));
	
  
	//MD5_Get32(passwd,ms_g_info.ms_info.passwd,strlen(ms_g_info.ms_info.passwd));
    
    printf("******************************************************\n");
   printf("[cmu_register] MD5 passwd <<<<<<<<%s>>>>>>>>\n",passwd);
    
	puPassword = mxmlNewElement(parameters, "puPassword");
	mxmlNewText(puPassword, 0, (const char *)passwd);

	puType = mxmlNewElement(parameters, "puType");
	mxmlNewInteger(puType,  ms_g_info.ms_info.pu_type);

/*	if(regParam.manufacturerId && regParam.manufacturerId[0])
	{
        mxml_node_t	*manufacturerId = mxmlNewElement(parameters, "manufacturerId");
        mxmlNewText(manufacturerId, 0, "xxxxx");
	}

	if(regParam.manufacturerName && regParam.manufacturerName[0])
	{
        mxml_node_t	*manufacturerName = mxmlNewElement(parameters, "manufacturerName");
        mxmlNewText(manufacturerName, 0, "xxxxx");
	}

	if(regParam.puType >= 0 && regParam.puType <= 3)
	{
        mxml_node_t	*puType = mxmlNewElement(parameters, "puType");
        mxmlNewText(puType, 0, "xxxxx");
	}

	if(regParam.productVersion && regParam.productVersion[0])
	{
        mxml_node_t	*productVersion = mxmlNewElement(parameters, "productVersion");
        mxmlNewText(productVersion, 0, "xxxxx");
	}

	if(regParam.lastUpgradeTime && regParam.lastUpgradeTime[0])
	{
        mxml_node_t	*lastUpgradeTime = mxmlNewElement(parameters, "lastUpgradeTime");
        mxmlNewText(lastUpgradeTime, 0, "xxxxx");
	}

	if(regParam.softwareVersion && regParam.softwareVersion[0])
	{
        mxml_node_t	*softwareVersion = mxmlNewElement(parameters, "softwareVersion");
        mxmlNewText(softwareVersion, 0, "xxxxx");
	}
*/
	

	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);

//	LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	memset(&reg,0,sizeof(reg));
	reg.cseq   = 0;
	reg.ver    = htons(0x0100);	//版本
	reg.length = htons((unsigned short)strlen(buf));

    if( os_socket_send(skt, &reg, sizeof(reg), 1000) < 0)
	{
		LOG("Register send info header fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}
	else if(os_socket_send(skt, buf,  strlen(buf), 1000) <0)
	{
		LOG("Register send info body xml fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}
    //printf("send register%s \n",buf);
	return 0;
}



int cmu_upLoad_alarm(int skt,char * prt)
{
	LPNET_CMU_UPLOAD_ALARM prt_param = (LPNET_CMU_UPLOAD_ALARM)prt;
	//int  count  = 0 ;
	int  void_count = 0;
	//char passwd[256] = {0};
	char buf[MAX_BUFFER_LEN] = {0};
	char type[25] = {0};
	sock_header reg;	
	mxml_node_t	*xml, *request, *parameters, *puId, *puIp, *alarmType,  *serverType, *diskNumber, *timeStamp, *data, *eliminated;
	
	xml = mxmlNewXML("1.0");
	request = mxmlNewElement(xml, "request");
	mxmlElementSetAttrf(request, "command", "RaiseAlarm");

	parameters = mxmlNewElement(request, "parameters");

	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, prt_param->pu_id);
	//strcpy(cmu_info->pu_id, ms_gb_g_info.ms_info.pu_id);

	puIp = mxmlNewElement(parameters, "puIp");
	mxmlNewText(puIp, 0, prt_param->pu_ip);
	//mxmlNewText(puIp, 0, "10.10.10.189");

/*
	puName = mxmlNewElement(parameters, "puName");
	mxmlNewText(puName, 0, ms_gb_g_info.ms_info.name);
*/
	if(prt_param->alarm_type ==CAM_MOTIONDETECT )
		strncpy(type,"CAM_MOTIONDETECT",25);
	else if(prt_param->alarm_type ==CAM_VIDEOLOSE )
		strncpy(type,"CAM_VIDEOLOSE",25);
	else if(prt_param->alarm_type ==VS_GPIN)
			strncpy(type,"VS_GPIN",25);
	else if(prt_param->alarm_type ==CAM_HIDEDETECT)
		    strncpy(type,"CAM_HIDEDETECT",25);
	else if(prt_param->alarm_type ==VS_DISKERROR)
		    strncpy(type,"VS_DISKERROR",25);
	else if(prt_param->alarm_type ==VS_DISKERROR)
		    strncpy(type,"VS_DISKFULL",25);


	void_count = prt_param->data.integerValue;
				
	alarmType = mxmlNewElement(parameters, "alarmType");
	mxmlNewText(alarmType, 0, type);

	serverType = mxmlNewElement(parameters, "serverType");
	mxmlNewText(serverType, 0, prt_param->server_type);

  
	diskNumber = mxmlNewElement(parameters, "diskNumber");
	mxmlNewInteger(diskNumber,  prt_param->disk_number);


	timeStamp = mxmlNewElement(parameters, "timeStamp");
	mxmlNewText(timeStamp, 0, prt_param->time_stamp);

	data = mxmlNewElement(parameters, "data");
	mxmlNewInteger(data,  void_count);

	eliminated = mxmlNewElement(parameters, "eliminated");
	mxmlNewText(eliminated, 0, prt_param->eliminated);
    

	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);

// msg head utf-8 trans gb2312

	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
    char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
   	sscanf(buf,"%[^>]",p) ;
    strcat(secondbuf, &buf[strlen(p)+1]) ;
   	memcpy(buf , secondbuf , 1024) ;


	LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	memset(&reg,0,sizeof(reg));
	reg.cseq   = 0;
	reg.ver    = htons(0x0100);	
	reg.length = htons((unsigned short)strlen(buf));
    //printf("Send buf is :%s\n",buf);
    if( os_socket_send(skt, &reg, sizeof(reg), 1000) < 0)
	{
		LOG("Register send info header fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}
	else if(os_socket_send(skt, buf,  strlen(buf), 1000) <0)
	{
		LOG("Register send info body xml fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}
	return 0;
}

int cmu_keepalive(int skt)
{
	char buf[MAX_BUFFER_LEN] = {0};
    sock_header reg;
	mxml_node_t	*xml, *request, *parameters, *puId, *puName, *keepAlivePeriod;

	xml = mxmlNewXML("1.0");
	request = mxmlNewElement(xml, "request");
	mxmlElementSetAttrf(request, "command", "PuKeepAlive");

	parameters = mxmlNewElement(request, "parameters");

	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, cmu_info->pu_id);

	puName = mxmlNewElement(parameters, "puName");
	mxmlNewText(puName, 0, ms_g_info.ms_info.name);
	
	keepAlivePeriod = mxmlNewElement(parameters, "keepAlivePeriod");
	mxmlNewInteger(keepAlivePeriod, cmu_info->kpl_period);
	

	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);

	memset(&reg,0,sizeof(reg));
	reg.cseq=0;
	reg.ver=htons(0x0100);
	reg.length=htons((unsigned short)strlen(buf));

	if( os_socket_send(skt, &reg, sizeof(reg), 1000) < 0)
	{
		LOG("Keepalive send info header fail.errno:%d,%s",errno,strerror(errno));
        printf("Keepalive send info header fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}
	else if(os_socket_send(skt, buf,  strlen(buf), 1000) <0)
	{
		LOG("Keepalive send info body xml fail.errno:%d,%s",errno,strerror(errno));
        printf("Keepalive send info body xml fail.errno:%d,%s",errno,strerror(errno));
		return -1;
	}

    //printf("send keepalive%s \n",buf);
	return 0;
}

int prase_cmu_reg_msg(int skt, char* buf, int len)
{
    int ret = -1;
	const char* code = NULL;
	mxml_node_t	*xml, *result_code, *parameters, *kpl_period, *mdu_ip, *mdu_port, *pNode;

	xml = mxmlLoadString(NULL, buf, NULL);
	result_code = mxmlFindElement(xml, xml, "result", "code", NULL, MXML_DESCEND);
	if(result_code != NULL)
	{
		code = mxmlElementGetAttr(result_code, "code");
	    //printf("<<<  result code = [%s]  >>>.\n", code);
		if(atoi(code) == 0 )
		{
		    ret = 0;

			parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
			if(parameters != NULL)
			{
				kpl_period = mxmlFindElement(parameters, xml, "puKeepAlivePeriod", NULL, NULL, MXML_DESCEND);
				if(kpl_period != NULL && kpl_period->type == MXML_ELEMENT)
				{
					pNode = kpl_period->child;
//					printf("<<<  kpl period = %s  >>>.\n", pNode->value.text.string);

					cmu_info->kpl_period = atoi(pNode->value.text.string);
				}

				mdu_ip = mxmlFindElement(parameters, xml, "sipRegIPAddr", NULL, NULL, MXML_DESCEND);
				if(mdu_ip != NULL && mdu_ip->type == MXML_ELEMENT)
				{
					pNode = mdu_ip->child;
//					printf("<<<  mdu-ip = %s  >>>.\n", pNode->value.text.string);
					strcpy(cmu_info->sip_ip, pNode->value.text.string);
				}

                mdu_port = mxmlFindElement(parameters, xml, "sipRegPort", NULL, NULL, MXML_DESCEND);
				if(mdu_port != NULL && mdu_port->type == MXML_ELEMENT)
				{
					pNode = mdu_port->child;
//					printf("<<<  mdu-port = %s  >>>.\n", pNode->value.text.string);
					cmu_info->sip_port = atoi(pNode->value.text.string);
				}
			}
		}
        else
        {
            LOG("pu register to cmu failed\n");
            net_error_num = PU_REGISTER_FAILD;
        }
	}

	mxmlDelete(xml);
	return ret;
}



int find_xml_time(const char **buf,char *info)
{
    NET_CMU_DEV_TIME * basic_info = (NET_CMU_DEV_TIME *)info;
	char *a=NULL;
	char *b=NULL;

    char time[25];

	char s1[25];
	
	int count=0;
/*截取currenttime*/
	a=strstr(*buf,"<currentTime>");

	a=a+13;

	b=strstr(*buf,"</currentTime>");

	b=b;

	count=b-a;

	if(count >25)//字符串长度太大，可能xml格式不对
		return 0;
	
	memset(time,0,sizeof(time));

	strncpy(time,a,count);

/*截取年*/
	a=strstr(time,"-");


	count=a-time;

	memset(s1,0,sizeof(s1));

	strncpy(s1,time,count);


	basic_info->year=atoi(s1);

/*截取月*/
	a=a+1;

	strcpy(s1,a);

	a=strstr(s1,"-");
	count=a-s1;
	memset(time,0,sizeof(time));
	strncpy(time,s1,count);
	
	basic_info->month=atoi(time);

/*截取日2012-03-30 15:39:00*/
	a=a+1;
	strcpy(time,a);
	a=strstr(time," ");
	count=a-time;
	memset(s1,0,sizeof(s1));
	strncpy(s1,time,count);

	basic_info->date=atoi(s1);


/*时*/

	a=a+1;
	strcpy(s1,a);
	a=strstr(s1,":");
	count=a-s1;
	memset(time,0,sizeof(time));
	strncpy(time,s1,count);
	
	basic_info->hour=atoi(time);
	

/*分*/
	a=a+1;
	strcpy(time,a);
	a=strstr(time,":");
	count=a-time;
	memset(s1,0,sizeof(s1));
	strncpy(s1,time,count);

	basic_info->minute=atoi(s1);


/*秒*/
 
	a=a+1;
	strcpy(s1,a);
	basic_info->second=atoi(s1);

return 0;
	
}



int prase_cmu_kpl_msg(int skt, char* buf, int len, char * info)
{
    NET_CMU_DEV_TIME *basic_info = (NET_CMU_DEV_TIME*)info;
    int ret = -1;
	const char* code = NULL;
	mxml_node_t *xml, *result_code, *parameters, *current_Time, *pNode;
	//char str_time[30];
	
	xml = mxmlLoadString(NULL, buf, NULL);

	result_code = mxmlFindElement(xml, xml, "result", "code", NULL, MXML_DESCEND);
		
	if(result_code != NULL)
	{
		code = mxmlElementGetAttr(result_code, "code");
		//printf("<<<  result code = [%s]  >>>.\n", code);
		if(atoi(code) == 0 )
		{
			ret = 0;
		
			parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
			if(parameters != NULL)
			{
				current_Time = mxmlFindElement(parameters, xml, "currentTime", NULL, NULL, MXML_DESCEND);
				if(current_Time != NULL && current_Time->type == MXML_ELEMENT)
						{
							pNode = current_Time->child;

							if(pNode  != NULL){

								if(find_xml_time((const char **)&buf,(char *)&basic_info)<0)
                                 {
                                    printf("find xml time Error\n");
									mxmlDelete(xml);
									return -1;
									
                                 }
								
								//printf("<<< current_Time = %s  >>>.\n", pNode->value.text.string);
								//trcpy(str_time, pNode->value.text.string);

								}
							}

					}
				}
			}
    else
    {
        LOG("pu keepalive to cmu failed\n");
        net_error_num = PU_KEEPALIVE_FAILD;
    }



	mxmlDelete(xml);

	return ret;
}

int handle_reg_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len)
{
	switch(cmd_id)
	{
	case CMD_REG_REGISTER:
		{
			if(prase_cmu_reg_msg(skt, buf, len) == 0)
			{
                printf("Register Ming Soft Successfully\n");
				cmu_info->is_reg = REGISTER_ON;
				cmu_info->cmd =+1;
				cmu_keepalive(skt);
			}
			break;
		}
	case CMD_REG_KEEPALIVE:
		{
			NET_CMU_DEV_TIME basic;
			unsigned long size = 0;
            //printf("Keepalive Ming Soft Successfully\n");
			memset(&basic, 0, sizeof(NET_CMU_DEV_TIME));
            #if 1
			if(cmu_info->kpl_timing_count > 250)
            {				
				if(prase_cmu_kpl_msg(skt, buf, len,(char *)&basic) == 0)
				{
					if(g_msg_proc)
						g_msg_proc(SET_PU_TIME, (char*)&basic, &size, g_param);
						printf("<<<  SET_PU_TIME success ooxxx  >>>.\n");
				}
				cmu_info->kpl_timing_count = 0;
			}
            #endif
			break;
		}
	default:
		break;
	}
	
	return 0;
}


