#include "ms_cmu_get.h"
#include "ms_pu_glob.h"

#include "ms_pu_param.h"
#include "ms_cmu_cmd.h"
#include "ms_pu_type.h"
#include "ms_pu_error.h"

#include "mxml.h"
#include "md5.h"
#include "oslib.h"
#include "iconv.h"
#include <string.h>
#include <stdio.h>


// 设备基本参数
int build_get_basic_info_pack(char *buf, int result,  LPNET_CMU_BASIC_INFO lp_basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *puId, *puName, *puPassword;
	char cValue[32] = "\0";
	unsigned char passwd[64] = {0};

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuBasicInfo");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	
	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, lp_basic_info->pu_id);
	
	puName = mxmlNewElement(parameters, "puName");
	mxmlNewText(puName, 0, lp_basic_info->pu_name);
	
	mingsoft_md5_passwd(ms_g_info.ms_info.passwd, passwd,strlen((char *)ms_g_info.ms_info.passwd));
	
	puPassword = mxmlNewElement(parameters, "puPassword");
	mxmlNewText(puPassword, 0, (const char *)passwd);	

	}
		
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);	
	mxmlDelete(xml);
	
//	LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	return 0;
}

// 设备网络参数
int build_get_ip_info_pack(char *buf,  int result,   LPNET_CMU_IP_INFO lp_ip_info)
{

	mxml_node_t	*xml, *response, *result_code, *parameters, *netId;
	mxml_node_t	*ipAddress, *controlPort, *subnetMask, *dns1, *dns2, *gateway;
	char cValue[32] = "\0";

	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuIpInfo");

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	

	if(result == PU_SUCCESS){	

	parameters = mxmlNewElement(response, "parameters");

	netId = mxmlNewElement(parameters, "netId");
	mxmlNewInteger(netId, lp_ip_info->net_id);

	ipAddress = mxmlNewElement(parameters, "ipAddress");
	mxmlNewText(ipAddress, 0, lp_ip_info->ip_addr);

	controlPort = mxmlNewElement(parameters, "controlPort");
	mxmlNewInteger(controlPort, lp_ip_info->ctrl_port);

	subnetMask = mxmlNewElement(parameters, "subnetMask");
	mxmlNewText(subnetMask, 0, lp_ip_info->net_mask);

	dns1 = mxmlNewElement(parameters, "dns");
	mxmlNewText(dns1, 0, lp_ip_info->dns_addr[0]);

	dns2 = mxmlNewElement(parameters, "dns");
	mxmlNewText(dns2, 0, lp_ip_info->dns_addr[1]);
	
	gateway = mxmlNewElement(parameters, "gateway");
	mxmlNewText(gateway, 0, lp_ip_info->gate_way);
	}
	
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备用户参数
int build_get_user_info_pack(char *buf,  int result,   LPNET_CMU_USER_LIST lp_user_info)
{
	int i = 0;	
	mxml_node_t	*xml, *response, *result_code, *parameters, *username, *password;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuUser");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	
	for(i = 0; i < lp_user_info->user_cnt; i++)
	{
		username = mxmlNewElement(parameters, "username");
		mxmlNewText(username, 0, lp_user_info->user_list[i].user_name);
		
		password = mxmlNewElement(parameters, "password");
		mxmlNewText(password, 0, lp_user_info->user_list[i].pass_word);		
	}
		
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

// 设备串口参数
int prase_get_serial_info_pack(char *buf,   int result,   LPNET_CMU_SERIAL_PORT lp_serial_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *forDecoder, *serialPort, *videoId, *pNode;
	//char cValue[32] = "\0";
	
	xml = mxmlLoadString(NULL, buf, NULL);

	
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		forDecoder = mxmlFindElement(parameters, xml, "forDecoder", NULL, NULL, MXML_DESCEND);
		if(forDecoder != NULL && forDecoder->type == MXML_ELEMENT)
		{
			pNode = forDecoder->child;
			//printf("<<<  forDecoder = %s  >>>.\n", pNode->value.text.string);
			strcpy(lp_serial_info->for_decoder, pNode->value.text.string);
		}
		
		serialPort = mxmlFindElement(parameters, xml, "serialPort", NULL, NULL, MXML_DESCEND);
		if(serialPort != NULL && serialPort->type == MXML_ELEMENT)
		{
			pNode = serialPort->child;
			//printf("<<<  serialPort = %s  >>>.\n", pNode->value.text.string);
			lp_serial_info->serial_port = atoi(pNode->value.text.string);
		}
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			lp_serial_info->video_id = atoi(pNode->value.text.string);
		}
	}
	
	mxmlDelete(xml);
	return ret;
}

int build_get_serial_info_pack(char *buf, int result,  LPNET_CMU_SERIAL_PORT lp_serial_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *forDecoder, *serialPort, *videoId;
	mxml_node_t	*baudRate, *dataBit, *parity, *stopBit, *mode, *timeOut, *flowControl, *decoderType;
	mxml_node_t	*decoderAddress, *workMode;
	char cValue[32] = "\0";
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuSerialPort");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	
	forDecoder = mxmlNewElement(parameters, "forDecoder");
	mxmlNewText(forDecoder, 0, lp_serial_info->for_decoder);
	
	serialPort = mxmlNewElement(parameters, "serialPort");
	mxmlNewInteger(serialPort, lp_serial_info->serial_port);			

	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_serial_info->video_id);			

	baudRate = mxmlNewElement(parameters, "baudRate");
	mxmlNewInteger(baudRate, lp_serial_info->baud_rate);			

	dataBit = mxmlNewElement(parameters, "dataBit");
	mxmlNewInteger(dataBit, lp_serial_info->data_bit);			

	parity = mxmlNewElement(parameters, "parity");
	mxmlNewInteger(parity, lp_serial_info->parity);			

	stopBit = mxmlNewElement(parameters, "stopBit");
	mxmlNewInteger(stopBit, lp_serial_info->stop_bit);			
	
	mode = mxmlNewElement(parameters, "mode");
	mxmlNewText(mode, 0, lp_serial_info->mode);			

	timeOut = mxmlNewElement(parameters, "timeOut");
	mxmlNewInteger(timeOut, lp_serial_info->time_out);			

	flowControl = mxmlNewElement(parameters, "flowControl");
	mxmlNewInteger(flowControl, lp_serial_info->flow_control);			

	decoderType = mxmlNewElement(parameters, "decoderType");
	mxmlNewInteger(decoderType, lp_serial_info->decoder_type);			

	decoderAddress = mxmlNewElement(parameters, "decoderAddress");
	mxmlNewInteger(decoderAddress, lp_serial_info->decoder_address);			

	workMode = mxmlNewElement(parameters, "workMode");
	mxmlNewInteger(workMode, lp_serial_info->work_mode);			
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

