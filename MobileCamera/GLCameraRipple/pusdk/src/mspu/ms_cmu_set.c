#include "stdafx.h"
#include "ms_cmu_set.h"
#include "ms_pu_glob.h"

#include "ms_pu_param.h"
#include "ms_cmu_cmd.h"

#include "mxml.h"
#include "md5.h"
#include "oslib.h"
//#include "ms_msg_parse.h"
#include "iconv.h"
#include "XmlLntercept.h"
#include "ossys.h"

char Replacement_parts[128] = "" ;
char Replacement_parts_preset[128] = "" ;


/*------------------------SetPuBasicInfo-------------------------------------*/
int parse_set_basic_info_pack(char *buf,int result,LPNET_CMU_BASIC_INFO basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *puId, *puName, *puPassword, *pNode;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		puId = mxmlFindElement(parameters, xml, "puId", NULL, NULL, MXML_DESCEND);
		if(puId != NULL && puId->type == MXML_ELEMENT)
		{
			pNode = puId->child;

			if(pNode  != NULL){

				//printf("<<<  pu_id = %s  >>>.\n", pNode->value.text.string);
				strcpy(basic_info->pu_id, pNode->value.text.string);
			}
		}

		puName = mxmlFindElement(parameters, xml, "puName", NULL, NULL, MXML_DESCEND);
		if(puName != NULL && puName->type == MXML_ELEMENT)
		{
			pNode = puName->child;

			if(pNode  != NULL){

				//printf("<<<  pu_name = %s  >>>.\n", pNode->value.text.string);
				strcpy(basic_info->pu_name, pNode->value.text.string);
			}
		}

		puPassword = mxmlFindElement(parameters, xml, "puPassword", NULL, NULL, MXML_DESCEND);
		if(puPassword != NULL && puPassword->type == MXML_ELEMENT)
		{
			pNode = puPassword->child;
			if(pNode  != NULL){

				//printf("<<<  pu_passwd = %s  >>>.\n", pNode->value.text.string);
				strcpy(basic_info->pu_passwd, pNode->value.text.string);

			}
		}
	}

	mxmlDelete(xml);
	return ret;
}



int build_set_basic_info_pack(char *buf, int result,  LPNET_CMU_BASIC_INFO basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_BASICINFO);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;

}


/*------------------------SetPuIpInfo-------------------------------------*/


int parse_set_ip_info_pack(char *buf,   int result,   LPNET_CMU_IP_INFO basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *netId, *pNode, *ipAddr, *ctrlPort, *netMask,  *dnsAddr, *gateWay; 

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		netId = mxmlFindElement(parameters, xml, "netId", NULL, NULL, MXML_DESCEND);
		if(netId != NULL && netId->type == MXML_ELEMENT)
		{
			pNode = netId->child;
			if(pNode  != NULL){
				//printf("<<<  net_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->net_id= atoi(pNode->value.text.string);

			}

		}

		ipAddr = mxmlFindElement(parameters, xml, "ipAddress", NULL, NULL, MXML_DESCEND);
		if(ipAddr != NULL && ipAddr->type == MXML_ELEMENT)
		{
			pNode = ipAddr->child;

			if(pNode  != NULL){
				//printf("<<<  ip_addr = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->ip_addr, pNode->value.text.string,15);
			}

		}

		ctrlPort = mxmlFindElement(parameters, xml, "controlPort", NULL, NULL, MXML_DESCEND);
		if(ctrlPort != NULL && ctrlPort->type == MXML_ELEMENT)
		{
			pNode = ctrlPort->child;

			if(pNode  != NULL){

				//printf("<<<  ctrl_port = %s  >>>.\n", pNode->value.text.string);
				basic_info->ctrl_port= atoi(pNode->value.text.string);
			}
		}

		netMask = mxmlFindElement(parameters, xml, "subnetMask", NULL, NULL, MXML_DESCEND);
		if(netMask != NULL && netMask->type == MXML_ELEMENT)
		{
			pNode = netMask->child;

			if(pNode  != NULL){
				//printf("<<< net_mask = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->net_mask, pNode->value.text.string,15);
			}
		}

		dnsAddr = mxmlFindElement(parameters, xml, "dns", NULL, NULL, MXML_DESCEND);
		if(dnsAddr != NULL && dnsAddr->type == MXML_ELEMENT)
		{
			pNode = dnsAddr->child;

			if(pNode  != NULL){
				//printf("<<<dns_addr = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->dns_addr[0], pNode->value.text.string,15);
			}	
		}

		gateWay = mxmlFindElement(parameters, xml, "gateway", NULL, NULL, MXML_DESCEND);
		if(gateWay != NULL && gateWay->type == MXML_ELEMENT)
		{
			pNode = gateWay->child;

			if(pNode  != NULL){
				//printf("<<<  gate_way = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->gate_way, pNode->value.text.string,15);
			}
		}

	}

	mxmlDelete(xml);
	return ret;
}


