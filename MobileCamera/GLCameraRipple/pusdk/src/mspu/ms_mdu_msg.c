#include "stdafx.h"
#include "ms_mdu_msg.h"
#include "ms_pu_glob.h"
#include "ms_pu_param.h"
#include "mxml.h"
#include "oslib.h"

long g_seq = 0;

int parse_reg_packet(const char* sip_packet)
{
	if( strstr(sip_packet, "REGISTER") && strstr(sip_packet, "OK") )
	{
		return 0;
	}

	return -1;
}

int build_kpl_packet(char *sip_packet)
{
	int ret = 0;
	if( !sip_packet )
		return -1;

	ret = sprintf(sip_packet, "OPTIONS sip:%s SIP/2.0\r\n"\
			"To: %s\r\n"\
			"From: %s\r\n"\
			"CSeq: %ld OPTIONS\r\n"
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

	return ret;
}

int parse_kpl_packet(const char* sip_packet)
{
	if((strstr(sip_packet, "OPTIONS"))&&(strstr(sip_packet, "OK")))
	{
		return 0;
	}

	return -1;
}

int parse_invite_packet(const char* sip_packet, LPNET_MDU_START_MEDIA start_media)
{
	int ret = -1;
	int iLength = 0;
	const char *starOpt = NULL;
	const char *stopOpt = NULL;
	char cLength[8] = {0}, cPuId[32] = {0}, xml[1024] = {0};
	mxml_node_t	*xml_node, *parameters, *pu_id, *pNode, *video_id, *media_type, *stream_type;
	mxml_node_t	*link_mode, *target_ip, *target_port, *file_name,  *start_time, *end_time,*playtype;	
    int playType = -1;
    
    starOpt = strstr(sip_packet, "<startTime>");
    starOpt += strlen("<startTime>");
    stopOpt = strstr(sip_packet, "</startTime>");
    char start_time_buf[40] = {0};
    iLength = (int)(stopOpt - starOpt);
   
    if(starOpt != NULL && stopOpt != NULL && iLength > 0)
        strncpy(start_time_buf,starOpt,iLength);
    //printf("+++++++++++++++++++++++++++++++++%s\n",start_time_buf);
    starOpt= NULL;
    stopOpt =NULL;
    iLength = 0;
    starOpt = strstr(sip_packet, "<stopTime>");
    starOpt += strlen("<stopTime>");
    stopOpt = strstr(sip_packet, "</stopTime>");
    char end_time_buf[40] = {0};
    iLength = (int)(stopOpt - starOpt);
   
    if(starOpt != NULL && stopOpt != NULL && iLength > 0)
        strncpy(end_time_buf,starOpt,iLength);
    //printf("+++++++++++++++++++++++++++++++++%s\n",end_time_buf);
    starOpt= NULL;
    stopOpt =NULL;
    iLength = 0;
	if((!strstr(sip_packet, "INVITE"))||(!strstr(sip_packet, "SIP")))
	{
		return -1;
	}

	starOpt = strstr(sip_packet, "To:");
	if(starOpt==NULL)
	{
		return -1;
	}

	starOpt += strlen("To:");
	while(*starOpt==' ')
	{
		starOpt++;
	}

	stopOpt = strstr(starOpt, "\r\n");
	if(stopOpt == NULL)
	{
		return -1;
	}

	iLength = (int)(stopOpt - starOpt);
	if(iLength <= 32)
	{
		strncpy(cPuId, starOpt, iLength);
	}
	else
	{
		return -1;
	}

	if(strncmp((char*)cmu_info->pu_id, cPuId, strlen(cPuId)) != 0)
	{
		return -1;
	}

	starOpt = strstr(sip_packet, "length:");
	if(starOpt==NULL)
			starOpt = strstr(sip_packet, "Length:");
	
	if(starOpt == NULL)
	{
		return -1;
	}

	starOpt += strlen("length:");
	while(*starOpt==' ')
	{
		starOpt++;
	}

	stopOpt = strstr(starOpt, "\r\n");
	if(starOpt == NULL)
	{
		return -1;
	}

	iLength = (int)(stopOpt - starOpt);
	if(iLength <= 8)
	{
		strncpy(cLength, starOpt, iLength);
	}
	else
	{
		return -1;
	}

	iLength = atoi(cLength);
	if(iLength > 1024)
	{
		return -1;
	}

	starOpt = strstr(sip_packet, "<?xml");
	if(starOpt == NULL)
	{
		return -1;
	}
	strncpy(xml, starOpt, iLength);


//	LOG("xxxxxxxxx---prase xml info- [%s]\n", xml);

//printf("xxxxxxxxx---prase xml info- [%s]\n", xml);

	xml_node = mxmlLoadString(NULL, xml, NULL);

	parameters = mxmlFindElement(xml_node, xml_node, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		pu_id = mxmlFindElement(parameters, xml_node, "puId", NULL, NULL, MXML_DESCEND);
		if(pu_id != NULL && pu_id->type == MXML_ELEMENT)
		{
			pNode = pu_id->child;
            if( pNode != NULL)
		//	printf("<<<  puId = %s  >>>.\n", pNode->value.text.string);
			strncpy(start_media->media.pu_id, pNode->value.text.string,sizeof(start_media->media.pu_id));
		}

		video_id = mxmlFindElement(parameters, xml_node, "videoId", NULL, NULL, MXML_DESCEND);
		if(video_id != NULL && video_id->type == MXML_ELEMENT)
		{
			pNode = video_id->child;
            if( pNode != NULL)
			//printf("<<<  video id = %s  >>>.\n", pNode->value.text.string);
			    start_media->media.video_id = atoi(pNode->value.text.string);
		}

		media_type = mxmlFindElement(parameters, xml_node, "mediaType", NULL, NULL, MXML_DESCEND);
		if(media_type != NULL && media_type->type == MXML_ELEMENT)
		{
			pNode = media_type->child;
            if( pNode != NULL)
			//printf("<<<  media type = %s  >>>.\n", pNode->value.text.string);
			    start_media->media.media_type = atoi(pNode->value.text.string);
            else
                start_media->media.media_type = 2;
            //printf("<<media type = %d\n",start_media->media.media_type);
		}

		stream_type = mxmlFindElement(parameters, xml_node, "streamType", NULL, NULL, MXML_DESCEND);
		if(stream_type != NULL && stream_type->type == MXML_ELEMENT)
		{
			pNode = stream_type->child;
            if( pNode != NULL)
		//	printf("<<<  stream type = %s  >>>.\n", pNode->value.text.string);
			    start_media->media.stream_type = atoi(pNode->value.text.string);
            else
            {
                start_media->media.stream_type = REQUEST_REAL_STREAM;
            }
		}

		link_mode = mxmlFindElement(parameters, xml_node, "linkMode", NULL, NULL, MXML_DESCEND);
		if(link_mode != NULL && link_mode->type == MXML_ELEMENT)
		{
			pNode = link_mode->child;
            if( pNode != NULL)
		//	printf("<<<  link mode = %s  >>>.\n", pNode->value.text.string);
			    start_media->link_mode = atoi(pNode->value.text.string);
		}

		target_ip = mxmlFindElement(parameters, xml_node, "targetIPAddr", NULL, NULL, MXML_DESCEND);
		if(target_ip != NULL && target_ip->type == MXML_ELEMENT)
		{
			pNode = target_ip->child;
            if( pNode != NULL)
			    strncpy(start_media->target_ip, pNode->value.text.string, sizeof(start_media->target_ip));
		}
		target_port = mxmlFindElement(parameters, xml_node, "targetPort", NULL, NULL, MXML_DESCEND);
		if(target_port != NULL && target_port->type == MXML_ELEMENT)
		{
			pNode = target_port->child;
            if( pNode != NULL)
	//		printf("<<<  target port  = %s  >>>.\n", pNode->value.text.string);
			    start_media->target_port = atoi(pNode->value.text.string);
		}

		file_name = mxmlFindElement(parameters, xml_node, "fileName", NULL, NULL, MXML_DESCEND);

		if(file_name != NULL && file_name->type == MXML_ELEMENT)
		{
            pNode = file_name->child;
            if( pNode != NULL)
            //	printf("<<<  file name = %s  >>>.\n", pNode->value.text.string);
                strncpy(start_media->fileName, pNode->value.text.string, sizeof(start_media->fileName));
            //request_type !=2 说明不是下载，是视频回放

            start_media->request_type = REQUEST_FILE_STREAM;
            //start_media->media.media_type= REQUEST_FILE_STREAM;
		}
        else
            start_media->request_type = 0;
		start_time = mxmlFindElement(parameters, xml_node, "startTime", NULL, NULL, MXML_DESCEND);
		if(start_time != NULL && start_time->type == MXML_ELEMENT)
		{
            if(pNode != NULL)
            {
                pNode = start_time->child;
                printf("startTime:/%s/\n",start_time_buf);
                sscanf(start_time_buf,"%04d-%02d-%02d %02d:%02d:%02d",
                &(start_media->start_time.year),
                &(start_media->start_time.month),
                &(start_media->start_time.date),
                &(start_media->start_time.hour),
                &(start_media->start_time.minute),
                &(start_media->start_time.second)
                );
            }
			
			//strncpy(start_media->start_time, pNode->value.text.string, sizeof(start_media->start_time));
			
		}

		end_time = mxmlFindElement(parameters, xml_node, "stopTime", NULL, NULL, MXML_DESCEND);
		if(end_time != NULL && end_time->type == MXML_ELEMENT)
		{
			pNode = end_time->child;
            //if(pNode != NULL)
		//	printf("<<<  end time = %s  >>>.\n", pNode->value.text.string);
			//strncpy(start_media->stop_time, pNode->value.text.string, sizeof(start_media->stop_time));
			//start_media->request_type = REQUEST_FILE_STREAM;
			printf("stopTime:/%s/\n",end_time_buf);
			sscanf(end_time_buf,"%04d-%02d-%02d %02d:%02d:%02d",
            &(start_media->stop_time.year),
            &(start_media->stop_time.month),
            &(start_media->stop_time.date),
            &(start_media->stop_time.hour),
            &(start_media->stop_time.minute),
            &(start_media->stop_time.second)
            );
		}
		playtype= mxmlFindElement(parameters, xml_node, "playType", NULL, NULL, MXML_DESCEND);
        if(playtype != NULL && playtype->type == MXML_ELEMENT)
		{
			pNode = playtype->child;
		    //if()
		//如果是下载，request_type 覆盖原来的回放，为2
			playType=atoi(pNode->value.text.string);
            if(playType==0)
               strcpy(start_media->cmd,"download");
		}
		ret = 0;
	}

    //新的cu，下载

	mxmlDelete(xml_node);
	
	return ret;
}

int build_invite_resp_packet(char *sip_packet,  LPNET_MDU_START_MEDIA start_media)
{
	if(!start_media->result_code)
	{
		switch(start_media->request_type)
		{
			case REQUEST_REAL_STREAM:
				return build_real_invite_resp_packet(sip_packet, start_media);
				break;
			case REQUEST_FILE_STREAM:
				return build_file_invite_resp_packet(sip_packet, start_media);
				break;
			default:
				LOG("Request type error %d ",start_media->request_type);
				return -1;
		}
	}
    else
    {
        printf("start_media->result_code%d\n",start_media->result_code);
        LOG("start_media->result_code%d\n",start_media->result_code);
    }

	return 0;
}

int build_file_invite_resp_packet(char* sip_packet, LPNET_MDU_START_MEDIA start_media)
{
    char time[30];
    
	mxml_node_t	*xml, *response, *result, *parameters, *start_time, *stop_time, *video_header;
	char buf[MAX_BUFFER_LEN] = {0};
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "Start_FilePlayBack_Rsp");

	result = mxmlNewElement(xml, "result");
	mxmlElementSetAttrf(result, "code", "%d", start_media->result_code);

	parameters = mxmlNewElement(response, "parameters");


    memset(time,0,sizeof(time));
    sprintf(time,"%04d-%02d-%02dT%02d:%02d:%02d",
    start_media->start_time.year,\
    start_media->start_time.month,\
    start_media->start_time.date,\
    start_media->start_time.hour,\
    start_media->start_time.minute,\
    start_media->start_time.second);
    
	start_time = mxmlNewElement(parameters, "startTime");
	mxmlNewText(start_time, 0, time);

    memset(time,0,sizeof(time));
    sprintf(time,"%04d-%02d-%02dT%02d:%02d:%02d",
    start_media->stop_time.year,\
    start_media->stop_time.month,\
    start_media->stop_time.date,\
    start_media->stop_time.hour,\
    start_media->stop_time.minute,\
    start_media->stop_time.second);
    
	stop_time = mxmlNewElement(parameters, "stopTime");
	mxmlNewText(stop_time, 0,time);

	if(start_media->video_header && start_media->video_header[0])
	{
		video_header = mxmlNewElement(parameters, "videoHeader");
		mxmlNewText(video_header, 0, start_media->video_header);
	}

	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);