// 设备输入输出接口参数
int build_get_inout_dev_pack(char *buf,	 int result,   LPNET_CMU_INOUT_DEV lp_inout_dev_info)
{
	int i = 0;
	mxml_node_t	*xml, *response, *result_code, *parameters, *input, *output, *channelNumber, *state;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuInOutDev");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	
	for(i = 0; i < lp_inout_dev_info->in_list_cnt; i++)
	{
		input = mxmlNewElement(parameters, "input");
		
		channelNumber = mxmlNewElement(input, "channelNumber");
		mxmlNewInteger(channelNumber, lp_inout_dev_info->in_list[i].channel_num);			
		
		state = mxmlNewElement(input, "state");
		mxmlNewInteger(state, lp_inout_dev_info->in_list[i].state);					
	}

	for(i = 0; i < lp_inout_dev_info->out_list_cnt; i++)
	{
		output = mxmlNewElement(parameters, "output");
		
		channelNumber = mxmlNewElement(output, "channelNumber");
		mxmlNewInteger(channelNumber, lp_inout_dev_info->out_list[i].channel_num);			
		
		state = mxmlNewElement(output, "state");
		mxmlNewInteger(state, lp_inout_dev_info->out_list[i].state);					
	}	
	

	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备时间参数
int build_get_time_info_pack(char *buf,  int result,   LPNET_CMU_DEV_TIME lp_time_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *hour, *minute, *second, *month, *date, *year;
	char cValue[32] = "\0";
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuTime");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
		
	hour = mxmlNewElement(parameters, "hour");
	mxmlNewInteger(hour, lp_time_info->hour);			
	
	minute = mxmlNewElement(parameters, "minute");
	mxmlNewInteger(minute, lp_time_info->minute);			
	
	second = mxmlNewElement(parameters, "second");
	mxmlNewInteger(second, lp_time_info->second);			
	
	month = mxmlNewElement(parameters, "month");
	mxmlNewInteger(month, lp_time_info->month);			
	
	date = mxmlNewElement(parameters, "date");
	mxmlNewInteger(date, lp_time_info->date);			

	year = mxmlNewElement(parameters, "year");
	mxmlNewInteger(year, lp_time_info->year);			

	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备图像制式参数
int prase_get_video_format_pack(char *buf, int result,  LPNET_CMU_VIDEO_FORMAT_LIST lp_video_format)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
		
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_video_format->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}

int build_get_video_format_pack(char *buf, int result,  LPNET_CMU_VIDEO_FORMAT_LIST lp_video_format)
{
	int i = 0;
	mxml_node_t	*xml, *response, *result_code, *parameters, *format, *videoId, *videoFormat;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuVideoFormat");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	for(i = 0; i < lp_video_format->vformat_cnt; i++)
	{
		format = mxmlNewElement(parameters, "format");
		
		videoId = mxmlNewElement(format, "videoId");
		mxmlNewInteger(videoId, lp_video_format->channel_video_format[i].video_id);			
		
		videoFormat = mxmlNewElement(format, "videoFormat");
		mxmlNewInteger(videoFormat, lp_video_format->channel_video_format[i].video_format);					
	}
	
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
		
	return 0;
}

// 设备图像编码参数
int prase_get_img_encode_pack(char *buf,  int result, 	LPNET_CMU_IMG_ENCODE lp_img_encode)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_img_encode->video_id = atoi(pNode->value.text.string);
		}
        
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_img_encode_pack(char *buf, int result, 	LPNET_CMU_IMG_ENCODE lp_img_encode)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *encodeMode, *picQuality;
	mxml_node_t *bitRate, *bitRateType, *frameRate, *imageSize, *streamType, *iFrameInterval;
	
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuImageEncodePara");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_img_encode->video_id);			
	
	encodeMode = mxmlNewElement(parameters, "encodeMode");
	mxmlNewText(encodeMode, 0, lp_img_encode->encode_mode);					
	
	picQuality = mxmlNewElement(parameters, "picQuality");
	mxmlNewInteger(picQuality, lp_img_encode->pic_quality);					

	bitRate = mxmlNewElement(parameters, "bitRate");
	mxmlNewInteger(bitRate, lp_img_encode->bit_rate);					

	bitRateType = mxmlNewElement(parameters, "bitRateType");
	mxmlNewInteger(bitRateType, lp_img_encode->bit_rate_type);					

	frameRate = mxmlNewElement(parameters, "frameRate");
	mxmlNewInteger(frameRate, lp_img_encode->frame_rate);					

	imageSize = mxmlNewElement(parameters, "imageSize");
	mxmlNewText(imageSize, 0, lp_img_encode->image_size);					

	streamType = mxmlNewElement(parameters, "streamType");
	if(lp_img_encode->stream_type)
		lp_img_encode->stream_type = 0;
	else
		lp_img_encode->stream_type = 1;
	
	mxmlNewInteger(streamType, lp_img_encode->stream_type);					

	iFrameInterval = mxmlNewElement(parameters, "iFrameInterval");
	mxmlNewInteger(iFrameInterval, lp_img_encode->iframe_interval);					

	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备图像显示参数
