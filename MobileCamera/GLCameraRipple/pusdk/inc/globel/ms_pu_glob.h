#ifndef __MS_PU_GLOB_H__
#define __MS_PU_GLOB_H__

#include <ms_pu_lib.h>
#include <ms_pu_type.h>
#define VERSION "2.8.0"
//char cVerTime[20];
//#define BUILD_TIME(cVerTime)  ({sprintf(cVerTime, "%s %s", __DATE__, __TIME__;})
//char build_time[30];

/********************************************/
extern void* thrd_pool;
extern void* gb_thrd_pool;

extern handle_grp* cmu_hgrp;

extern unit_info*  cmu_info;
extern sock_info*  cmu_skt;

extern unit_info* mdu_info;
extern sock_info*  mdu_skt;

extern alram_data* cmu_alarm;
extern handle_grp* gb_cmu_hgrp;

extern unit_info*  gb_cmu_info;
extern sock_info*  gb_cmu_skt;
extern alram_data* gb_cmu_alarm;
extern void* stream_pool;
extern void* gb_stream_pool;
extern net_info_t		  gb_g_info;
extern net_info_t		  ms_g_info;

extern net_register_proc  g_reg_proc ;
extern net_msg_proc		  g_msg_proc;
extern net_stream_proc	  g_stream_proc;
extern uint32_t			  g_param;

extern net_register_proc  gb_g_reg_proc ;
extern net_msg_proc		  gb_g_msg_proc;
extern net_stream_proc	  gb_g_stream_proc;
extern uint32_t			  gb_g_param;

extern int net_error_num;
extern unsigned int select_param;
extern int pu_initiative_cancellation;
extern void *stop_mutex;
extern unsigned int ms_platform_status;
extern unsigned int gb_platform_status;
/********************************************/

#endif/*__MS_PU_GLOB_H__*/