int build_set_ip_info_pack(char *buf, int result,  LPNET_CMU_IP_INFO basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters; 

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_IPINFO);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuUser-----------------------------------*/
int parse_set_puUser_info_pack(char *buf,   int result,   LPNET_CMU_USER basic_info)
{
	int ret = -1;

	mxml_node_t	*xml, *parameters, *userName, *passWord, *pNode;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		userName = mxmlFindElement(parameters, xml, "username", NULL, NULL, MXML_DESCEND);
		if(userName != NULL && userName->type == MXML_ELEMENT)
		{
			pNode = userName->child;

			if(pNode  != NULL){
				//printf("<<<  user_name = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->user_name, pNode->value.text.string,16);
			}
		}

		passWord = mxmlFindElement(parameters, xml, "password", NULL, NULL, MXML_DESCEND);
		if(passWord != NULL && passWord->type == MXML_ELEMENT)
		{
			pNode = passWord->child;

			if(pNode  != NULL){
				//printf("<<<  pass_word = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->pass_word, pNode->value.text.string,32);

			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}


int build_set_puUser_info_pack(char *buf, int result,  LPNET_CMU_USER basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_USER);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuSerialPort-----------------------------------*/
int parse_set_puSerialPort_info_pack(char *buf,   int result,   LPNET_CMU_SERIAL_PORT basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *forDecoder, *pNode, *serialPort;
	mxml_node_t	*workMode, *videoId, *decoderType, *decoderAddress;
	mxml_node_t *baudRate, *dataBit, *parity, *stopBit, *timeOut, *flowControl;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;
		forDecoder = mxmlFindElement(parameters, xml, "forDecoder", NULL, NULL, MXML_DESCEND);
		if(forDecoder != NULL && forDecoder->type == MXML_ELEMENT)
		{
			pNode = forDecoder->child;

			if(pNode  != NULL){
				//printf("<<<  for_decoder = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->for_decoder, pNode->value.text.string,5);

			}
		}
		if(strcmp(basic_info->for_decoder,"true") !=0)
		{

			serialPort = mxmlFindElement(parameters, xml, "serialPort", NULL, NULL, MXML_DESCEND);
			if(serialPort != NULL && serialPort->type == MXML_ELEMENT)
			{
				pNode = serialPort->child;

				if(pNode  != NULL){
					//printf("<<<  serial_port = %s  >>>.\n", pNode->value.text.string);
					basic_info->serial_port= atoi(pNode->value.text.string);
				}
			}

			workMode = mxmlFindElement(parameters, xml, "workMode", NULL, NULL, MXML_DESCEND);
			if(workMode != NULL && workMode->type == MXML_ELEMENT)
			{
				pNode = workMode->child;

				if(pNode  != NULL){
					//printf("<<<  work_mode = %s  >>>.\n", pNode->value.text.string);
					basic_info->work_mode= atoi(pNode->value.text.string);

				}
			}
		}
		else
		{
			videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
			if(videoId != NULL && videoId->type == MXML_ELEMENT)
			{
				pNode = videoId->child;

				if(pNode  != NULL){
					//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
					basic_info->video_id= atoi(pNode->value.text.string);
				}

			}

			decoderType = mxmlFindElement(parameters, xml, "decoderType", NULL, NULL, MXML_DESCEND);
			if(decoderType != NULL && decoderType->type == MXML_ELEMENT)
			{
				pNode = decoderType->child;

				if(pNode  != NULL){
					//printf("<<<  decoder_type = %s  >>>.\n", pNode->value.text.string);
					basic_info->decoder_type= atoi(pNode->value.text.string);
				}
			}

			decoderAddress = mxmlFindElement(parameters, xml, "decoderAddress", NULL, NULL, MXML_DESCEND);
			if(decoderAddress != NULL && decoderAddress->type == MXML_ELEMENT)
			{
				pNode = decoderAddress->child;

				if(pNode  != NULL){
					//printf("<<<  decoder_address = %s  >>>.\n", pNode->value.text.string);
					basic_info->decoder_address= atoi(pNode->value.text.string);
				}
			}
		}

		baudRate = mxmlFindElement(parameters, xml, "baudRate", NULL, NULL, MXML_DESCEND);
		if(baudRate != NULL && baudRate->type == MXML_ELEMENT)
		{
			pNode = baudRate->child;

			if(pNode  != NULL){

				//printf("<<<  baud_rate = %s  >>>.\n", pNode->value.text.string);
				basic_info->baud_rate= atoi(pNode->value.text.string);
			}

		}

		dataBit = mxmlFindElement(parameters, xml, "dataBit", NULL, NULL, MXML_DESCEND);
		if(dataBit != NULL && dataBit->type == MXML_ELEMENT)
		{
			pNode = dataBit->child;

			if(pNode  != NULL){

				//printf("<<<  data_bit = %s  >>>.\n", pNode->value.text.string);
				basic_info->data_bit= atoi(pNode->value.text.string);
			}
		}

		parity = mxmlFindElement(parameters, xml, "parity", NULL, NULL, MXML_DESCEND);
		if(parity != NULL && parity->type == MXML_ELEMENT)
		{
			pNode = parity->child;

			if(pNode  != NULL){
				//printf("<<<  parity = %s  >>>.\n", pNode->value.text.string);
				basic_info->parity= atoi(pNode->value.text.string);
			}
		}

		stopBit = mxmlFindElement(parameters, xml, "stopBit", NULL, NULL, MXML_DESCEND);
		if(stopBit != NULL && stopBit->type == MXML_ELEMENT)
		{
			pNode = stopBit->child;

			if(pNode  != NULL){
				//printf("<<<  stop_bit = %s  >>>.\n", pNode->value.text.string);
				basic_info->stop_bit= atoi(pNode->value.text.string);
			}
		}

		timeOut = mxmlFindElement(parameters, xml, "timeOut", NULL, NULL, MXML_DESCEND);
		if(timeOut != NULL && timeOut->type == MXML_ELEMENT)
		{
			pNode = timeOut->child;

			if(pNode  != NULL){
				//printf("<<<  time_out = %s  >>>.\n", pNode->value.text.string);
				basic_info->time_out= atoi(pNode->value.text.string);
			}
		}

		flowControl = mxmlFindElement(parameters, xml, "flowControl", NULL, NULL, MXML_DESCEND);
		if(flowControl != NULL && flowControl->type == MXML_ELEMENT)
		{
			pNode = flowControl->child;

			if(pNode  != NULL){
				//printf("<<<  flow_control = %s  >>>.\n", pNode->value.text.string);
				basic_info->flow_control= atoi(pNode->value.text.string);
			}
		}
	}

	mxmlDelete(xml);
	return ret;
}


int build_set_puSerialPort_info_pack(char *buf, int result,  LPNET_CMU_SERIAL_PORT basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_SERIALPORT);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuOutDev-----------------------------------*/
int parse_set_inout_info_pack(char *buf,   int result,   LPNET_CMU_INOUT_INFO basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *channelNum, *state, *pNode;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		channelNum = mxmlFindElement(parameters, xml, "ChannelNumber", NULL, NULL, MXML_DESCEND);
		if(channelNum != NULL && channelNum->type == MXML_ELEMENT)
		{
			pNode = channelNum->child;

			if(pNode  != NULL){
				//printf("<<<  channel_num = %s  >>>.\n", pNode->value.text.string);
				basic_info->channel_num= atoi(pNode->value.text.string);
			}
		}

		state = mxmlFindElement(parameters, xml, "state", NULL, NULL, MXML_DESCEND);
		if(state != NULL && state->type == MXML_ELEMENT)
		{
			pNode = state->child;

			if(pNode  != NULL){
				//printf("<<<  state = %s  >>>.\n", pNode->value.text.string);
				basic_info->state= atoi(pNode->value.text.string);
			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_inout_info_pack(char *buf, int result,  LPNET_CMU_INOUT_INFO basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_OUTDEV);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");
	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuTime-----------------------------------*/	
int parse_set_time_info_pack(char *buf,   int result,   LPNET_CMU_DEV_TIME basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *hour, *pNode, *minute, *second, *month, *date, *year;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		hour = mxmlFindElement(parameters, xml, "hour", NULL, NULL, MXML_DESCEND);
		if(hour != NULL && hour->type == MXML_ELEMENT)
		{
			pNode = hour->child;

			if(pNode  != NULL){

				//	printf("<<<  hour = %s  >>>.\n", pNode->value.text.string);
				basic_info->hour= atoi(pNode->value.text.string);

			}

		}

		minute = mxmlFindElement(parameters, xml, "minute", NULL, NULL, MXML_DESCEND);
		if(minute != NULL && minute->type == MXML_ELEMENT)
		{
			pNode = minute->child;

			if(pNode  != NULL){
				//	printf("<<<  minute = %s  >>>.\n", pNode->value.text.string);
				basic_info->minute= atoi(pNode->value.text.string);
			}

		}

		second = mxmlFindElement(parameters, xml, "second", NULL, NULL, MXML_DESCEND);
		if(second != NULL && second->type == MXML_ELEMENT)
		{
			pNode = second->child;

			if(pNode  != NULL){
				//	printf("<<<  second = %s  >>>.\n", pNode->value.text.string);
				basic_info->second= atoi(pNode->value.text.string);

			}
		}

		month = mxmlFindElement(parameters, xml, "month", NULL, NULL, MXML_DESCEND);
		if(month != NULL && month->type == MXML_ELEMENT)
		{
			pNode = month->child;

			if(pNode  != NULL){
				//	printf("<<<  month = %s  >>>.\n", pNode->value.text.string);
				basic_info->month= atoi(pNode->value.text.string);
			}

		}

		date = mxmlFindElement(parameters, xml, "date", NULL, NULL, MXML_DESCEND);
		if(date != NULL && date->type == MXML_ELEMENT)
		{
			pNode = date->child;

			if(pNode  != NULL){
				//	printf("<<<  date = %s  >>>.\n", pNode->value.text.string);
				basic_info->date= atoi(pNode->value.text.string);
			}

		}

		year = mxmlFindElement(parameters, xml, "year", NULL, NULL, MXML_DESCEND);
		if(year != NULL && year->type == MXML_ELEMENT)
		{
			pNode = year->child;

			if(pNode  != NULL){
				//	printf("<<<  year = %s  >>>.\n", pNode->value.text.string);
				basic_info->year= atoi(pNode->value.text.string);
			}
		}
	}

	mxmlDelete(xml);
	return ret;
}

int public_set_time_info_pack(char *buf, int result,  LPNET_CMU_DEV_TIME basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_TIME);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuVideoFormat-----------------------------------*/
int parse_set_Videoformat_info_pack(char *buf,   int result,   LPNET_CMU_VIDEO_FORMAT basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *videoFormat, *pNode;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}
		}

		videoFormat = mxmlFindElement(parameters, xml, "encodeMode", NULL, NULL, MXML_DESCEND);
		if(videoFormat != NULL && videoFormat->type == MXML_ELEMENT)
		{
			pNode = videoFormat->child;

			if(pNode  != NULL){
				printf("<<<  video_format = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_format= atoi(pNode->value.text.string);

			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_Videoformat_info_pack(char *buf, int result,  LPNET_CMU_VIDEO_FORMAT basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_VIDEOFORMAT);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuImageEncodePara-----------------------------------*/
int parse_set_PuImageEncode_info_pack(char *buf,   int result,   LPNET_CMU_IMG_ENCODE basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode, *encodMode, *picQuality, *bitRate;
	mxml_node_t *bitRateType, *frameRate,  *imageSize, *streamType, *iFrameInterval;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);

			}
		}

		encodMode = mxmlFindElement(parameters, xml, "encodeMode", NULL, NULL, MXML_DESCEND);
		if(encodMode != NULL && encodMode->type == MXML_ELEMENT)
		{
			pNode = encodMode->child;

			if(pNode  != NULL){
				//printf("<<<  encode_mode = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->encode_mode, pNode->value.text.string,32);

			}
		}

		picQuality = mxmlFindElement(parameters, xml, "picQuality", NULL, NULL, MXML_DESCEND);
		if(picQuality != NULL && picQuality->type == MXML_ELEMENT)
		{
			pNode = picQuality->child;

			if(pNode  != NULL){
				//printf("<<<  pic_quality = %s  >>>.\n", pNode->value.text.string);
				basic_info->pic_quality= atoi(pNode->value.text.string);
			}
		}

		bitRate = mxmlFindElement(parameters, xml, "bitRate", NULL, NULL, MXML_DESCEND);
		if(bitRate != NULL && bitRate->type == MXML_ELEMENT)
		{
			pNode = bitRate->child;

			if(pNode  != NULL){
				//printf("<<<  bit_rate = %s  >>>.\n", pNode->value.text.string);
				basic_info->bit_rate= atoi(pNode->value.text.string);
			}
		}

		bitRateType = mxmlFindElement(parameters, xml, "bitRateType", NULL, NULL, MXML_DESCEND);
		if(bitRateType != NULL && bitRateType->type == MXML_ELEMENT)
		{
			pNode = bitRateType->child;

			if(pNode  != NULL){
				//printf("<<<  bit_rate_type = %s  >>>.\n", pNode->value.text.string);
				basic_info->bit_rate_type= atoi(pNode->value.text.string);
			}
		}

		frameRate = mxmlFindElement(parameters, xml, "frameRate", NULL, NULL, MXML_DESCEND);
		if(frameRate != NULL && frameRate->type == MXML_ELEMENT)
		{
			pNode = frameRate->child;

			if(pNode  != NULL){
				//printf("<<<  video_format = %s  >>>.\n", pNode->value.text.string);
				basic_info->frame_rate= atoi(pNode->value.text.string);

			}
		}

		imageSize = mxmlFindElement(parameters, xml, "imageSize", NULL, NULL, MXML_DESCEND);
		if(imageSize != NULL && imageSize->type == MXML_ELEMENT)
		{
			pNode = imageSize->child;

			if(pNode  != NULL){
				//printf("<<<  image_size = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->image_size, pNode->value.text.string,32);
			}
		}

		streamType = mxmlFindElement(parameters, xml, "streamType", NULL, NULL, MXML_DESCEND);
		if(streamType != NULL && streamType->type == MXML_ELEMENT)
		{
			pNode = streamType->child;

			if(pNode  != NULL){
				//printf("<<<  streamType = %s  >>>.\n", pNode->value.text.string);
				basic_info->stream_type = atoi(pNode->value.text.string);
			}
		}

		iFrameInterval = mxmlFindElement(parameters, xml, "iFrameInterval", NULL, NULL, MXML_DESCEND);
		if(iFrameInterval != NULL && iFrameInterval->type == MXML_ELEMENT)
		{
			pNode = iFrameInterval->child;

			if(pNode  != NULL){
				//printf("<<<  iframe_interval = %s  >>>.\n", pNode->value.text.string);
				basic_info->iframe_interval= atoi(pNode->value.text.string);

			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_PuImageEncode_info_pack(char *buf, int result,  LPNET_CMU_IMG_ENCODE basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_IMAGEENCODEPARA);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuImageDisplayPara-----------------------------------*/
int parse_set_PuImageDisplay_info_pack(char *buf,   int result,   LPNET_CMU_IMG_DISPLAY basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode, *contrast, *bright, *hue, *saturation; 

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}

		}

		contrast = mxmlFindElement(parameters, xml, "contrast", NULL, NULL, MXML_DESCEND);
		if(contrast != NULL && contrast->type == MXML_ELEMENT)
		{
			pNode = contrast->child;

			if(pNode  != NULL){
				//printf("<<<  contrast = %s  >>>.\n", pNode->value.text.string);
				basic_info->contrast= atoi(pNode->value.text.string);
			}
		}

		bright = mxmlFindElement(parameters, xml, "bright", NULL, NULL, MXML_DESCEND);
		if(bright != NULL && bright->type == MXML_ELEMENT)
		{
			pNode = bright->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->bright= atoi(pNode->value.text.string);

			}
		}

		hue = mxmlFindElement(parameters, xml, "hue", NULL, NULL, MXML_DESCEND);
		if(hue != NULL && hue->type == MXML_ELEMENT)
		{
			pNode = hue->child;

			if(pNode  != NULL){
				//printf("<<<  hue = %s  >>>.\n", pNode->value.text.string);
				basic_info->hue= atoi(pNode->value.text.string);

			}
		}

		saturation = mxmlFindElement(parameters, xml, "saturation", NULL, NULL, MXML_DESCEND);
		if(saturation != NULL && saturation->type == MXML_ELEMENT)
		{
			pNode = saturation->child;

			if(pNode  != NULL){
				//printf("<<<  saturation = %s  >>>.\n", pNode->value.text.string);
				basic_info->saturation= atoi(pNode->value.text.string);

			}
		}
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_PuImageDisplay_info_pack(char *buf, int result,  LPNET_CMU_IMG_DISPLAY basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_IMAGEDISPLAYPARA);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuImageTextPara-----------------------------------*/
int parse_set_ImageTextPara_info_pack(char *buf,   int result,   LPNET_CMU_IMG_TEXT basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *videoId, *pNode, *timeEnable, *timeX, *timeY;
	mxml_node_t *textEnable, *text, *textX, *textY; 


	//char blankStr[50] = {0};
	//char parseName[50] = "presetName";
	//char *in_gb2312 = buf;
	//char *encodeBuf[MAX_BUFFER_LEN] = {0};
	char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	//char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
	sscanf(buf,"%[^>]",p) ;
	strcat(firstbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , firstbuf , 1024) ;
	char *tmp = NULL;
	tmp = XmlLntercept(buf) ;
	xml = mxmlLoadString(NULL, tmp, NULL);
  

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}
		}

		timeEnable = mxmlFindElement(parameters, xml, "bitmapTimeEnable", NULL, NULL, MXML_DESCEND);
		if(timeEnable != NULL && timeEnable->type == MXML_ELEMENT)
		{
			pNode = timeEnable->child;

			if(pNode  != NULL){
				//printf("<<<  time_enable = %s  >>>.\n", pNode->value.text.string);
				//basic_info->time_enable= atoi(pNode->value.text.string);
				strncpy(basic_info->time_enable,pNode->value.text.string,5);
			}
		}

		timeX = mxmlFindElement(parameters, xml, "bitmapX", NULL, NULL, MXML_DESCEND);
		if(timeX != NULL && timeX->type == MXML_ELEMENT)
		{
			pNode = timeX->child;

			if(pNode  != NULL){
				//printf("<<<  time_X = %s  >>>.\n", pNode->value.text.string);
				basic_info->time_X= atoi(pNode->value.text.string);

			}
		}

		timeY = mxmlFindElement(parameters, xml, "bitmapY", NULL, NULL, MXML_DESCEND);
		if(timeY != NULL && timeY->type == MXML_ELEMENT)
		{
			pNode = timeY->child;

			if(pNode  != NULL){
				//printf("<<<  time_Y = %s  >>>.\n", pNode->value.text.string);
				basic_info->time_Y= atoi(pNode->value.text.string);

			}
		}

		textEnable = mxmlFindElement(parameters, xml, "bitmapTextEnable", NULL, NULL, MXML_DESCEND);
		if(textEnable != NULL && textEnable->type == MXML_ELEMENT)
		{
			pNode = textEnable->child;

			if(pNode  != NULL){
				//printf("<<<  text_enable = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->text_enable,pNode->value.text.string,5);
				//strncpy(basic_info->text_enable,"false",5);				
			}
		}

		text = mxmlFindElement(parameters, xml, "bitmapText", NULL, NULL, MXML_DESCEND);

		if(text != NULL && text->type == MXML_ELEMENT  )
		{
			pNode = text->child;

			if(pNode != NULL){

				//printf("<<<  text = %s  >>>.\n", pNode->value.text.string);

				strncpy(basic_info->text,Replacement_parts,128);
				memset(Replacement_parts , 0 , 128) ;

			}

		}

		textX = mxmlFindElement(parameters, xml, "bitmapTextX", NULL, NULL, MXML_DESCEND);
		if(textX != NULL && textX->type == MXML_ELEMENT)
		{
			pNode = textX->child;

			if(pNode != NULL){

				//printf("<<<  text_X = %s  >>>.\n", pNode->value.text.string);
				basic_info->text_X= atoi(pNode->value.text.string);

			}

		}

		textY = mxmlFindElement(parameters, xml, "bitmapTextY", NULL, NULL, MXML_DESCEND);
		if(textY != NULL && textY->type == MXML_ELEMENT)
		{
			pNode = textY->child;

			if(pNode  != NULL){

				//printf("<<<  text_Y = %s  >>>.\n", pNode->value.text.string);
				basic_info->text_Y= atoi(pNode->value.text.string);

			}
		}
	}
    if(tmp != NULL)
    {
        free(tmp);
        tmp = NULL;
    }
	mxmlDelete(xml);
   
	return ret;
}

