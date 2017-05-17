#include "stdafx.h"
#include "ms_pu_glob.h"
#include <stdio.h>

//#include <string.h>
//线程池
void*       thrd_pool = NULL;
void*       gb_thrd_pool = NULL;

handle_grp* cmu_hgrp  = NULL;
unit_info*  cmu_info  = NULL;
sock_info*  cmu_skt   = NULL;
//sock_info*   logs_skt   = NULL;
alram_data* cmu_alarm = NULL;

unit_info*  mdu_info = NULL;
sock_info*  mdu_skt   = NULL;

handle_grp* gb_cmu_hgrp = NULL;
unit_info*  gb_cmu_info  = NULL;
sock_info*  gb_cmu_skt   = NULL;
//sock_info*   logs_skt   = NULL;
alram_data* gb_cmu_alarm = NULL;

//流对象池
void* stream_pool     = NULL;
void* gb_stream_pool     = NULL;
net_info_t		  gb_g_info;
net_info_t        ms_g_info;

//Ming Soft
net_register_proc g_reg_proc = NULL;
net_msg_proc	  g_msg_proc= NULL;
net_stream_proc   g_stream_proc= NULL;
uint32_t		  g_param= 0;

net_register_proc gb_g_reg_proc = NULL;
net_msg_proc	  gb_g_msg_proc= NULL;
net_stream_proc   gb_g_stream_proc= NULL;
uint32_t		  gb_g_param= 0;

int net_error_num;
unsigned int select_param;
int pu_initiative_cancellation;

void *  stop_mutex;
unsigned int ms_platform_status;
unsigned int gb_platform_status;
//sprintf(build_time,"%s %s", __DATE__,__TIME__);