//	PRINT_LOG("xml is ===[%d]- [%s].\n", strlen(buf), buf);

	sprintf(sip_packet, "SIP/2.0 200 OK\r\n"\
			"To: %s\r\n"\
			"From: %s\r\n"\
			"CSeq: %ld INVITE\r\n"
			"Call-ID: 323eaa-850ee-bf8797\r\n"\
			"Max-Forwards: 70\r\n"\
			"Via: SIP/2.0/UDP %s:%d\r\n"\
			"Contact: <sip:%s>\r\n"\
			"Content-Length: %d\r\n\r\n",
			cmu_info->pu_id,
			cmu_info->pu_id,
			g_seq++,
			cmu_skt->cli_ip,
			cmu_info->sip_port,
			cmu_skt->cli_ip,
			strlen(buf));
	strcat(sip_packet, buf);

	return strlen(sip_packet);
}

int build_real_invite_resp_packet(char* sip_packet, LPNET_MDU_START_MEDIA start_media)
{
	mxml_node_t	*xml, *response, *result, *parameters, *media_type, *stream_type;
	mxml_node_t	*image_width, *image_hight, *video_format, *frame_rate;
	char buf[MAX_BUFFER_LEN] = {0};
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "Start_MEDIA_Rsp");

	result = mxmlNewElement(xml, "result");
	mxmlElementSetAttrf(result, "code", "%d", start_media->result_code);

	parameters = mxmlNewElement(response, "parameters");

	media_type = mxmlNewElement(parameters, "mediaType");
	mxmlNewInteger(media_type, start_media->media.media_type);

	stream_type = mxmlNewElement(parameters, "streamType");
	mxmlNewInteger(stream_type, start_media->media.stream_type);

	image_width = mxmlNewElement(parameters, "imageWidth");
	mxmlNewInteger(image_width, start_media->image_width);

	image_hight = mxmlNewElement(parameters, "imageHight");
	mxmlNewInteger(image_hight, start_media->image_hight);

	video_format = mxmlNewElement(parameters, "videoFormat");
	mxmlNewInteger(video_format, start_media->video_format);

	frame_rate = mxmlNewElement(parameters, "frameRate");
	mxmlNewInteger(frame_rate, start_media->frame_rate);

	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);