int build_set_ImageTextPara_info_pack(char *buf, int result,  LPNET_CMU_IMG_TEXT basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;
	//char *tmp = NULL ;	
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_IMAGETEXTPARA);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);

	parameters = mxmlNewElement(response, "parameters");
	//    	meiDianFlag = mxmlNewElement(parameters , "meiDianFlag")        ;
	//	mxmlElementSetAttrf(meiDianFlag, "code", "1");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	sscanf(buf,"%[^>]",p) ;
	strcat(secondbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , secondbuf , 1024) ;
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;

}

/*-------------------------SetPuAlarmParams-----------------------------------*/
int parse_set_PuAlarmParams_info_pack(char *buf,   int result,   LPNET_CMU_BASIC_ALARM basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *pNode, *alarmInChannel, *alarmEnabled;
	mxml_node_t *isCopyToall, *alarmOutput, *alarmRecord,*activatedTimes, *shootPhoto,*alarmPtzAction;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		alarmInChannel = mxmlFindElement(parameters, xml, "alarmInChannel", NULL, NULL, MXML_DESCEND);
		if(alarmInChannel != NULL && alarmInChannel->type == MXML_ELEMENT)
		{
			pNode = alarmInChannel->child;

			if(pNode  != NULL){

				//printf("<<<  alarm_in_channel = %s  >>>.\n", pNode->value.text.string);
				basic_info->alarm_in_channel= atoi(pNode->value.text.string);

			}
		}

		alarmEnabled = mxmlFindElement(parameters, xml, "alarmEnabled", NULL, NULL, MXML_DESCEND);
		if(alarmEnabled != NULL && alarmEnabled->type == MXML_ELEMENT)
		{
			pNode = alarmEnabled->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_enabled = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->alarm_enabled,pNode->value.text.string,5);

			}
		}

		isCopyToall = mxmlFindElement(parameters, xml, "isCopyToall", NULL, NULL, MXML_DESCEND);
		if(isCopyToall != NULL && isCopyToall->type == MXML_ELEMENT)
		{
			pNode = isCopyToall->child;

			if(pNode  != NULL){
				//printf("<<<  is_copy_toall = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->is_copy_toall,pNode->value.text.string,5);

			}
		}

		alarmOutput = mxmlFindElement(parameters, xml, "alarmOutput", NULL, NULL, MXML_DESCEND);
		if(alarmOutput != NULL && alarmOutput->type == MXML_ELEMENT)
		{
			pNode = alarmOutput->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(alarmOutput, "channel"))
				{
					basic_info->alarm_output.alarm_channel = atoi(mxmlElementGetAttr(alarmOutput, "channel"));
					basic_info->alarm_output.alarm_time= atoi(mxmlElementGetAttr(alarmOutput, "channel"));
				}
			}
		}

		alarmRecord  = mxmlFindElement(parameters, xml, "alarmRecord ", NULL, NULL, MXML_DESCEND);
		if(alarmRecord  != NULL && alarmRecord ->type == MXML_ELEMENT)
		{
			pNode = alarmRecord ->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(alarmOutput, "recodChannel"))
				{
					basic_info->alarm_record.record_channel= atoi(mxmlElementGetAttr(alarmOutput, "recodChannel"));
					basic_info->alarm_record.record_time= atoi(mxmlElementGetAttr(alarmOutput, "recordTime"));
					basic_info->alarm_record.precord_time= atoi(mxmlElementGetAttr(alarmOutput, "precodeTime"));
				}
			}
		}

		activatedTimes = mxmlFindElement(parameters, xml, "activatedTimes", NULL, NULL, MXML_DESCEND);
		if(activatedTimes != NULL && activatedTimes->type == MXML_ELEMENT)
		{
			pNode = activatedTimes->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(alarmOutput, "end"))
				{
					strncpy(basic_info->active_times.active_end,mxmlElementGetAttr(alarmOutput, "end"),5);		
				}
				else if(mxmlElementGetAttr(alarmOutput, "start"))
				{
					strncpy(basic_info->active_times.active_start,mxmlElementGetAttr(alarmOutput, "start"),5);
				}
			}
		}

		shootPhoto = mxmlFindElement(parameters, xml, "shootPhoto", NULL, NULL, MXML_DESCEND);
		if(shootPhoto != NULL && shootPhoto->type == MXML_ELEMENT)
		{
			pNode = shootPhoto->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(alarmOutput, "channel"))
				{
					basic_info->shot_photo.shot_channel= atoi(mxmlElementGetAttr(alarmOutput, "channel"));
					basic_info->shot_photo.shot_times= atoi(mxmlElementGetAttr(alarmOutput, "shotTimes"));	
				}
			}
		}

		alarmPtzAction = mxmlFindElement(parameters, xml, "alarmPtzAction", NULL, NULL, MXML_DESCEND);
		if(alarmPtzAction != NULL && alarmPtzAction->type == MXML_ELEMENT)
		{
			pNode = alarmPtzAction->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(alarmOutput, "ptzChannel"))
				{
					basic_info->ptz_preset.ptz_channel = atoi(mxmlElementGetAttr(alarmOutput, "ptzChannel"));		
				}
				else if(mxmlElementGetAttr(alarmOutput, "preset"))
				{
					basic_info->ptz_preset.ptz_preset = atoi(mxmlElementGetAttr(alarmOutput, "preset"));
				}
			}
		}
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_PuAlarmParams_info_pack(char *buf, int result,  LPNET_CMU_BASIC_ALARM basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_ALARMPARAMS);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);	
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuGpinAlarm-----------------------------------*/
int parse_set_GpinAlarm_info_pack(char *buf,   int result,   LPNET_CMU_GPIN_ALARM_INFO basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *pNode, *gpinId, *alarmEnabled,*alarmStatus, *gpinAlarmOutput, *gpinAlarmVideo;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		gpinId = mxmlFindElement(parameters, xml, "gpinId", NULL, NULL, MXML_DESCEND);
		if(gpinId != NULL && gpinId->type == MXML_ELEMENT)
		{
			pNode = gpinId->child;

			if(pNode  != NULL){
				//printf("<<<  gpin_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->gpin_id= atoi(pNode->value.text.string);
			}
		}

		alarmEnabled = mxmlFindElement(parameters, xml, "alarmEnabled", NULL, NULL, MXML_DESCEND);
		if(alarmEnabled != NULL && alarmEnabled->type == MXML_ELEMENT)
		{
			pNode = alarmEnabled->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_enabled = %s  >>>.\n", pNode->value.text.string);
				//basic_info->time_enable= atoi(pNode->value.text.string);
				strncpy(basic_info->alarm_enabled,pNode->value.text.string,5);	
			}
		}

		alarmStatus = mxmlFindElement(parameters, xml, "alarm_status", NULL, NULL, MXML_DESCEND);
		if(alarmStatus != NULL && alarmStatus->type == MXML_ELEMENT)
		{
			pNode = alarmStatus->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_status = %s  >>>.\n", pNode->value.text.string);
				basic_info->alarm_status= atoi(pNode->value.text.string);
			}
		}

		gpinAlarmOutput = mxmlFindElement(parameters, xml, "gpinAlarmOutput", NULL, NULL, MXML_DESCEND);
		if(gpinAlarmOutput != NULL && gpinAlarmOutput->type == MXML_ELEMENT)
		{
			pNode = gpinAlarmOutput->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(gpinAlarmOutput, "channel"))
				{
					basic_info->gpin_alarm_output.alarm_time = atoi(mxmlElementGetAttr(gpinAlarmOutput, "alarmTime"));
					basic_info->gpin_alarm_output.alarm_channel= atoi(mxmlElementGetAttr(gpinAlarmOutput, "channel"));
				}
			}
		}

		gpinAlarmVideo  = mxmlFindElement(parameters, xml, "gpin_alarm_video ", NULL, NULL, MXML_DESCEND);
		if(gpinAlarmVideo  != NULL && gpinAlarmVideo ->type == MXML_ELEMENT)
		{
			pNode = gpinAlarmVideo ->child;

			if(pNode  != NULL){
				if(mxmlElementGetAttr(gpinAlarmVideo, "recodChannel"))
				{
					basic_info->gpin_alarm_video.alarm_record_channel	= atoi(mxmlElementGetAttr(gpinAlarmVideo, "alarmRecordChannel"));
					basic_info->gpin_alarm_video.alarm_shot_channel = atoi(mxmlElementGetAttr(gpinAlarmVideo, "alarm_shot_channel"));
				}
			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_GpinAlarm_info_pack(char *buf, int result,  LPNET_CMU_GPIN_ALARM_INFO basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_GIPINALARM);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuVideoLose-----------------------------------*/
int parse_set_VideoLose_info_pack(char *buf,   int result,   LPNET_CMU_VIDEO_LOSE basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters, *pNode, *videoId, *alarmEnabled,*alarmOutput, *alarmState;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){

				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}
		}

		alarmEnabled = mxmlFindElement(parameters, xml, "alarmEnabled", NULL, NULL, MXML_DESCEND);
		if(alarmEnabled != NULL && alarmEnabled->type == MXML_ELEMENT)
		{
			pNode = alarmEnabled->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_enabled = %s  >>>.\n", pNode->value.text.string);
				//basic_info->time_enable= atoi(pNode->value.text.string);
				basic_info->alarm_enabled  = atoi(pNode->value.text.string);
			}
		}

		alarmOutput = mxmlFindElement(parameters, xml, "alarmOutput", NULL, NULL, MXML_DESCEND);
		if(alarmOutput != NULL && alarmOutput->type == MXML_ELEMENT)
		{
			pNode = alarmOutput->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_output = %s  >>>.\n", pNode->value.text.string);
				basic_info->alarm_output= atoi(pNode->value.text.string);
			}
		}

		alarmState = mxmlFindElement(parameters, xml, "alarmState", NULL, NULL, MXML_DESCEND);
		if(alarmState != NULL && alarmState->type == MXML_ELEMENT)
		{
			pNode = alarmState->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_state = %s  >>>.\n", pNode->value.text.string);
				basic_info->alarm_state= atoi(pNode->value.text.string);
			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_VideoLose_info_pack(char *buf, int result,  LPNET_CMU_VIDEO_LOSE basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_VIDEOLOSE);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);	
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;	
}