int prase_get_img_display_pack(char *buf, int result, 	LPNET_CMU_IMG_DISPLAY lp_img_display)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_img_display->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_img_display_pack(char *buf, int result, 	LPNET_CMU_IMG_DISPLAY lp_img_display)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *contrast, *bright, *hue, *saturation;

	char cValue[32] = "\0";
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuImageDisplayPara");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_img_display->video_id);			
	
	contrast = mxmlNewElement(parameters, "contrast");
	mxmlNewInteger(contrast, lp_img_display->contrast);					
	
	bright = mxmlNewElement(parameters, "bright");
	mxmlNewInteger(bright, lp_img_display->bright);					
	
	hue = mxmlNewElement(parameters, "hue");
	mxmlNewInteger(hue, lp_img_display->hue);					
	
	saturation = mxmlNewElement(parameters, "saturation");
	mxmlNewInteger(saturation, lp_img_display->saturation);					
		
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备图像相关的文字参数
int prase_get_img_text_pack(char *buf,	int result, 	LPNET_CMU_IMG_TEXT lp_img_text)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_img_text->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_img_text_pack(char *buf,	int result, 	LPNET_CMU_IMG_TEXT lp_img_text)
{
	//char *out = NULL ;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char  p[1024] ;
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *bitmapX, *bitmapY ,*meiDianFlag;
	mxml_node_t	*bitmapTimeEnable, *bitmapTextEnable, *bitmapText, *bitmapTextX, *bitmapTextY;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuImageTextPara");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_img_text->video_id);			
	
	bitmapX = mxmlNewElement(parameters, "bitmapX");
	mxmlNewInteger(bitmapX, lp_img_text->time_X);					
	
	bitmapY = mxmlNewElement(parameters, "bitmapY");
	mxmlNewInteger(bitmapY, lp_img_text->time_Y);

	meiDianFlag = mxmlNewElement(parameters , "meiDianFlag")	;
	mxmlNewInteger(meiDianFlag , 1);		
	
	bitmapTimeEnable = mxmlNewElement(parameters, "bitmapTimeEnable");
	mxmlNewText(bitmapTimeEnable, 0, lp_img_text->time_enable);					

	bitmapTextEnable = mxmlNewElement(parameters, "bitmapTextEnable");
	memset(lp_img_text->text_enable,0,5);
	//strncpy(lp_img_text->text_enable,"false",5);

	mxmlNewText(bitmapTextEnable, 0, "true");					
		
	
	bitmapText = mxmlNewElement(parameters, "bitmapText");
	//memset(lp_img_text->text,0,128);
	//strncpy(lp_img_text->text,"happy",5);
	mxmlNewText(bitmapText, 0, lp_img_text->text);					

	bitmapTextX = mxmlNewElement(parameters, "bitmapTextX");
	mxmlNewInteger(bitmapTextX, lp_img_text->text_X);					
	
	bitmapTextY = mxmlNewElement(parameters, "bitmapTextY");
	mxmlNewInteger(bitmapTextY, lp_img_text->text_Y);					
	
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	sscanf(buf,"%[^>]",p) ;
	strcat(secondbuf , &buf[strlen(p)+1]) ;
	memcpy(buf , secondbuf , 1024) ;
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备基本告警参数
int prase_get_basic_alarm_pack(char *buf, int result, 	LPNET_CMU_BASIC_ALARM lp_basic_alarm)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *alarmInChannel, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		alarmInChannel = mxmlFindElement(parameters, xml, "alarmInChannel", NULL, NULL, MXML_DESCEND);
		if(alarmInChannel != NULL && alarmInChannel->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = alarmInChannel->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_basic_alarm->alarm_in_channel = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_basic_alarm_pack(char *buf, int result, 	LPNET_CMU_BASIC_ALARM lp_basic_alarm)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *alarmInChannel, *alarmEnabled, *isCopyToAll;
	mxml_node_t	*alarmRecord, *alarmOutput, *shootPhoto, *preset, *activatedTimes;
	
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuImageTextPara");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	alarmInChannel = mxmlNewElement(parameters, "alarmInChannel");
	mxmlNewInteger(alarmInChannel, lp_basic_alarm->alarm_in_channel);			
	
	alarmEnabled = mxmlNewElement(parameters, "alarmEnabled");
	mxmlNewText(alarmEnabled, 0, lp_basic_alarm->alarm_enabled);					
	
	isCopyToAll = mxmlNewElement(parameters, "isCopyToAll");
	mxmlNewText(isCopyToAll, 0, lp_basic_alarm->is_copy_toall);					
	
	alarmRecord = mxmlNewElement(parameters, "alarmRecord");
	mxmlElementSetAttrf(alarmRecord, "recordChannel", "%d", lp_basic_alarm->alarm_record.record_channel);	
	mxmlElementSetAttrf(alarmRecord, "recordTime", "%d", lp_basic_alarm->alarm_record.record_time);	
	mxmlElementSetAttrf(alarmRecord, "precodeTime", "%d", lp_basic_alarm->alarm_record.precord_time);	
	
	alarmOutput = mxmlNewElement(parameters, "alarmOutput");
	mxmlElementSetAttrf(alarmOutput, "channel", "%d", lp_basic_alarm->alarm_output.alarm_channel);	
	mxmlElementSetAttrf(alarmOutput, "alarmTime", "%d", lp_basic_alarm->alarm_output.alarm_time);		
	
	shootPhoto = mxmlNewElement(parameters, "shootPhoto");
	mxmlElementSetAttrf(shootPhoto, "channel", "%d", lp_basic_alarm->shot_photo.shot_channel);	
	mxmlElementSetAttrf(shootPhoto, "enabled", "%d", lp_basic_alarm->shot_photo.shot_times);		
	
	preset = mxmlNewElement(parameters, "preset");
	mxmlElementSetAttrf(preset, "presetChannel", "%d", lp_basic_alarm->ptz_preset.ptz_channel);	
	mxmlElementSetAttrf(preset, "preset", "%d", lp_basic_alarm->ptz_preset.ptz_preset);		
	
	activatedTimes = mxmlNewElement(parameters, "activatedTimes");
	mxmlElementSetAttrf(activatedTimes, "start", "%s", lp_basic_alarm->active_times.active_start);	
	mxmlElementSetAttrf(activatedTimes, "end", "%s", lp_basic_alarm->active_times.active_end);						
	
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备Gpin告警参数
int prase_get_gpin_alarm_pack(char *buf, int result, 	LPNET_CMU_GPIN_ALARM_INFO lp_gpin_alarm)
{
	return 0;
}
int build_get_gpin_alarm_pack(char *buf, int result, 	LPNET_CMU_GPIN_ALARM_INFO lp_gpin_alarm)
{
			mxml_node_t *xml, *response, *result_code/*, *parameters, *hideDetection, *videoId, *alarm_enabled*/;
			//mxml_node_t *alar_time, *x, *y, *width, *height;
			//mxml_node_t *sensitivity, *h_alarm_output, *alarm_output, *alarm_state, *h_vedio, *vedio;
			
			char cValue[32] = "\0";
			
			xml = mxmlNewXML("1.0");
			response = mxmlNewElement(xml, "response");
			mxmlElementSetAttrf(response, "command", "GetPuGpinAlarm");
			
			result_code = mxmlNewElement(response, "result");
			mxmlElementSetAttrf(result_code, "code", "%d", result); 
		
			
			memset(cValue, 0, 32);
			strncpy(cValue, msGetPuErrorInfo(result), 32);
			mxmlNewText(result_code, 0,cValue); 
			
			if(result == PU_SUCCESS){	
						
			}
			mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
			mxmlDelete(xml);
			
			//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
			
			return 0;
}

// 设备视频丢失告警参数
int prase_get_video_lost_pack(char *buf, int result, 	LPNET_CMU_VIDEO_LOSE lp_video_lost)
{
	return 0;
}
int build_get_video_lost_pack(char *buf, int result, 	LPNET_CMU_VIDEO_LOSE lp_video_lost)
{
		mxml_node_t *xml, *response, *result_code;/* *parameters, *hideDetection, *videoId, *alarm_enabled*/
		//mxml_node_t *alar_time, *x, *y, *width, *height;
		//mxml_node_t *sensitivity, *h_alarm_output, *alarm_output, *alarm_state, *h_vedio, *vedio, *alarm_record;
		
		char cValue[32] = "\0";
		
		xml = mxmlNewXML("1.0");
		response = mxmlNewElement(xml, "response");
		mxmlElementSetAttrf(response, "command", "GetPuVideoLose");
		
		result_code = mxmlNewElement(response, "result");
		mxmlElementSetAttrf(result_code, "code", "%d", result); 
	
		
		memset(cValue, 0, 32);
		strncpy(cValue, msGetPuErrorInfo(result), 32);
		mxmlNewText(result_code, 0,cValue); 
		
		if(result == PU_SUCCESS){	
					
		}
		mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
		mxmlDelete(xml);
		
		//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
		
		return 0;
	}


// 设备视频遮蔽告警参数
int prase_get_hide_detect_pack(char *buf, int result, 	LPNET_CMU_HIDE_DETECT lp_hide_detect)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_hide_detect->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}

int build_get_hide_detect_pack(char *buf, int result, 	LPNET_CMU_HIDE_DETECT lp_hide_detect)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *hideDetection, *videoId, *alarm_enabled;
	mxml_node_t	*alar_time, *x, *y, *width, *height;
	mxml_node_t	*sensitivity, *h_alarm_output, *alarm_output, *alarm_state, *h_vedio, *vedio, *alarm_record;
	
	char cValue[32] = "\0";
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuHideDetection");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	
	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){	
	
	parameters = mxmlNewElement(response, "parameters");
	hideDetection = mxmlNewElement(parameters, "hideDetection");
	
	videoId = mxmlNewElement(hideDetection, "videoId");
	mxmlNewInteger(videoId, lp_hide_detect->video_id);					
	
	alarm_enabled = mxmlNewElement(hideDetection, "hideDetectionAlarmEnabled");
	mxmlNewText(alarm_enabled, 0, lp_hide_detect->alarm_enabled);					
	
	alar_time = mxmlNewElement(hideDetection, "hideDetectionAlarmTime");
	mxmlNewText(alar_time, 0, lp_hide_detect->alarm_time);					

	x = mxmlNewElement(hideDetection, "hideDetectionX");
	mxmlNewInteger(x,  lp_hide_detect->hide_coord.x);					

	y = mxmlNewElement(hideDetection, "hideDetectionY");
	mxmlNewInteger(y,  lp_hide_detect->hide_coord.y);					

	width = mxmlNewElement(hideDetection, "hideDetectionWidth");
	mxmlNewInteger(width,  lp_hide_detect->hide_coord.width);					

	height = mxmlNewElement(hideDetection, "hideDetectionHeight");
	mxmlNewInteger(height,  lp_hide_detect->hide_coord.height);					

	sensitivity = mxmlNewElement(hideDetection, "hideDetectionSensitivity");
	mxmlNewInteger(sensitivity,  lp_hide_detect->sensitivity);
	
	h_alarm_output = mxmlNewElement(hideDetection, "hideDetectionAlarmOutput");
	alarm_output = mxmlNewElement(h_alarm_output, "alarmOutput");
	mxmlNewInteger(alarm_output, lp_hide_detect->hide_alarm_output.alarm_channel);					

	alarm_state = mxmlNewElement(h_alarm_output, "alarmState");
	mxmlNewInteger(alarm_state,  lp_hide_detect->hide_alarm_output.alarm_time);
	
	h_vedio = mxmlNewElement(hideDetection, "hideDetectionVedio");
	vedio = mxmlNewElement(h_vedio, "hideDetection");
	mxmlNewInteger(vedio, lp_hide_detect->hide_alarm_video.alarm_record_channel);					

	alarm_record = mxmlNewElement(h_vedio, "AlarmRecord");
	mxmlNewInteger(alarm_record, lp_hide_detect->hide_alarm_video.alarm_shot_channel);					
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备移动侦测告警初始化参数
int prase_get_motion_detect_pack(char *buf, int result, 	LPNET_CMU_MOTION_DETECT lp_motion_detect)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);	
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_motion_detect->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_motion_detect_pack(char *buf, int result, 	LPNET_CMU_MOTION_DETECT lp_motion_detect)
{
	int i = 0; 
	char tmp[128] = {0};
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *alarm_enabled;
	mxml_node_t *alarm_time, *blocks, *frequency, *sensitivity;
	
	char cValue[32] = "\0";
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuMotionDetection");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_motion_detect->video_id);					
	
	alarm_enabled = mxmlNewElement(parameters, "motionDetectionAlarmEnabled");
	mxmlNewText(alarm_enabled, 0, lp_motion_detect->alarm_enabled);					
	
	alarm_time = mxmlNewElement(parameters, "motionDetectionAlarmTime");
	mxmlNewText(alarm_time, 0, lp_motion_detect->alarm_time);					
	
	for(i = 0; i < lp_motion_detect->block_list_cnt; i++)
	{ 
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d,%d,%d,%d",
			lp_motion_detect->block_lists[i].x,\
			lp_motion_detect->block_lists[i].y,\
			lp_motion_detect->block_lists[i].width,\
			lp_motion_detect->block_lists[i].height);

			printf("\n x value is %d\n",lp_motion_detect->block_lists[i].x);
			printf("\n y value is %d\n",lp_motion_detect->block_lists[i].y);
			printf("\n width value is %d\n",lp_motion_detect->block_lists[i].width);
			printf("\n height value is %d\n",lp_motion_detect->block_lists[i].height);
			

		blocks = mxmlNewElement(parameters, "motionDetectionBlocks");
		mxmlNewText(blocks, 0, tmp);						
	}

	frequency = mxmlNewElement(parameters, "motionDetectionFrequency");
	mxmlNewInteger(frequency, lp_motion_detect->frequency);					

	sensitivity = mxmlNewElement(parameters, "motionDetectionSensitivity");
	mxmlNewInteger(sensitivity, lp_motion_detect->sensitivity);					