//	PRINT_LOG("xml is ===[%d]- [%s].\n", strlen(buf), buf);

	sprintf(sip_packet, "SIP/2.0 200 OK\r\n"\
			"To: %s\r\n"\
			"From: %s\r\n"\
			"CSeq: %ld INVITE\r\n"
			"Call-ID: 323eaa-850ee-bf8797\r\n"\
			"Max-Forwards: 70\r\n"\
			"Via: SIP/2.0/UDP %s:%d\r\n"\
			"Contact: <sip:%s>\r\n"\
			"Content-Length: %d\r\n\r\n",
			cmu_info->pu_id,
			cmu_info->pu_id,
			g_seq++,
			cmu_skt->cli_ip,
			cmu_info->sip_port,
			cmu_skt->cli_ip,
			strlen(buf));
	strcat(sip_packet, buf);

	return strlen(sip_packet);
}

int parse_ack_packet(const char* sip_packet)
{
	if(strstr(sip_packet, "ACK"))
	{
		return 0;
	}

	return -1;
}

int parse_bye_packet(const char* sip_packet, LPNET_MDU_START_MEDIA stop_media)
{
	const char *starOpt = NULL;
	const char *stopOpt = NULL;
	char cLength[8] = {0}, xml[1024] = {0};
	int iLength = 0;
	mxml_node_t	*xml_node, *parameters, *pu_id, *pNode, *video_id, *media_type, *stream_type,*file_name;

	//printf("Stop Media:%s\n",sip_packet);
	if(!strstr(sip_packet, "BYE"))
	{
		return -1;
	}

	starOpt = strstr(sip_packet, "length:");
	if(starOpt==NULL)
	{
		return -1;
	}

	starOpt += strlen("length:");

	while(*starOpt==' ')
	{
		starOpt++;
	}

	stopOpt = strstr(starOpt, "\r\n");
	if(stopOpt==NULL)
	{
		return -1;
	}
	iLength = (int)(stopOpt - starOpt);
	if(iLength<=8)
	{
		strncpy(cLength, starOpt, iLength);
	}
	else
	{
		return -1;
	}

	iLength = atoi(cLength);
	if(iLength>1024)
	{
		return -1;
	}
	starOpt = strstr(sip_packet, "<?xml");
	if(starOpt==NULL)
	{
		return -1;
	}
	strncpy(xml, starOpt, iLength);

	xml_node = mxmlLoadString(NULL, xml, NULL);
 
	parameters = mxmlFindElement(xml_node, xml_node, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		pu_id = mxmlFindElement(parameters, xml_node, "puId", NULL, NULL, MXML_DESCEND);
		if(pu_id != NULL && pu_id->type == MXML_ELEMENT)
		{
            
			mxml_node_t* pNode = pu_id->child;
            if(pNode != NULL)
            
			//printf("<<<  puId = %s  >>>.\n", pNode->value.text.string);
			strncpy(stop_media->media.pu_id, pNode->value.text.string,sizeof(stop_media->media.pu_id));
		}

		video_id = mxmlFindElement(parameters, xml_node, "videoId", NULL, NULL, MXML_DESCEND);
		if(video_id != NULL && video_id->type == MXML_ELEMENT)
		{
			pNode = video_id->child;
		//	printf("<<<  video id = %s  >>>.\n", pNode->value.text.string);
						stop_media->request_type = REQUEST_REAL_STREAM;
			stop_media->media.video_id = atoi(pNode->value.text.string);
		}

		media_type = mxmlFindElement(parameters, xml_node, "mediaType", NULL, NULL,MXML_DESCEND);
		if(media_type != NULL && media_type->type == MXML_ELEMENT)
		{
			pNode = media_type->child;
			//printf("<<<  media type = %s  >>>.\n", pNode->value.text.string);
			stop_media->media.media_type= atoi(pNode->value.text.string);
		}
        else
		{
          	printf("<<<  media type Default  2>>>.\n");

            stop_media->media.media_type=2;//默认为2
		}

		stream_type = mxmlFindElement(parameters, xml_node, "streamType", NULL, NULL, MXML_DESCEND);
		if(stream_type != NULL && stream_type->type == MXML_ELEMENT)
		{
			pNode = stream_type->child;
			//printf("<<<  stream type = %s  >>>.\n", pNode->value.text.string);
			stop_media->media.stream_type = atoi(pNode->value.text.string);
		}
		file_name = mxmlFindElement(parameters, xml_node, "fileName", NULL, NULL, MXML_DESCEND);

		if(file_name != NULL && file_name->type == MXML_ELEMENT)
		{
            
                pNode = file_name->child;
    		    //printf("<<<  file name = %s  >>>.\n", pNode->value.text.string);
                if(pNode != NULL)
                {
        			strcpy(stop_media->fileName, pNode->value.text.string);
                    //printf("[parse_bye_packet]<<<  stop_media->fileName = %s  >>>.\n", stop_media->fileName);

        			//strncpy(stop_media->media->fileName, pNode->value.text.string, sizeof(stop_media->fileName));
        			stop_media->request_type = REQUEST_FILE_STREAM;
                }
                else
                {
                    stop_media->request_type = REQUEST_REAL_STREAM;
                 }
		}
        else
        {
             stop_media->request_type = REQUEST_REAL_STREAM;
        	 //stop_media->media.media_type= REQUEST_REAL_STREAM;  
        }

	}
	mxmlDelete(xml_node);
	return 0;
}