/*-------------------------SetPuHideDetection-----------------------------------*/
int parse_set_HideDetect_info_pack(char *buf,   int result,   LPNET_CMU_HIDE_DETECT basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters,*hideDetection ,*pNode, *videoId, *alarmEnabled,*alarmTime, *hideX,*hideY,*hideWidth;
	mxml_node_t	*hideHeight, *hideDetectSensitiv,*DeleteAlarmOut ,*alarmOutput, *alarmState, *hideDetectionVedio,*AlarmRecord;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		hideDetection = mxmlFindElement(parameters,xml,"hideDetection",NULL, NULL, MXML_DESCEND);
		videoId = mxmlFindElement(hideDetection, parameters, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}
		}

		alarmEnabled = mxmlFindElement(hideDetection, parameters, "hideDetectionAlarmEnabled", NULL, NULL, MXML_DESCEND);
		if(alarmEnabled != NULL && alarmEnabled->type == MXML_ELEMENT)
		{
			pNode = alarmEnabled->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_enabled = %s  >>>.\n", pNode->value.text.string);
				//basic_info->time_enable= atoi(pNode->value.text.string);
				strncpy(basic_info->alarm_enabled,pNode->value.text.string,5);
			}
		}

		alarmTime = mxmlFindElement(hideDetection, parameters, "hideDetectionAlarmTime", NULL, NULL, MXML_DESCEND);
		if(alarmTime != NULL && alarmTime->type == MXML_ELEMENT)
		{
			pNode = alarmTime->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->alarm_time,pNode->value.text.string,10);
			}
		}

		hideX = mxmlFindElement(hideDetection, parameters, "hideDetectionX", NULL, NULL, MXML_DESCEND);
		if(hideX != NULL && hideX->type == MXML_ELEMENT)
		{
			pNode = hideX->child;

			if(pNode  != NULL){
				//printf("<<<  hideX = %s  >>>.\n", pNode->value.text.string);
				basic_info->hide_coord.x= atoi(pNode->value.text.string);
			}
		}

		hideY = mxmlFindElement(hideDetection, parameters, "hideDetectionY", NULL, NULL, MXML_DESCEND);
		if(hideY != NULL && hideY->type == MXML_ELEMENT)
		{
			pNode = hideY->child;

			if(pNode  != NULL){
				//printf("<<<  hideY = %s  >>>.\n", pNode->value.text.string);
				basic_info->hide_coord.y= atoi(pNode->value.text.string);
			}
		}

		hideWidth = mxmlFindElement(hideDetection, parameters, "hideDetectionWidth", NULL, NULL, MXML_DESCEND);
		if(hideWidth != NULL && hideWidth->type == MXML_ELEMENT)
		{
			pNode = hideWidth->child;

			if(pNode  != NULL){
				//printf("<<<  hideWidth = %s  >>>.\n", pNode->value.text.string);
				basic_info->hide_coord.width= atoi(pNode->value.text.string);
			}
		}

		hideHeight = mxmlFindElement(hideDetection, parameters, "hideDetectionHeight", NULL, NULL, MXML_DESCEND);
		if(hideHeight != NULL && hideHeight->type == MXML_ELEMENT)
		{
			pNode = hideHeight->child;

			if(pNode  != NULL){
				//printf("<<<  hideHeight = %s  >>>.\n", pNode->value.text.string);
				basic_info->hide_coord.height = atoi(pNode->value.text.string);

			}
		}

		hideDetectSensitiv= mxmlFindElement(hideDetection, parameters, "hideDetectionSensitivity", NULL, NULL, MXML_DESCEND);
		if(hideDetectSensitiv != NULL && hideDetectSensitiv->type == MXML_ELEMENT)
		{
			pNode = hideDetectSensitiv->child;

			if(pNode  != NULL){
				//printf("<<<  sensitivity = %s  >>>.\n", pNode->value.text.string);
				basic_info->sensitivity = atoi(pNode->value.text.string);
			}
		}

		DeleteAlarmOut= mxmlFindElement(hideDetection, parameters, "hideDetectionAlarmOutput", NULL, NULL, MXML_DESCEND);
		if(DeleteAlarmOut != NULL && DeleteAlarmOut->type == MXML_ELEMENT)
		{
			alarmOutput= mxmlFindElement(DeleteAlarmOut, hideDetection, "alarmOutput", NULL, NULL, MXML_DESCEND);
			if(alarmOutput != NULL && alarmOutput->type == MXML_ELEMENT)
			{
				pNode = alarmOutput->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->hide_alarm_output.alarm_channel = atoi(pNode->value.text.string);

				}
			}
			alarmState= mxmlFindElement(DeleteAlarmOut, hideDetection, "alarmState", NULL, NULL, MXML_DESCEND);
			if(alarmState != NULL && alarmState->type == MXML_ELEMENT)
			{
				pNode = alarmState->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
					basic_info->hide_alarm_output.alarm_time = atoi(pNode->value.text.string);
				}
			}
		}

		hideDetectionVedio= mxmlFindElement(hideDetection, parameters, "hideDetectionVedio", NULL, NULL, MXML_DESCEND);
		if(hideDetectionVedio != NULL && hideDetectionVedio->type == MXML_ELEMENT)
		{
			hideDetection= mxmlFindElement(hideDetectionVedio, hideDetection, "hideDetection", NULL, NULL, MXML_DESCEND);

			if(hideDetection != NULL && hideDetection->type == MXML_ELEMENT)
			{
				pNode = hideDetection->child;

				if(pNode  != NULL){
					///printf("<<<  alarm_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->hide_alarm_video.alarm_record_channel= atoi(pNode->value.text.string);
				}
			}
			AlarmRecord= mxmlFindElement(hideDetectionVedio, hideDetection, "AlarmRecord", NULL, NULL, MXML_DESCEND);

			if(AlarmRecord != NULL && AlarmRecord->type == MXML_ELEMENT)
			{
				pNode = AlarmRecord->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_shot_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->hide_alarm_video.alarm_shot_channel= atoi(pNode->value.text.string);
				}
			}
		}
	}

	mxmlDelete(xml);
	return ret;
}

