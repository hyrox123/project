#ifndef _MS_MS_CMU_OPER_H_
#define _MS_MS_CMU_OPER_H_
#include "ms_pu_stream.h"

int handle_oper_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len);

int handle_ptz_oper_msg( char* buf, int len);

int handle_contrl_oper_msg( int cmd );


int handle_upload_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len);

int handle_upload_alarm(char* buf, unsigned long len);

int prase_oper_control_file_playpack(char *buf, int result, LPNET_MDU_START_MEDIA media_oper);

#endif/*_MS_CMU_OPER_H_*/