/*
	alarm_output = mxmlNewElement(parameters, "motionDetectionAlarmOutput");
	mxmlElementSetAttrf(alarm_output, "alarmOutput", "%d", lp_motion_detect->motion_alarm_output.alarm_channel);	
	mxmlElementSetAttrf(alarm_output, "alarmState", "%d", lp_motion_detect->motion_alarm_output.alarm_time);	
	
	vedio = mxmlNewElement(parameters, "motionDetectionVedio");
	mxmlElementSetAttrf(vedio, "motionDetectionAlarmRecord", "%d", lp_motion_detect->motion_alarm_video.alarm_record_channel);	
	mxmlElementSetAttrf(vedio, "motionDetectionAlarmShoot", "%d", lp_motion_detect->motion_alarm_video.alarm_shot_channel);		
	*/
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备图像遮挡区域
int prase_get_img_hide_area_pack(char *buf, int result, 	LPNET_CMU_IMG_HIDE_AREA lp_img_hide_area)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_img_hide_area->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_img_hide_area_pack(char *buf, int result, 	LPNET_CMU_IMG_HIDE_AREA lp_img_hide_area)
{
	int i = 0; 
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *enabled;
	mxml_node_t *image_area, *x, *y, *width, *height;
	
	char cValue[32] = "\0";
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuImageHideArea");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	
	
	parameters = mxmlNewElement(response, "parameters");

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_img_hide_area->video_id);					
	
	enabled = mxmlNewElement(parameters, "HideAreaEnabled");
	mxmlNewText(enabled, 0, lp_img_hide_area->enabled);					
		
	for(i = 0; i < lp_img_hide_area->area_list_cnt; i++)
	{ 
		image_area = mxmlNewElement(parameters, "hideImageArea");
		
		x = mxmlNewElement(image_area, "HideAreaX");
		mxmlNewInteger(x, lp_img_hide_area->area_lists[i].x);						

		y = mxmlNewElement(image_area, "HideAreaY");
		mxmlNewInteger(y,  lp_img_hide_area->area_lists[i].y);						

		width = mxmlNewElement(image_area, "HideAreaWidth");
		mxmlNewInteger(width, lp_img_hide_area->area_lists[i].width);						

		height = mxmlNewElement(image_area, "hideAreaHeight");
		mxmlNewInteger(height,  lp_img_hide_area->area_lists[i].height);									
	}
		

	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备本地存储参数