int build_set_HideDetect_info_pack(char *buf, int result,  LPNET_CMU_HIDE_DETECT basic_info)
{	
	mxml_node_t	*xml, *response, *result_code, *parameters;
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_HIDEDETECTION);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}

/*-------------------------SetPuMotionDetection-----------------------------------*/
int parse_set_MotionDetect_info_pack(char *buf,   int result,   LPNET_CMU_MOTION_DETECT basic_info)
{

	int ret = -1;


	char *p = NULL;
	char *q = NULL;
	char ImageEncodePara[512] = {0};
	char exchangBuff[32] = {0};
	int i=0;
	int j=0;
	int z = 0;
	int cnt = 0;
	int num = 0;

	mxml_node_t	*xml, *parameters, *pNode, *node,*videoId, *alarmEnabled,*motionAlarmTime, *motionDetectionFrequency,*motionDetectionSensitivity;
	mxml_node_t	*motionDetectionAlarmOutput, *alarmOutput, *alarmState, *motionDetectionVedio,*motionDetectionAlarmRecord, *motionDetectionAlarmShoot;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}
		}

		alarmEnabled = mxmlFindElement(parameters, xml, "motionDetectionAlarmEnabled", NULL, NULL, MXML_DESCEND);
		if(alarmEnabled != NULL && alarmEnabled->type == MXML_ELEMENT)
		{
			pNode = alarmEnabled->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_enabled = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->alarm_enabled,pNode->value.text.string,5);
			}
		}

		motionAlarmTime = mxmlFindElement(parameters, xml, "motionDetectionAlarmTime", NULL, NULL, MXML_DESCEND);
		if(motionAlarmTime != NULL && motionAlarmTime->type == MXML_ELEMENT)
		{
			pNode = motionAlarmTime->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->alarm_time,pNode->value.text.string,5);
			}
		}

		for (node = mxmlFindElement(parameters, parameters, "motionDetectionBlocks", NULL, NULL, MXML_DESCEND); node != NULL;node = mxmlFindElement(node, parameters, "motionDetectionBlocks", NULL, NULL, MXML_DESCEND) )
		{
			if(node->type == MXML_ELEMENT)	
			  cnt++;
		}


		basic_info->block_list_cnt = cnt;
		basic_info->block_lists = (LPNET_CMU_COORDINATE)os_sys_malloc(cnt * sizeof(NET_CMU_COORDINATE), __FILE__, __LINE__ );

		for (node = mxmlFindElement(parameters, parameters, "motionDetectionBlocks", NULL, NULL, MXML_DESCEND); 
					node != NULL;node = mxmlFindElement(node, parameters, "motionDetectionBlocks", NULL, NULL, MXML_DESCEND),num++ )
		{	
			if(node != NULL && node->type == MXML_ELEMENT)
			{
				pNode = node->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
					memset(ImageEncodePara,0,512);
					memset(exchangBuff,0,32);
					strcpy(ImageEncodePara,pNode->value.text.string);


					p = ImageEncodePara;
					q = ImageEncodePara;
					j = strlen(ImageEncodePara);
					z= 0;

					for(i = 0; i < j;i++,p++)
					{
						if(*p == ',')
						{
							ImageEncodePara[i] = '\0';
							sprintf(exchangBuff,"%s",q);

							switch(z)
							{
								case 0: basic_info->block_lists[num].x= atoi(exchangBuff) ;break;
								case 1: basic_info->block_lists[num].y= atoi(exchangBuff) ;break;
								case 2: basic_info->block_lists[num].height= atoi(exchangBuff) ;break;
								default: break;
							}
							q = p;
							q++;
							z++;

							if(z == 3)
							{
								sprintf(exchangBuff,"%s",q);
								printf("--------=====------%s--------=========--------" ,exchangBuff );
								basic_info->block_lists[num].width= atoi(exchangBuff);

							}
						}
					}
				}

			}
		}

		motionDetectionFrequency = mxmlFindElement(parameters, xml, "motionDetectionFrequency", NULL, NULL, MXML_DESCEND);
		if(motionDetectionFrequency != NULL && motionDetectionFrequency->type == MXML_ELEMENT)
		{
			pNode = motionDetectionFrequency->child;

			if(pNode  != NULL){
				//printf("<<<  frequency = %s  >>>.\n", pNode->value.text.string);
				basic_info->frequency= atoi(pNode->value.text.string);
			}
		}

		motionDetectionSensitivity = mxmlFindElement(parameters, xml, "motionDetectionSensitivity", NULL, NULL, MXML_DESCEND);
		if(motionDetectionSensitivity != NULL && motionDetectionSensitivity->type == MXML_ELEMENT)
		{
			pNode = motionDetectionSensitivity->child;

			if(pNode  != NULL){
				//printf("<<<  sensitivity = %s  >>>.\n", pNode->value.text.string);
				basic_info->sensitivity= atoi(pNode->value.text.string);
			}
		}

		motionDetectionAlarmOutput = mxmlFindElement(parameters, xml, "motionDetectionAlarmOutput", NULL, NULL, MXML_DESCEND);
		if(motionDetectionAlarmOutput != NULL && motionDetectionAlarmOutput->type == MXML_ELEMENT)
		{
			alarmOutput= mxmlFindElement(motionDetectionAlarmOutput, parameters, "alarmOutput", NULL, NULL, MXML_DESCEND);

			if(alarmOutput != NULL && alarmOutput->type == MXML_ELEMENT)
			{
				pNode = alarmOutput->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->motion_alarm_output.alarm_channel= atoi(pNode->value.text.string);
				}
			}

			alarmState= mxmlFindElement(motionDetectionAlarmOutput, parameters, "alarmState", NULL, NULL, MXML_DESCEND);
			if(alarmState != NULL && alarmState->type == MXML_ELEMENT)
			{
				pNode = alarmState->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
					basic_info->motion_alarm_output.alarm_time= atoi(pNode->value.text.string);
				}
			}
		}

		motionDetectionVedio = mxmlFindElement(parameters, xml, "motionDetectionVedio", NULL, NULL, MXML_DESCEND);
		if(motionDetectionVedio != NULL && motionDetectionVedio->type == MXML_ELEMENT)
		{
			motionDetectionAlarmRecord= mxmlFindElement(motionDetectionVedio, parameters, "motionDetectionAlarmRecord", NULL, NULL, MXML_DESCEND);

			if(motionDetectionAlarmRecord != NULL && motionDetectionAlarmRecord->type == MXML_ELEMENT)
			{
				pNode = motionDetectionAlarmRecord->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_record_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->motion_alarm_video.alarm_record_channel= atoi(pNode->value.text.string);
				}
			}	
			motionDetectionAlarmShoot= mxmlFindElement(motionDetectionVedio, parameters, "motionDetectionAlarmShoot", NULL, NULL, MXML_DESCEND);

			if(motionDetectionAlarmShoot != NULL && motionDetectionAlarmShoot->type == MXML_ELEMENT)
			{
				pNode = motionDetectionAlarmShoot->child;

				if(pNode  != NULL){
					//printf("<<<  alarm_shot_channel = %s  >>>.\n", pNode->value.text.string);
					basic_info->motion_alarm_video.alarm_shot_channel = atoi(pNode->value.text.string);
				}

			}
		}
	}

	mxmlDelete(xml);

	return ret;
}


int build_set_MotionDetect_info_pack(char *buf, int result,  LPNET_CMU_MOTION_DETECT basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_MOTIONDETECTION);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;	
}