int build_bye_resp_packet(char* sip_packet)
{
	int ret = sprintf(sip_packet, "SIP/2.0 200 OK\r\n"\
                        "To: %s\r\n"\
                        "From: %s\r\n"\
                        "CSeq: %ld BYE\r\n"
                        "Call-ID: 323eaa-850ee-bf8797\r\n"\
                        "Max-Forwards: 70\r\n"\
                        "Via: SIP/2.0/UDP %s:%d\r\n"\
                        "Contact: <sip:%s>\r\n"\
                        "Content-Length: %d\r\n\r\n",
                        cmu_info->pu_id,
                        cmu_info->pu_id,
                        g_seq++,
                        cmu_skt->cli_ip,
                        cmu_info->sip_port,
                        cmu_skt->cli_ip,
                        0);

	return ret;
}

int build_media_info(int video_id, int media_type, int stream_type, char* sip_packet)
{
	mxml_node_t	*xml, *response, *parameters, *puId, *videoId, *mediaType,*streamType;
	char buf[MAX_BUFFER_LEN] = {0};
	static long cseq = 0;
	const char* cmdFmt = NULL;
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "request");
	mxmlElementSetAttrf(response, "command", "Media_Info");
		
	parameters = mxmlNewElement(response, "parameters");
		
	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, cmu_info->pu_id);
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId,  video_id);
	
	mediaType = mxmlNewElement(parameters, "mediaType");
	mxmlNewInteger(mediaType, media_type);

	
	streamType = mxmlNewElement(parameters, "streamType");
	mxmlNewInteger(streamType, stream_type);
	
	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);
	