int prase_get_storage_task_pack(char *buf, int result, 	LPNET_CMU_STORAGE_TASK lp_storage_task)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  videoId = %s  >>>.\n", pNode->value.text.string);
			
			lp_storage_task->video_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_storage_task_pack(char *buf, int result, 	LPNET_CMU_STORAGE_TASK lp_storage_task)
{
	int i = 0; 
	mxml_node_t	*xml, *response, *result_code, *parameters, *videoId, *record_enabled, *recycleRec_enable;
	mxml_node_t *schedule, *weekday, *recordType, *startHour, *startMin, *stopHour, *stopMin;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuLocalStorageTask");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	videoId = mxmlNewElement(parameters, "videoId");
	mxmlNewInteger(videoId, lp_storage_task->video_id);					
	
	record_enabled = mxmlNewElement(parameters, "recordEnable");
	mxmlNewInteger(record_enabled, lp_storage_task->record_enable);					
	
	recycleRec_enable = mxmlNewElement(parameters, "recycleRecEnable");
	mxmlNewInteger(recycleRec_enable, lp_storage_task->recycle_enable);					
	
	for(i = 0; i < lp_storage_task->schedule_list_cnt; i++)
	{ 
		schedule = mxmlNewElement(parameters, "schedule");
		
		weekday = mxmlNewElement(schedule, "weekday");
		mxmlNewInteger(weekday, lp_storage_task->schedule_lists[i].week_day);						
		
		recordType = mxmlNewElement(schedule, "recordType");
		mxmlNewInteger(recordType,  lp_storage_task->schedule_lists[i].record_type);						
		
		startHour = mxmlNewElement(schedule, "startHour");
		mxmlNewInteger(startHour, lp_storage_task->schedule_lists[i].start_hour);						
		
		startMin = mxmlNewElement(schedule, "startMin");
		mxmlNewInteger(startMin,  lp_storage_task->schedule_lists[i].start_min);									

		stopHour = mxmlNewElement(schedule, "stopHour");
		mxmlNewInteger(stopHour,  lp_storage_task->schedule_lists[i].stop_hour);									

		stopMin = mxmlNewElement(schedule, "stopMin");
		mxmlNewInteger(stopMin,  lp_storage_task->schedule_lists[i].stop_min);											
	}

	}
	
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备云台预置点
int prase_get_ptz_preset_pack(char *buf, int result, 	LPNET_CMU_PRESET_LIST lp_ptz_preset)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;

	xml = mxmlLoadString(NULL, buf, NULL);	
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  ptzId = %s  >>>.\n", pNode->value.text.string);
			
			lp_ptz_preset->ptz_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_ptz_preset_pack(char *buf, int result, 	LPNET_CMU_PRESET_LIST lp_ptz_preset)
{

	//char *out = NULL ;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char  p[1024] ;
	int i = 0; 
	mxml_node_t	*xml, *response, *result_code, *parameters, *preset, *presetIndex, *presetName;
	char cValue[32] = "\0";


	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPresetPTZ");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
		
	for(i = 0; i < lp_ptz_preset->preset_list_cnt; i++)
	{ 
		preset = mxmlNewElement(parameters, "preset");
		
		presetIndex = mxmlNewElement(preset, "presetIndex");
		mxmlNewInteger(presetIndex, lp_ptz_preset->preset_lists[i].preset_index);						
		
		presetName = mxmlNewElement(preset, "presetName");
		mxmlNewText(presetName,  0,  lp_ptz_preset->preset_lists[i].preset_name);							
	}
	
	}



	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);


		printf("\nbuild gb  brefore\n");
		//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	sscanf(buf,"%[^>]",p) ;
	//printf("%s" , p) ;
	strcat(secondbuf , &buf[strlen(p)+1]) ;
	
	//printf("%s\n" , secondbuf) ;
	memcpy(buf , secondbuf , 1024) ;
	
	mxmlDelete(xml);

	
    printf("\nbuild gb  after\n");
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备默认云台预置点
int prase_get_default_preset_pack(char *buf, int result, 	LPNET_CMU_DEFAULT_PRESET lp_default_preset)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode;
	
	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		
		videoId = mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			ret = 0;
			
			pNode = videoId->child;
			
			//printf("<<<  ptzId = %s  >>>.\n", pNode->value.text.string);
			
			lp_default_preset->ptz_id = atoi(pNode->value.text.string);
		}
	}		
	
	mxmlDelete(xml);
	return ret;
}
int build_get_default_preset_pack(char *buf, int result,LPNET_CMU_DEFAULT_PRESET lp_default_preset)
{

	//char *out = NULL ;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char  p[1024] ;
	mxml_node_t	*xml, *response, *result_code, *parameters, *ptzId, *presetIndex, *presetName, *restoreTime;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetDefaulePresetPTZ");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");

	ptzId = mxmlNewElement(parameters, "ptzId");
	mxmlNewInteger(ptzId, lp_default_preset->ptz_id);						
	
	presetIndex = mxmlNewElement(parameters, "presetIndex");
	mxmlNewInteger(presetIndex, lp_default_preset->preset_index);						
	
	presetName = mxmlNewElement(parameters, "presetName");
	mxmlNewText(presetName, 0, lp_default_preset->preset_name);						
	
	restoreTime = mxmlNewElement(parameters, "restoreTime");
	mxmlNewInteger(restoreTime,  lp_default_preset->restore_time);							
	
	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	sscanf(buf,"%[^>]",p) ;
	strcat(secondbuf , &buf[strlen(p)+1]) ;
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

