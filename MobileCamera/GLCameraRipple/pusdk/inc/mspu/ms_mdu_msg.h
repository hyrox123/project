#ifndef __MS_MDU_MSG_H__
#define __MS_MDU_MSG_H__

#include "ms_pu_param.h"


int parse_reg_packet(const char* sip_packet);


int build_kpl_packet(char *sip_packet);

int parse_kpl_packet(const char* sip_packet);


int parse_invite_packet(const char* sip_packet, LPNET_MDU_START_MEDIA start_media);

int build_invite_resp_packet(char *sip_packet, LPNET_MDU_START_MEDIA start_media);


int build_file_invite_resp_packet(char* sip_packet, LPNET_MDU_START_MEDIA start_media);

int build_real_invite_resp_packet(char* sip_packet, LPNET_MDU_START_MEDIA start_media);


int parse_ack_packet(const char* sip_packet);

int parse_bye_packet(const char* sip_packet, LPNET_MDU_START_MEDIA stop_media);

int build_bye_resp_packet(char* sip_packet);

int build_media_info(int video_id, int media_type, int stream_type,char* sip_packet);

int build_fileback_info(char* file_name, char* sip_packet);

#endif/*__MS_MDU_MSG_H__*/