/*-------------------------SetPuImageHideArea-----------------------------------*/
int parse_set_ImagHideArea_info_pack(char *buf,   int result,   LPNET_CMU_IMG_HIDE_AREA basic_info)
{
	int ret = -1;
	char *p;	
	char *q;
	char ImageEncodePara[512] = {0};
	char exchangBuff[32] = {0};
	int i=0;
	int j=0;
	int z = 0;
	int cnt = 0;
	int num = 0;

	mxml_node_t	*xml, *parameters, *pNode,*node, *videoId, *enabled;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);

			}
		}

		enabled = mxmlFindElement(parameters, xml, "hideAreaEnabled", NULL, NULL, MXML_DESCEND);
		if(enabled != NULL && enabled->type == MXML_ELEMENT)
		{
			pNode = enabled->child;

			if(pNode  != NULL){
				//printf("<<<  enabled = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->enabled,pNode->value.text.string,5);
			}

		}
		printf("endbel %s\n",basic_info->enabled);

		if(strcmp(basic_info->enabled,"false")){



			for (node = mxmlFindElement(parameters, parameters, "hideImageArea", NULL, NULL, MXML_DESCEND); node != NULL;node = mxmlFindElement(node, parameters, "hideImageArea", NULL, NULL, MXML_DESCEND) )
			{
				if(node->type == MXML_ELEMENT)	
				  cnt++;
			}




			basic_info->area_list_cnt= cnt;
			basic_info->area_lists= (LPNET_CMU_COORDINATE)os_sys_malloc(cnt * sizeof(NET_CMU_COORDINATE), __FILE__, __LINE__ );


			for (node = mxmlFindElement(parameters, parameters, "hideImageArea", NULL, NULL, MXML_DESCEND); 
						node != NULL;node = mxmlFindElement(node, parameters, "hideImageArea", NULL, NULL, MXML_DESCEND),num++ )
			{	
				if(node != NULL && node->type == MXML_ELEMENT)
				{
					pNode = node->child;

					memset(ImageEncodePara,0,512);
					memset(exchangBuff,0,32);
					strcpy(ImageEncodePara,pNode->value.text.string);


					p = ImageEncodePara;
					q = ImageEncodePara;
					j = strlen(ImageEncodePara);
					z= 0;

					for(i = 0; i < j;i++,p++)
					{
						if(*p == ',')
						{
							ImageEncodePara[i] = '\0';
							sprintf(exchangBuff,"%s",q);

							switch(z)
							{
								case 0: basic_info->area_lists[num].x= atoi(exchangBuff) ;   break;
								case 1: basic_info->area_lists[num].y= atoi(exchangBuff) ;  break;
								case 2: basic_info->area_lists[num].height= atoi(exchangBuff) ; break;
							}

							q = p;
							q++;
							z++;

							if(z == 3)
							{
								sprintf(exchangBuff,"%s",q);
								basic_info->area_lists[num].width= atoi(exchangBuff);
							}
						}
					}

				}


			}

		}

	}


	mxmlDelete(xml);
	return ret;
}


int build_set_ImagHideArea_info_pack(char *buf, int result,  LPNET_CMU_IMG_HIDE_AREA basic_info)
{

	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_IMAGEHIDEAREA);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;

}

/*-------------------------SetPuLocalStorageTask-----------------------------------*/
int parse_set_StoreageTask_info_pack(char *buf,   int result,   LPNET_CMU_STORAGE_TASK basic_info)
{

	int ret = -1;


	int cnt=0;
	int num = 0;
	int  iWeek = 0;
	int  iType = 0;
	int  iStartHour = 0;
	int  iStartMin = 0;
	int  iStopHour = 0;
	int  iStopMin = 0;

	mxml_node_t	*xml, *parameters,*pNode, *node ,*videoId,*weekday, *recordType,*startHour,*startMin;
	mxml_node_t	*stopHour, *stopMin,*recycleRecEnable ,*recordPolicy, *frameInterval, *recordEnable;
	xml = mxmlLoadString(NULL, buf, NULL);

	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		videoId = mxmlFindElement(parameters, xml, "videoId", NULL, NULL, MXML_DESCEND);
		if(videoId != NULL && videoId->type == MXML_ELEMENT)
		{
			pNode = videoId->child;

			if(pNode  != NULL){
				//printf("<<<  video_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->video_id= atoi(pNode->value.text.string);
			}

		}

		for (node = mxmlFindElement(parameters, parameters, "schedule", NULL, NULL, MXML_DESCEND); node != NULL;node = mxmlFindElement(node, parameters, "schedule", NULL, NULL, MXML_DESCEND) )
		{
			if(node->type == MXML_ELEMENT)	
			  cnt++;
		}


		basic_info->schedule_list_cnt = cnt;
		basic_info->schedule_lists = (LPNET_CMU_STORAGE_SCHEDULE)os_sys_malloc(cnt * sizeof(NET_CMU_STORAGE_SCHEDULE), __FILE__, __LINE__ );





		for (node = mxmlFindElement(parameters, parameters, "schedule", NULL, NULL, MXML_DESCEND); 
					node != NULL;node = mxmlFindElement(node, parameters, "schedule", NULL, NULL, MXML_DESCEND),num++)

		{



			if(node != NULL && node->type == MXML_ELEMENT)
			{
				weekday = mxmlFindElement(node, parameters, "weekday", NULL, NULL, MXML_DESCEND);

				if(weekday != NULL && weekday->type == MXML_ELEMENT)
				{
					pNode = weekday->child;

					if(pNode  != NULL){
						//printf("\n<<<  weekday = %s  >>>.\n", pNode->value.text.string);
						iWeek= atoi(pNode->value.text.string);
					}
				}


				recordType = mxmlFindElement(node, parameters, "recordType", NULL, NULL, MXML_DESCEND);

				if(recordType != NULL && recordType->type == MXML_ELEMENT)
				{
					pNode = recordType->child;

					if(pNode  != NULL){
						//printf("<<<  recordType = %s  >>>.\n", pNode->value.text.string);
						iType= atoi(pNode->value.text.string);
					}
				}



				startHour = mxmlFindElement(node, parameters, "startHour", NULL, NULL, MXML_DESCEND);

				if(startHour != NULL && startHour->type == MXML_ELEMENT)
				{
					pNode = startHour->child;

					if(pNode  != NULL){
						//printf("<<<  startHour = %s  >>>.\n", pNode->value.text.string);
						iStartHour= atoi(pNode->value.text.string);
					}
				}



				startMin = mxmlFindElement(node, parameters, "startMin", NULL, NULL, MXML_DESCEND);					
				if(startMin != NULL && startMin->type == MXML_ELEMENT)
				{
					pNode = startMin->child;

					if(pNode  != NULL){
						//printf("<<<  startMin = %s  >>>.\n", pNode->value.text.string);
						iStartMin= atoi(pNode->value.text.string);
					}
				}



				stopHour = mxmlFindElement(node, parameters, "stopHour", NULL, NULL, MXML_DESCEND);
				if(stopHour != NULL && stopHour->type == MXML_ELEMENT)
				{
					pNode = stopHour->child;

					if(pNode  != NULL){
						//printf("<<<  stopHour = %s  >>>.\n", pNode->value.text.string);
						iStopHour= atoi(pNode->value.text.string);
					}
				}


				stopMin = mxmlFindElement(node, parameters, "stopMin", NULL, NULL, MXML_DESCEND);
				if(stopMin != NULL && stopMin->type == MXML_ELEMENT)
				{
					pNode = stopMin->child;

					if(pNode  != NULL){
						//printf("<<<  iStopMin = %s  >>>.\n", pNode->value.text.string);
						iStopMin= atoi(pNode->value.text.string);
					}
				}

			}




			if(((iWeek > 7) || (iWeek < 1))
						|| ((iStartHour < 0) || (iStartHour > 24))
						|| ((iStartMin < 0) || (iStartMin > 59))
						|| ((iStopHour < 0) || (iStopHour > 24))
						|| ((iStopMin < 0) || (iStopMin > 59)))
			{          
				continue;
			}

			basic_info->schedule_lists[num].week_day = iWeek;
			basic_info->schedule_lists[num].record_type = iType;
			basic_info->schedule_lists[num].start_hour = iStartHour;
			basic_info->schedule_lists[num].start_min= iStartMin;
			basic_info->schedule_lists[num].stop_hour= iStopHour;
			basic_info->schedule_lists[num].stop_min = iStopMin;



			//printf("/n================frist %d loop ===================================/n",num);



			printf("\niWeek value is %d\n",iWeek);
			printf("iType value is %d\n",iType);
			printf("iStartHour value is %d\n",iStartHour);
			printf("iStartMin value is %d\n",iStartMin);
			printf("iStopHour value is %d\n",iStopHour);
			printf("iStopMin value is %d\n",iStopMin);


			//printf("/n=========================================================================/n");


		}	



		recycleRecEnable = mxmlFindElement(parameters, xml,  "recycleRecEnable", NULL, NULL, MXML_DESCEND);
		if(recycleRecEnable != NULL && recycleRecEnable->type == MXML_ELEMENT)
		{
			pNode = recycleRecEnable->child;

			if(pNode  != NULL){
				//printf("<<<  recycleRecEnable = %s  >>>.\n", pNode->value.text.string);
				basic_info->recycle_enable = atoi(pNode->value.text.string);
			}
		}

		recordPolicy = mxmlFindElement(parameters, xml, "recordPolicy", NULL, NULL, MXML_DESCEND);
		if(recordPolicy != NULL && recordPolicy->type == MXML_ELEMENT)
		{
			pNode = recordPolicy->child;

			if(pNode  != NULL){
				//printf("<<<  alarm_time = %s  >>>.\n", pNode->value.text.string);
				basic_info->record_policy = atoi(pNode->value.text.string);
			}
		}

		frameInterval = mxmlFindElement(parameters, xml, "frameInterval", NULL, NULL, MXML_DESCEND);
		if(frameInterval != NULL && frameInterval->type == MXML_ELEMENT)
		{
			pNode = frameInterval->child;

			if(pNode  != NULL){
				//printf("<<<  frameInterval = %s  >>>.\n", pNode->value.text.string);
				basic_info->frame_interval = atoi(pNode->value.text.string);
			}
		}

		recordEnable = mxmlFindElement(parameters, xml, "recordEnable", NULL, NULL, MXML_DESCEND);
		if(recordEnable != NULL && recordEnable->type == MXML_ELEMENT)
		{
			pNode = recordEnable->child;

			if(pNode  != NULL){
				//printf("<<<  record_enable = %s  >>>.\n", pNode->value.text.string);
				basic_info->record_enable= atoi(pNode->value.text.string);
			}
		}



	}

	mxmlDelete(xml);



	return ret;
}


int build_set_StoreageTask_info_pack(char *buf, int result,  LPNET_CMU_STORAGE_TASK basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_LOCALSTORAGETASK);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;
}