// 设备巡航轨迹
// 设备状态信息
int build_get_dev_state_pack(char *buf,	  int result,    LPNET_CMU_DEV_STATE lp_dev_state)
{
	int i = 0;
	mxml_node_t	*xml, *response, *result_code, *parameters, *deviceStatus, *disk, *diskId;
	mxml_node_t	*totalSize, *freeSize, *disk_status, *videoChannel, *videoId, *recordStatus;
	mxml_node_t	*signalStatus, *hardwareStatus, *bitRate, *alarmIn, *alarmOut, *channelNumber, *status;
	
	char cValue[32] = "\0";
	
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuState");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	


	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	deviceStatus = mxmlNewElement(parameters, "deviceStatus");
	mxmlNewInteger(deviceStatus, lp_dev_state->device_status);						
	
	for(i = 0; i < lp_dev_state->disk_list_cnt; i++)
	{
		disk = mxmlNewElement(parameters, "disk");
		
		diskId = mxmlNewElement(disk, "diskId");
		mxmlNewInteger(diskId, lp_dev_state->disk_status_lists[i].disk_id);						
		
		totalSize = mxmlNewElement(disk, "totalSize");
		mxmlNewInteger(totalSize, lp_dev_state->disk_status_lists[i].total_size);							
		
		freeSize = mxmlNewElement(disk, "freeSize");
		mxmlNewInteger(freeSize,  lp_dev_state->disk_status_lists[i].free_size);							
		
		disk_status = mxmlNewElement(disk, "status");
		mxmlNewInteger(disk_status,  lp_dev_state->disk_status_lists[i].status);							
	}

	for(i = 0; i < lp_dev_state->video_channel_list_cnt; i++)
	{
		videoChannel = mxmlNewElement(parameters, "videoChannel");
		
		videoId = mxmlNewElement(videoChannel, "videoId");
		mxmlNewInteger(videoId, lp_dev_state->video_channel_lists[i].video_id);						
		
		recordStatus = mxmlNewElement(videoChannel, "recordStatus");
		mxmlNewInteger(recordStatus, lp_dev_state->video_channel_lists[i].record_status);							
		
		signalStatus = mxmlNewElement(videoChannel, "signalStatus");
		mxmlNewInteger(signalStatus, lp_dev_state->video_channel_lists[i].signal_status);							
		
		hardwareStatus = mxmlNewElement(videoChannel, "hardwareStatus");
		mxmlNewInteger(hardwareStatus,  lp_dev_state->video_channel_lists[i].hardware_status);							

		bitRate = mxmlNewElement(videoChannel, "bitRate");
		mxmlNewInteger(bitRate,  lp_dev_state->video_channel_lists[i].bit_rate);							
	}
	
	for(i = 0; i < lp_dev_state->alarm_in_list_cnt; i++)
	{
		alarmIn = mxmlNewElement(parameters, "alarmIn");
		
		channelNumber = mxmlNewElement(alarmIn, "channelNumber");
		mxmlNewInteger(channelNumber, lp_dev_state->alarm_in_lists[i].alarm_inout_channel);						
		
		status = mxmlNewElement(alarmIn, "status");
		mxmlNewInteger(status, lp_dev_state->alarm_in_lists[i].alarm_inout_state);								
	}

	for(i = 0; i < lp_dev_state->alarm_out_list_cnt; i++)
	{
		alarmOut = mxmlNewElement(parameters, "alarmOut");
		
		channelNumber = mxmlNewElement(alarmOut, "channelNumber");
		mxmlNewInteger(channelNumber, lp_dev_state->alarm_out_lists[i].alarm_inout_channel);						
		
		status = mxmlNewElement(alarmOut, "status");
		mxmlNewInteger(status, lp_dev_state->alarm_out_lists[i].alarm_inout_state);								
	}

	}
	
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;	
}

// 设备日志信息
int build_get_dev_log_pack(char *buf,	 int result,     LPNET_CMU_DEV_LOG lp_dev_log)
{
	return 0;
}