//	PRINT_LOG("xml is ===[%d]- [%s].\n", strlen(buf), buf);
//////////////////////////////////////////////////////////////////////////

	cmdFmt="%s sip:%s SIP/2.0\r\n"\
					   "To:%s\r\nFrom: %s\r\n"\
					   "CSeq: %d %s\r\nCall-ID:%s\r\n"\
					   "Max-Forward:70\r\nVia:SIP/2.0/UDP %s:%d\r\n"\
					   "Contact:<sip:%s>\r\n"\
					   "Content-length:%d\r\n\r\n";
	
	sprintf(sip_packet, cmdFmt,
		"INFO", cmu_info->sip_ip, cmu_info->sip_ip,
		cmu_info->pu_id, ++cseq,"INFO","323eaa-850ee-bf8797", 
		cmu_skt->cli_ip, cmu_info->sip_port, cmu_skt->cli_ip, 
		strlen(buf));

	strcat(sip_packet, buf);
	
	return strlen(sip_packet);
}

int build_fileback_info(char* file_name, char* sip_packet)
{
	mxml_node_t	*xml, *response, *parameters, *puId, *fileName;
	char buf[MAX_BUFFER_LEN] = {0};
	const char* cmdFmt = NULL;
	static long cseq = 0;
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "request");
	mxmlElementSetAttrf(response, "command", "FileBack_Infos");
	
	parameters = mxmlNewElement(response, "parameters");
	
	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, cmu_info->pu_id);
	
	fileName = mxmlNewElement(parameters, "fileName");
	mxmlNewText(fileName, 0, file_name);
		
	mxmlSaveString(xml, buf, sizeof(buf), NULL);
	mxmlDelete(xml);
	
//	PRINT_LOG("xml is ===[%d]- [%s].\n", strlen(buf), buf);
	//////////////////////////////////////////////////////////////////////////
	
	cmdFmt= "%s sip:%s SIP/2.0\r\n"\
		"To:%s\r\n"\
		"From: %s\r\n"\
		"CSeq: %d %s\r\n"\
		"Call-ID:%s\r\n"\
		"Max-Forward:70\r\n"\
		"Via:SIP/2.0/UDP %s:%d\r\n"\
		"Contact:<sip:%s>\r\n"\
		"Content-length:%d\r\n\r\n";
	
	sprintf(sip_packet, cmdFmt, 
		"INFO", cmu_info->sip_ip,
		cmu_info->sip_ip, cmu_info->pu_id,
		cseq++, "INFO", "323eaa-850ee-bf8797",
		cmu_skt->cli_ip, cmu_info->sip_port,
		cmu_skt->cli_ip, strlen(buf));

	strcat(sip_packet, buf);
	
	return strlen(sip_packet);
}