//-------------------------SetPresetPTZ----------------------------------
int parse_set_presetList_info_pack(char *buf,   int result,   LPNET_CMU_PRESET_LIST basic_info)
{
	int ret = -1;
	//char outBuf[MAX_BUFFER_LEN] = {0};

	//char *msgBuf = buf;

	//	iconv_t cd;
	//	int rc;
	//	int outDataLen = MAX_BUFFER_LEN;
	//	int inDataLen = MAX_BUFFER_LEN;
	//char blankStr[50] = {0};
	//char parseName[50] = "presetName";
	//char *in_gb2312 = buf;
	//char *encodeBuf[MAX_BUFFER_LEN] = {0};
	//char *encodeBuf2[MAX_BUFFER_LEN] = {0};

	mxml_node_t	*xml, *parameters ,*pNode, *ptzId, *presetIndex,*presetName;

	/*
	   char **pin = &buf;
	   char **pout = &msgBuf;

	   cd = iconv_open("gb2312","utf-8");
	   if(cd == 0)
	   printf("gb->utf erro 1");


	   if(iconv(cd,&buf,&outDataLen,&msgBuf,&inDataLen)== -1)	
	   printf("gb->utf erro 1");

	   iconv_close(cd);


*/


	//	parse_string(buf, parseName,blankStr);

	//printf("blankStr is %s\n",blankStr);

	//u2g(in_gb2312,strlen(in_gb2312),encodeBuf,MAX_BUFFER_LEN);	

	//g2u(encodeBuf,strlen(encodeBuf),encodeBuf2,MAX_BUFFER_LEN);


	char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
	sscanf(buf,"%[^>]",p) ;
	strcat(firstbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , firstbuf , 1024) ;
	char *tmp = NULL;

	tmp = Contrary_XmlLntercept(buf) ;
	printf("parse xml is %s \n", tmp);



	xml = mxmlLoadString(NULL, tmp, NULL);


	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		ptzId = mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;

			if(pNode  != NULL){
				//printf("<<<  ptz_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->ptz_id= atoi(pNode->value.text.string);
			}
		}

		basic_info->preset_lists = (LPNET_CMU_PRESET)os_sys_malloc(sizeof(NET_CMU_PRESET), __FILE__, __LINE__ );

		presetIndex = mxmlFindElement(parameters, xml, "presetIndex", NULL, NULL, MXML_DESCEND);
		if(presetIndex != NULL && presetIndex->type == MXML_ELEMENT)
		{
			pNode = presetIndex->child;

			if(pNode  != NULL){
				//printf("<<<  presetIndex = %s  >>>.\n", pNode->value.text.string);
				basic_info->preset_lists->preset_index	= atoi(pNode->value.text.string);	
			}
		}

		presetName = mxmlFindElement(parameters, xml, "presetName", NULL, NULL, MXML_DESCEND);
		if(presetName != NULL && presetName->type == MXML_ELEMENT)
		{
			pNode = presetName->child;

			if(pNode  != NULL){
				strncpy(basic_info->preset_lists->preset_name,Replacement_parts_preset,16);
				printf("<<<  preset_name = %s  >>>.\n",basic_info->preset_lists->preset_name );
				memset(Replacement_parts_preset , 0 , 128) ;
			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}


int build_set_presetList_info_pack(char *buf, int result,  LPNET_CMU_PRESET_LIST basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	//char *tmp = NULL ;	
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_PRESETPTZ);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	sscanf(buf,"%[^>]",p) ;
	strcat(secondbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , secondbuf , 1024) ;
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;	
}

//------------------------SetDefaulePresetPTZ
int parse_set_defaultResetprz_info_pack(char *buf,   int result,   LPNET_CMU_DEFAULT_PRESET basic_info)
{
	int ret = -1;

	mxml_node_t	*xml, *parameters ,*pNode,*ptzId, *presetIndex, *presetName,*restoreTime;

	char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	//char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	char p[1024];
	//char FinalDest[1024] ;
	sscanf(buf,"%[^>]",p) ;
	strcat(firstbuf, &buf[strlen(p)+1]) ;
	memcpy(buf , firstbuf , 1024) ;
	char *tmp = NULL;

	tmp = Contrary_XmlLntercept(buf) ;
	printf("parse xml is %s \n", tmp);



	xml = mxmlLoadString(NULL, tmp, NULL);


	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		ptzId = mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;

			if(pNode  != NULL){
				//printf("<<<  ptz_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->ptz_id= atoi(pNode->value.text.string);
			}
		}

		presetIndex = mxmlFindElement(parameters, xml, "presetIndex", NULL, NULL, MXML_DESCEND);
		if(presetIndex != NULL && presetIndex->type == MXML_ELEMENT)
		{
			pNode = presetIndex->child;

			if(pNode  != NULL){
				//printf("<<<  presetIndex = %s  >>>.\n", pNode->value.text.string);
				//basic_info->time_enable= atoi(pNode->value.text.string);
				basic_info->preset_index= atoi(pNode->value.text.string);	
			}
		}

		presetName = mxmlFindElement(parameters, xml, "presetName", NULL, NULL, MXML_DESCEND);
		if(presetName != NULL && presetName->type == MXML_ELEMENT)
		{
			pNode = presetName->child;

			if(pNode  != NULL){
				strncpy(basic_info->preset_name,Replacement_parts_preset,16);
				printf("<<<  preset_name = %s  >>>.\n",basic_info->preset_name);
				memset(Replacement_parts_preset , 0 , 128) ;
			}
		}

		restoreTime = mxmlFindElement(parameters, xml, "restoreTime", NULL, NULL, MXML_DESCEND);
		if(restoreTime != NULL && restoreTime->type == MXML_ELEMENT)
		{
			pNode = restoreTime->child;

			if(pNode  != NULL){
				//printf("<<<  restore_time = %s  >>>.\n", pNode->value.text.string);
				basic_info->restore_time= atoi(pNode->value.text.string);
			}
		}	
	}

	mxmlDelete(xml);
	return ret;
}


int build_set_defaultResetprz_info_pack(char *buf, int result,  LPNET_CMU_DEFAULT_PRESET basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;
	//char firstbuf[1024] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" ;
	//char secondbuf[1024] ="<?xml version=\"1.0\" encoding=\"GB2312\" ?>" ;
	//char p[1024];
	//char FinalDest[1024] ;
	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_DEFAULTPRESETPTZ);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");

	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);

	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;	
}

//--------------SetCruiseTrack
int parse_set_cruisetrack_info_pack(char *buf,   int result,   LPNET_CMU_CRUISE_TRACK basic_info)
{
	int ret = -1;
	mxml_node_t	*xml, *parameters ,*pNode, *ptzId, *cruiseNumber,*cruisePoint;

	xml = mxmlLoadString(NULL, buf, NULL);
	parameters = mxmlFindElement(xml, xml, "parameters", NULL, NULL, MXML_DESCEND);
	if(parameters != NULL)
	{
		ret = 0;

		ptzId = mxmlFindElement(parameters, xml, "ptzId", NULL, NULL, MXML_DESCEND);
		if(ptzId != NULL && ptzId->type == MXML_ELEMENT)
		{
			pNode = ptzId->child;

			if(pNode  != NULL){
				//printf("<<<  ptz_id = %s  >>>.\n", pNode->value.text.string);
				basic_info->ptz_id= atoi(pNode->value.text.string);
			}
		}

		cruiseNumber = mxmlFindElement(parameters, xml, "cruiseNumber", NULL, NULL, MXML_DESCEND);
		if(cruiseNumber != NULL && cruiseNumber->type == MXML_ELEMENT)
		{
			pNode = cruiseNumber->child;

			if(pNode  != NULL){
				//printf("<<<  cruise_num = %s  >>>.\n", pNode->value.text.string);
				basic_info->cruise_num = atoi(pNode->value.text.string);
			}

		}

		cruisePoint = mxmlFindElement(parameters, xml, "presetName", NULL, NULL, MXML_DESCEND);
		if(cruisePoint != NULL && cruisePoint->type == MXML_ELEMENT)
		{
			pNode = cruisePoint->child;

			if(pNode  != NULL){
				//printf("<<<  cruise_point = %s  >>>.\n", pNode->value.text.string);
				strncpy(basic_info->cruise_point, pNode->value.text.string,200);
			}
		}
	}

	mxmlDelete(xml);
	return ret;
}


int build_set_cruisetrack_info_pack(char *buf, int result,  LPNET_CMU_CRUISE_TRACK basic_info)
{
	mxml_node_t	*xml, *response, *result_code, *parameters;

	xml = mxmlNewXML("1.0");
	response = mxmlNewElement(xml, "response");
	mxmlElementSetAttrf(response, "command", SET_PU_CRUISETRACK);

	result_code = mxmlNewElement(response, "result");
	mxmlElementSetAttrf(result_code, "code", "%d", result);	

	parameters = mxmlNewElement(response, "parameters");


	mxmlSaveString(xml, buf, MAX_BUFFER_LEN, NULL);
	mxmlDelete(xml);

	//LOG("xml is ===[%d]- [\n%s\n].\n", strlen(buf), buf);

	return 0;

}