// 设备软件版本信息
int build_get_soft_ver_pack(char *buf,	int result,      LPNET_CMU_SOFT_VERSION lp_soft_ver)
{
	mxml_node_t	*xml, *response, *result_code, *parameters, *puId, *version, *buildTime;
	char cValue[32] = "\0";

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", "GetPuSoftwareVersion");
	
	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	memset(cValue, 0, 32);
	strncpy(cValue, msGetPuErrorInfo(result), 32);
	mxmlNewText(result_code, 0,cValue);	
	
	if(result == PU_SUCCESS){
	
	parameters = mxmlNewElement(response, "parameters");
	
	puId = mxmlNewElement(parameters, "puId");
	mxmlNewText(puId, 0, lp_soft_ver->pu_id);						
	
	version = mxmlNewElement(parameters, "version");
	mxmlNewText(version, 0, "10.1.3");						

	buildTime = mxmlNewElement(parameters, "buildTime");
	mxmlNewText(buildTime, 0, "2012-09-25 09:30:00");						

	}
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);
	
	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
	
	return 0;
}

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
int handle_get_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len)
{
	int ret = -1, result = 0;
	unsigned long size = 0;
	
	switch(cmd_id)
	{
	#if 1
	case CMD_GET_BASICINFO:		// 设备基本参数
		{
			NET_CMU_BASIC_INFO basic;
				memset(&basic,0,sizeof(NET_CMU_BASIC_INFO));

			if(g_msg_proc)
				result = g_msg_proc(GET_PU_BASICINFO, (char*)&basic, &size, g_param);
            else
                LOG("msg_proc is NULL\n");
			/*
				strncpy(basic->pu_id, "2001_pusdk1", 32);
				strncpy(basic->pu_name, "pusdk", 32);
				strncpy(basic->pu_passwd, "pusdk", 32);
			*/
				printf("basic->pu_id%s\n",basic.pu_id);
				printf("basic->pu_name%s\n",basic.pu_name);
				printf("basic->pu_passwd%s\n",basic.pu_passwd);
				
			
			ret = build_get_basic_info_pack(buf, result,  &basic);
			break;
		}
    #endif
	case CMD_GET_IPINFO:		// 设备网络参数
		{
			NET_CMU_IP_INFO ip;
            //struct sockaddr_in local_name;
            //socklen_t name_len = sizeof(local_name);
			memset(&ip,0,sizeof(NET_CMU_IP_INFO));
			
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_IPINFO, (char*)&ip, &size, g_param);
            else
                LOG("msg_proc is NULL\n");
            //getsockname(skt, (struct sockaddr*)&local_name, &name_len);
	        //memcpy(ip.ip_addr, (char *)inet_ntoa(local_name.sin_addr), sizeof(ip.ip_addr));
			ret = build_get_ip_info_pack(buf, result,&ip);
			break;
		}
	case CMD_GET_USER:			// 设备用户参数
		{
			NET_CMU_USER_LIST user;
			memset(&user,0,sizeof(NET_CMU_USER_LIST));
			
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_USER, (char*)&user, &size, g_param);

			ret = build_get_user_info_pack(buf, result,  &user);
			break;
		}
	case CMD_GET_SERIALPORT:	// 设备串口参数
		{
			NET_CMU_SERIAL_PORT serial;
			memset(&serial,0,sizeof(NET_CMU_SERIAL_PORT));
			
			if(prase_get_serial_info_pack(buf, result,  &serial) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_SERIALPORT, (char*)&serial, &size, g_param);

				ret = build_get_serial_info_pack(buf, result,  &serial);
			}
			break;
		}
	case CMD_GET_INOUTDEV:		// 设备输入输出接口参数
		{
			NET_CMU_INOUT_DEV inout_dev;
			memset(&inout_dev,0,sizeof(NET_CMU_INOUT_DEV));

			
			if(g_msg_proc)
				result =  g_msg_proc(GET_PU_INOUTDEV, (char*)&inout_dev, &size, g_param);


			memset(buf,0,MAX_BUFFER_LEN);
			ret = build_get_inout_dev_pack(buf, result,  &inout_dev);
			
			if(inout_dev.in_list){

				free(inout_dev.in_list);
				inout_dev.in_list = NULL;
			}
			
			if(inout_dev.out_list){

				free(inout_dev.out_list);
				inout_dev.out_list = NULL;
			}

			break;
		}
	case CMD_GET_TIME:			// 设备时间参数
		{
			NET_CMU_DEV_TIME time;
			memset(&time,0,sizeof(NET_CMU_DEV_TIME));
			
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_TIME, (char*)&time, &size, g_param);

			ret = build_get_time_info_pack(buf, result,  &time);
			break;
		}
	case CMD_GET_VIDEOFORMAT:	// 设备图像制式参数
		{
			NET_CMU_VIDEO_FORMAT_LIST video_format;
			memset(&video_format,0,sizeof(NET_CMU_VIDEO_FORMAT_LIST));
			
			if(prase_get_video_format_pack(buf, result,  &video_format) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_VIDEOFORMAT, (char*)&video_format, &size, g_param);

				ret = build_get_video_format_pack(buf, result,  &video_format);

				if(video_format.channel_video_format){
					free(video_format.channel_video_format);
					video_format.channel_video_format = NULL;
				}
			}
			break;
		}
	case CMD_GET_IMAGEENCODEPARA:	// 设备图像编码参数
		{
			NET_CMU_IMG_ENCODE img_encode;
			memset(&img_encode,0,sizeof(NET_CMU_IMG_ENCODE));

			if(prase_get_img_encode_pack(buf, result,  &img_encode) == 0)
			{
                printf("Jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n");
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_IMAGEENCODEPARA, (char*)&img_encode, &size, g_param);

				ret = build_get_img_encode_pack(buf, result,  &img_encode);
			}
			break;
		}
	case CMD_GET_IMAGEDISPLAYPARA:	// 设备图像显示参数
		{
			NET_CMU_IMG_DISPLAY img_display;
			memset(&img_display,0,sizeof(NET_CMU_IMG_DISPLAY));


			
			if(prase_get_img_display_pack(buf, result,  &img_display) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_IMAGEDISPLAYPARA, (char*)&img_display, &size, g_param);

				ret = build_get_img_display_pack(buf, result,  &img_display);
			}
			break;
		}
	case CMD_GET_IMAGETEXTPARA:		// 设备图像相关的文字参数
		{
			NET_CMU_IMG_TEXT img_text;
			memset(&img_text,0,sizeof(NET_CMU_IMG_TEXT));
			
			if(prase_get_img_text_pack(buf, result,  &img_text) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_IMAGETEXTPARA, (char*)&img_text, &size, g_param);

				ret = build_get_img_text_pack(buf, result,   &img_text);
				if(ret == 0)
				{
					printf("--------------++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n") ;
					//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);
				}
			}
			break;
		}
	case CMD_GET_ALARMPARAMS:		// 设备基本告警参数
		{
			NET_CMU_BASIC_ALARM basic_alarm;
			memset(&basic_alarm,0,sizeof(NET_CMU_BASIC_ALARM));

			/*
			if(prase_get_basic_alarm_pack(buf, result,  &basic_alarm) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_ALARMPARAMS, (char*)&basic_alarm, &size, g_param);

				ret = build_get_basic_alarm_pack(buf, result,  &basic_alarm);
			}
			*/

			memset(buf,0,MAX_BUFFER_LEN);
			result = 17;
			ret = build_get_basic_alarm_pack(buf, result,  &basic_alarm);
			break;
		}
	case CMD_GET_GIPINALARM:		// 设备Gpin告警参数
		{
			NET_CMU_GPIN_ALARM_INFO gpin;
			memset(&gpin,0,sizeof(NET_CMU_GPIN_ALARM_INFO));

			/*
			if(prase_get_gpin_alarm_pack(buf, result,  &gpin) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_GIPINALARM, (char*)&gpin, &size, g_param);

				ret = build_get_gpin_alarm_pack(buf, result,  &gpin);
			}

			*/

			memset(buf,0,MAX_BUFFER_LEN);
			result = 17;
			ret = build_get_gpin_alarm_pack(buf, result,  &gpin);
			break;
		}
	case CMD_GET_VIDEOLOSE:			// 设备视频丢失告警参数
		{
			NET_CMU_VIDEO_LOSE video_lost;
			memset(&video_lost,0,sizeof(NET_CMU_VIDEO_LOSE));

			/*
			if(prase_get_video_lost_pack(buf, result,  &video_lost) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_VIDEOLOSE, (char*)&video_lost, &size, g_param);

				ret = build_get_video_lost_pack(buf, result,  &video_lost);
			}

			*/

			memset(buf,0,MAX_BUFFER_LEN);
			result = 17;
			ret = build_get_video_lost_pack(buf, result,  &video_lost);
			
			break;
		}
	case CMD_GET_HIDEDETECTION:		// 设备视频遮蔽告警参数
		{
			NET_CMU_HIDE_DETECT hide_detect;
			memset(&hide_detect,0,sizeof(NET_CMU_HIDE_DETECT));

			/*
			if(prase_get_hide_detect_pack(buf, result,  &hide_detect) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_HIDEDETECTION, (char*)&hide_detect, &size, g_param);

				ret = build_get_hide_detect_pack(buf, result,  &hide_detect);
			}

			*/

			memset(buf,0,MAX_BUFFER_LEN);
			result = 17;
			ret = build_get_hide_detect_pack(buf, result,  &hide_detect);
			
			break;
		}
	case CMD_GET_MOTIONDETECTION:	// 设备移动侦测告警初始化参数
		{
			NET_CMU_MOTION_DETECT motion_detect;
			memset(&motion_detect,0,sizeof(NET_CMU_MOTION_DETECT));

		
			if(prase_get_motion_detect_pack(buf, result,  &motion_detect) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_MOTIONDETECTION, (char*)&motion_detect, &size, g_param);


				memset(buf,0,MAX_BUFFER_LEN);
				ret = build_get_motion_detect_pack(buf, result,  &motion_detect);

				if(motion_detect.block_lists){

					free(motion_detect.block_lists);
					motion_detect.block_lists = NULL;
					}
			}

			break;
		}
	case CMD_GET_IMAGEHIDEAREA:		// 设备图像遮挡区域
		{
			NET_CMU_IMG_HIDE_AREA hide_area;
			memset(&hide_area,0,sizeof(NET_CMU_IMG_HIDE_AREA));
			
			if(prase_get_img_hide_area_pack(buf, result,  &hide_area) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_IMAGEHIDEAREA, (char*)&hide_area, &size, g_param);
				
				memset(buf,0,MAX_BUFFER_LEN);
				ret = build_get_img_hide_area_pack(buf, result,   &hide_area);
				
				if(hide_area.area_lists){
				
					free(hide_area.area_lists);
					hide_area.area_lists = NULL;

				}
				
			}
			break;
		}
	case CMD_GET_LOCALSTORAGETASK:	// 设备本地存储参数
		{

		  /*
			NET_CMU_STORAGE_TASK storage_task;
			memset(&storage_task,0,sizeof(NET_CMU_IMG_HIDE_AREA));
		
			if(prase_get_storage_task_pack(buf, result,  &storage_task) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_LOCALSTORAGETASK, (char*)&storage_task, &size, g_param);


				memset(buf,0,MAX_BUFFER_LEN);
				ret = build_get_storage_task_pack(buf, result,  &storage_task);

				if(storage_task.schedule_lists){
					free(storage_task.schedule_lists);
					storage_task.schedule_lists = NULL;
				}
				
			}

			*/
			
			break;
		}
	case CMD_GET_PRESETPTZ:			// 设备云台预置点
		{
			NET_CMU_PRESET_LIST ptz_preset;
			memset(&ptz_preset,0,sizeof(NET_CMU_PRESET_LIST));
			
			if(prase_get_ptz_preset_pack(buf, result,  &ptz_preset) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_PRESETPTZ, (char*)&ptz_preset, &size, g_param);
				
				ret = build_get_ptz_preset_pack(buf, result,  &ptz_preset);

				if (ptz_preset.preset_lists)
					free(ptz_preset.preset_lists);
					ptz_preset.preset_lists = NULL;
			}
			break;
		}
	case CMD_GET_DEFAULTPRESETPTZ:	// 设备默认云台预置点
		{
			NET_CMU_DEFAULT_PRESET default_preset;
			memset(&default_preset,0,sizeof(NET_CMU_DEFAULT_PRESET));
			
			if(prase_get_default_preset_pack(buf, result,  &default_preset) == 0)
			{
				if(g_msg_proc)
					result = g_msg_proc(GET_PU_DEFAULTPRESETPTZ, (char*)&default_preset, &size, g_param);

				ret = build_get_default_preset_pack(buf, result,  &default_preset);
			}
			break;
		}
	case CMD_GET_CRUISETRACK:		// 设备巡航轨迹
		{
			NET_CMU_CRUISE_TRACK_LIST cruise_list;
			memset(&cruise_list,0,sizeof(NET_CMU_CRUISE_TRACK_LIST));
			
	/*
			if(prase_get_default_preset_pack(buf, result,  &cruise_list) == 0)
			{
				if(g_msg_proc)
					g_msg_proc(GET_PU_CRUISETRACK, (char*)&cruise_list, &size, g_param);
				ret = build_get_default_preset_pack(buf, result,  &cruise_list);
			}

	*/

			memset(buf,0,MAX_BUFFER_LEN);
			result = 2;
			//ret = build_get_default_preset_pack(buf, result,  &cruise_list);	
			
			break;
		}
	case CMD_GET_STATE:				// 设备状态信息
		{
			NET_CMU_DEV_STATE dev_state;
			memset(&dev_state,0,sizeof(NET_CMU_DEV_STATE));

			memset(&dev_state,0,sizeof(NET_CMU_DEV_STATE));
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_STATE, (char*)&dev_state, &size, g_param);

			ret = build_get_dev_state_pack(buf, result,  &dev_state);

			free(dev_state.disk_status_lists);
			free(dev_state.video_channel_lists);
			free(dev_state.alarm_out_lists);
			free(dev_state.alarm_in_lists);
			
			break;
		}
	case CMD_GET_LOG:				// 设备日志信息
		{
			NET_CMU_DEV_LOG dev_log;
			memset(&dev_log,0,sizeof(NET_CMU_DEV_LOG));
			
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_LOG, (char*)&dev_log, &size, g_param);

			ret = build_get_dev_log_pack(buf, result,  &dev_log);
			break;
		}
	case CMD_GET_SOFTWAREVERSION:	// 设备软件版本信息
		{
			NET_CMU_SOFT_VERSION software_ver;
			memset(&software_ver,0,sizeof(NET_CMU_SOFT_VERSION));
			
			if(g_msg_proc)
				result = g_msg_proc(GET_PU_SOFTWAREVERSION, (char*)&software_ver, &size, g_param);
			
			ret = build_get_soft_ver_pack(buf, result,  &software_ver);
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

		printf("resp.ceeq is %d\n",resp.cseq);
		printf("resp.ver is %d\n",resp.ver);
		printf("resp.length is %d\n",resp.length);

		if( os_socket_send(skt, &resp, sizeof(resp), 1000) < 0)
		{
			LOG("cmu get_info send info header fail. errno:%d,%s",errno,strerror(errno));
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
