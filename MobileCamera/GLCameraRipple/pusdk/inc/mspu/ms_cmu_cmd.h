#ifndef __MS_CMU_CMD_H__
#define __MS_CMU_CMD_H__

//#include "stdio.h"
//#include "string.h"


/*******************************************************************************/
/* --- msg type id------*/
#define MSG_REG								0x01
#define MSG_GET								0x02
#define MSG_SET								0x03
#define MSG_OPER							0x04
#define MSG_UPLOAD							0x05

/*******************************************************************************/
/* --- cmd register id------*/
#define CMD_REG_REGISTER					0x001
#define CMD_REG_KEEPALIVE					0x002

/*******************************************************************************/
/* --- cmd getting id------*/
#define CMD_GET_BASICINFO					0x101
#define CMD_GET_IPINFO						0x102
#define CMD_GET_USER						0x103
#define CMD_GET_SERIALPORT					0x104
#define CMD_GET_INOUTDEV					0x105
#define CMD_GET_TIME						0x106
#define CMD_GET_VIDEOFORMAT					0x107
#define CMD_GET_IMAGEENCODEPARA				0x108
#define CMD_GET_IMAGEDISPLAYPARA			0x109
#define CMD_GET_IMAGETEXTPARA				0x110
#define CMD_GET_ALARMPARAMS					0x111
#define CMD_GET_GIPINALARM					0x112
#define CMD_GET_VIDEOLOSE					0x113
#define CMD_GET_HIDEDETECTION				0x114
#define CMD_GET_MOTIONDETECTION				0x125
#define CMD_GET_IMAGEHIDEAREA				0x126
#define CMD_GET_LOCALSTORAGETASK			0x117
#define CMD_GET_PRESETPTZ					0x118
#define CMD_GET_DEFAULTPRESETPTZ			0x119
#define CMD_GET_CRUISETRACK					0x120
#define CMD_GET_STATE						0x121
#define CMD_GET_LOG							0x122
#define CMD_GET_SOFTWAREVERSION				0x123

/*******************************************************************************/
/* --- cmd setting id------*/
#define CMD_SET_BASICINFO					0x201
#define CMD_SET_IPINFO						0x202
#define CMD_SET_USER						0x203
#define CMD_SET_SERIALPORT					0x204
#define CMD_SET_INOUTDEV					0x205
#define CMD_SET_TIME						0x206
#define CMD_SET_VIDEOFORMAT					0x207
#define CMD_SET_IMAGEENCODEPARA				0x208
#define CMD_SET_IMAGEDISPLAYPARA			0x209
#define CMD_SET_IMAGETEXTPARA				0x210
#define CMD_SET_ALARMPARAMS					0x211
#define CMD_SET_GIPINALARM					0x212
#define CMD_SET_VIDEOLOSE					0x213
#define CMD_SET_HIDEDETECTION				0x214
#define CMD_SET_MOTIONDETECTION				0x215
#define CMD_SET_IMAGEHIDEAREA				0x216
#define CMD_SET_LOCALSTORAGETASK			0x217
#define CMD_SET_PRESETPTZ					0x218
#define CMD_SET_DEFAULTPRESETPTZ			0x219
#define CMD_SET_CRUISETRACK					0x220

/*******************************************************************************/
/* --- cmd operation id------*/
#define CMD_OPER_DELUSER					0x301
#define CMD_OPER_CLEANPULOCALSTORAGEFILE	0x302
#define CMD_OPER_QUERYPULOCALSTORAGEFILES	0x303
#define CMD_OPER_CONTROLPTZ					0x304
#define CMD_OPER_DELPRESETPTZ				0x305
#define CMD_OPER_CONTROLPU					0x306
#define CMD_OPER_TRANSPARENTCHANNEL			0x307
/*******************************************************************************/
#define CMD_OPER_CONTROLFILEBACK            0x308    
/*******************************************************************************/
/* --- cmd upload id------*/
#define CMD_UPLOAD_RAISEALARM				0x401
#define CMD_UPLOAD_RAISEDIGITAL				0x402


int get_msg_cmd_id(unsigned short *msg_id, unsigned short *cmd_id, const char* const_msg_name);

#endif//__MS_CMD_H__