int handle_set_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len)
{
	int ret = -1, result = 0;
	unsigned long size = 0;

	switch(cmd_id)
	{
#if 1
		case CMD_SET_BASICINFO:		// 设置设备基本参数
			{
				NET_CMU_BASIC_INFO basic;
				memset(&basic, 0, sizeof(NET_CMU_BASIC_INFO));

				if(parse_set_basic_info_pack(buf, result,  &basic) == 0)
				{	
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_BASICINFO, (char*)&basic, &size, g_param);
                    else
                      LOG("msg_proc is NULL\n");

					ret = build_set_basic_info_pack(buf, result,  &basic);				
				}
				break;
			}
#endif
		case CMD_SET_IPINFO:		// 设置设备基本参数
			{
				NET_CMU_IP_INFO basic;
				memset(&basic, 0, sizeof(NET_CMU_IP_INFO));

				if(parse_set_ip_info_pack(buf, result, &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_IPINFO, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_ip_info_pack(buf, result, &basic);			
				}
				break;
			}	
#if 1
		case CMD_SET_USER:		// 设置网络基本参数
			{
				NET_CMU_USER basic;
				memset(&basic, 0, sizeof(NET_CMU_USER));

				if(parse_set_puUser_info_pack(buf, result,  &basic) == 0)
				{	
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_USER, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_puUser_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_SERIALPORT://设置串口基本参数
			{

				NET_CMU_SERIAL_PORT basic;
				memset(&basic, 0, sizeof(NET_CMU_USER));

				if(parse_set_puSerialPort_info_pack(buf, result,  &basic) == 0)
				{	
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_SERIALPORT, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_puSerialPort_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_INOUTDEV:	//开关量输入/输出参数
			{
				NET_CMU_INOUT_INFO basic;
				memset(&basic, 0, sizeof(NET_CMU_INOUT_INFO));



				if(parse_set_inout_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_OUTDEV, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_inout_info_pack(buf, result,  &basic);			
				}



				memset(buf,0,MAX_BUFFER_LEN);
				result = 2;
				ret = build_set_inout_info_pack(buf, result,  &basic);	
				break;
			}
#endif
#if 1
		case CMD_SET_TIME:		//时间参数
			{
				NET_CMU_DEV_TIME basic;
				memset(&basic, 0, sizeof(NET_CMU_DEV_TIME));

				if(parse_set_time_info_pack(buf, result,  &basic) == 0)
				{
                    #if 1
					if(g_msg_proc)
					{
                        //printf("g_msg_proc(SET_PU_TIME, (char*)&basic, &size, g_param);\n");
                        result = g_msg_proc(SET_PU_TIME, (char*)&basic, &size, g_param);
                    }
					  
                     else
                      LOG("msg_proc is NULL\n");
                    #endif
					ret = public_set_time_info_pack(buf, result,  &basic);				
				}
				break;
			}
#endif
#if 1
		case CMD_SET_VIDEOFORMAT:		//图像制式参数
			{
				NET_CMU_VIDEO_FORMAT basic;
				memset(&basic, 0, sizeof(NET_CMU_VIDEO_FORMAT));

				if(parse_set_Videoformat_info_pack(buf, result,  &basic) == 0)
				{	
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_VIDEOFORMAT, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_Videoformat_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_IMAGEENCODEPARA:	//图像参数
			{
				NET_CMU_IMG_ENCODE basic;
				char imageSize[32] = "\0";
				memset(&basic, 0, sizeof(NET_CMU_IMG_ENCODE));

				if(parse_set_PuImageEncode_info_pack(buf, result,  &basic) == 0)
				{	

					strncpy(imageSize,basic.image_size,32);

#if 0
					if((strcmp(imageSize,"720I")) || (strcmp(imageSize,"1080I"))  || !(strcmp(imageSize,"1080P")) || !(strcmp(imageSize,"DCIF")))
					{

						printf("imageSize value is %s\n",imageSize);
						result = 17;

						ret = build_set_PuImageEncode_info_pack(buf, result,  &basic);	

						break;

					}

#endif			 
					if(g_msg_proc){
						result = g_msg_proc(SET_PU_IMAGEENCODEPARA, (char*)&basic, &size, g_param);
                    
						printf("\n============device  return result value is %d======================\n",result);
					}
                    else
                      LOG("msg_proc is NULL\n");

					//result = 17;
					memset(buf,0,MAX_BUFFER_LEN);

					ret = build_set_PuImageEncode_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_IMAGEDISPLAYPARA:	// 对比度
			{
				NET_CMU_IMG_DISPLAY basic;
				memset(&basic, 0, sizeof(NET_CMU_IMG_DISPLAY));

				if(parse_set_PuImageDisplay_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_IMAGEDISPLAYPARA, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_PuImageDisplay_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_IMAGETEXTPARA:	//时间文本	
			{
				NET_CMU_IMG_TEXT basic;
				memset(&basic, 0, sizeof(NET_CMU_IMG_TEXT));

				if(parse_set_ImageTextPara_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_IMAGETEXTPARA, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_ImageTextPara_info_pack(buf, result,  &basic);	
					if(ret == 0)
					{
						printf("the set IMAGETESTOARA IS STATING .............................................%s\n" , buf) ;
					}
				}
				break;
			}
		case CMD_SET_ALARMPARAMS:		
			{
				NET_CMU_BASIC_ALARM basic;
				memset(&basic, 0, sizeof(NET_CMU_BASIC_ALARM));
				/*

				   if(parse_set_PuAlarmParams_info_pack(buf, result,  &basic) == 0)
				   {
				   if(g_msg_proc)
				   result = g_msg_proc(SET_PU_GIPINALARM, (char*)&basic, &size, g_param);

				   ret = build_set_PuAlarmParams_info_pack(buf, result,  &basic);				
				   }

*/

				memset(buf,0,MAX_BUFFER_LEN);
				result = 17;
				ret = build_set_PuAlarmParams_info_pack(buf, result,  &basic);


				break;
			}
		case CMD_SET_GIPINALARM:		
			{
				NET_CMU_GPIN_ALARM_INFO basic;
				memset(&basic, 0, sizeof(NET_CMU_GPIN_ALARM_INFO));


				/*
				   if(parse_set_GpinAlarm_info_pack(buf, result,  &basic) == 0)
				   {
				   if(g_msg_proc)
				   result = g_msg_proc(SET_PU_GIPINALARM, (char*)&basic, &size, g_param);

				   ret = build_set_GpinAlarm_info_pack(buf, result,  &basic);				
				   }

*/


				memset(buf,0,MAX_BUFFER_LEN);
				result = 17;
				ret = build_set_GpinAlarm_info_pack(buf, result,  &basic);
				break;
			}
		case CMD_SET_VIDEOLOSE:		
			{
				NET_CMU_VIDEO_LOSE basic;
				memset(&basic, 0, sizeof(NET_CMU_VIDEO_LOSE));



				/*

				   if(parse_set_VideoLose_info_pack(buf, result,  &basic) == 0)
				   {	
				   if(g_msg_proc)
				   result = g_msg_proc(SET_PU_VIDEOLOSE, (char*)&basic, &size, g_param);

				   ret = build_set_VideoLose_info_pack(buf, result,  &basic);				
				   }

*/

				memset(buf,0,MAX_BUFFER_LEN);
				result = 17;
				ret = build_set_VideoLose_info_pack(buf, result,  &basic);
				break;
			}
		case CMD_SET_HIDEDETECTION:	//设备视频遮蔽告警参数
			{
				NET_CMU_HIDE_DETECT basic;
				memset(&basic, 0, sizeof(NET_CMU_HIDE_DETECT));




				if(parse_set_HideDetect_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_HIDEDETECTION, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_HideDetect_info_pack(buf, result,  &basic);				
				}



				memset(buf,0,MAX_BUFFER_LEN);
				result = 17;
				ret = build_set_HideDetect_info_pack(buf, result,  &basic);
				break;
			}
		case CMD_SET_MOTIONDETECTION:		
			{
				NET_CMU_MOTION_DETECT basic;
				memset(&basic, 0, sizeof(NET_CMU_MOTION_DETECT));


				if(parse_set_MotionDetect_info_pack(buf, result,  &basic) == 0)
				{		
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_MOTIONDETECTION, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					if(basic.block_lists){

						free((&basic)->block_lists);
						basic.block_lists = NULL;
					}
					memset(buf,0,MAX_BUFFER_LEN);
					ret = build_set_MotionDetect_info_pack(buf, result,  &basic);				
				}

				break;
			}
		case CMD_SET_IMAGEHIDEAREA:		//图像遮挡设置
			{
				NET_CMU_IMG_HIDE_AREA basic;
				memset(&basic, 0, sizeof(NET_CMU_IMG_HIDE_AREA));

				if(parse_set_ImagHideArea_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_IMAGEHIDEAREA, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");


					if(basic.area_lists){
						free(basic.area_lists);
						basic.area_lists = NULL;
					}
					memset(buf,0,MAX_BUFFER_LEN);
					ret = build_set_ImagHideArea_info_pack(buf, result,  &basic);	

				}


				break;
			}
		case CMD_SET_LOCALSTORAGETASK:		
			{


				/*
				   NET_CMU_STORAGE_TASK basic;
				   memset(&basic, 0, sizeof(NET_CMU_STORAGE_TASK));


				   if(parse_set_StoreageTask_info_pack(buf, result,  &basic) == 0)
				   {

				   if(g_msg_proc)
				   result = g_msg_proc(SET_PU_LOCALSTORAGETASK, (char*)&basic, &size, g_param);


				   if(basic.schedule_lists){

				//free((&basic)->schedule_lists);
				free(basic.schedule_lists);
				basic.schedule_lists = NULL;


				}

				memset(buf,0,MAX_BUFFER_LEN);
				ret = build_set_StoreageTask_info_pack(buf, result,  &basic);		


				}

*/
				break;
			}
		case CMD_SET_PRESETPTZ:		
			{
				NET_CMU_PRESET_LIST basic;
				memset(&basic, 0, sizeof(NET_CMU_PRESET_LIST));

				if(parse_set_presetList_info_pack(buf, result,  &basic) == 0)
				{
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_PRESETPTZ, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					if(basic.preset_lists){
						free(basic.preset_lists);
						basic.preset_lists = NULL;
					}
					ret = build_set_presetList_info_pack(buf, result,  &basic);				
				}
				break;
			}
		case CMD_SET_DEFAULTPRESETPTZ:		
			{

				NET_CMU_DEFAULT_PRESET basic;
				memset(&basic, 0, sizeof(NET_CMU_DEFAULT_PRESET));

				if(parse_set_defaultResetprz_info_pack(buf, result,  &basic) == 0)
				{			
					if(g_msg_proc)
					  result = g_msg_proc(SET_PU_DEFAULTPRESETPTZ, (char*)&basic, &size, g_param);
                     else
                      LOG("msg_proc is NULL\n");

					ret = build_set_defaultResetprz_info_pack(buf, result,  &basic);	
				}


				break;
			}
		case CMD_SET_CRUISETRACK:		
			{
				NET_CMU_CRUISE_TRACK basic;
				memset(&basic, 0, sizeof(NET_CMU_CRUISE_TRACK));
				/*
				   if(parse_set_cruisetrack_info_pack(buf, result,  &basic) == 0)
				   {

				   if(g_msg_proc)
				   result = g_msg_proc(SET_PU_CRUISETRACK, (char*)&basic, &size, g_param);

				   ret = build_set_cruisetrack_info_pack(buf, result,  &basic);				
				   }
				   */
				memset(buf,0,MAX_BUFFER_LEN);
				result = 2;
				ret = build_set_cruisetrack_info_pack(buf, result,  &basic);
				break;
			}
#endif
		default:
			break;
	}

	if(ret == 0)
	{
		sock_header resp;
		memset(&resp,0,sizeof(resp));

		resp.cseq   = cseq;
		resp.ver    = htons(0x0100);
		resp.length = htons((unsigned int)strlen(buf));

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
