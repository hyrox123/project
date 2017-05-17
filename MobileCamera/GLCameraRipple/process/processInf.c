//
//  processInf.cpp
//  mingview
//
//  Created by jason on 13-7-3.
//  Copyright (c) 2013年 jason. All rights reserved.
//

extern "C"
{
#include "ms_pu_lib.h"
#include "ms_pu_param.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavcodec/avcodec.h"
};

#include <queue.h>
#include <pthread.h>
#include "processInf.h"

#define MAX_RESERVER_FRAME    30

typedef enum
{
	R_NOCREATE,
	R_Accept,
	R_Worked,
	R_Error
} runningStatus;

typedef struct
{
    unsigned long int  flag;
    unsigned long int  size;
}NAL_HEADER;

typedef struct
{
	FILE* ptr_file;
    pthread_t pid;
	void* thrd;
	int   flag;
	int   video_id;
	int   media_type;
	int   stream_type;
	int   audio_skt;
	char  filename[64];
	unsigned long id;
	unsigned short unSeqNo;
    int count;
}NET_STREAM_OPEN_PARAM, *LPNET_STREAM_OPEN_PARAM;

typedef struct
{
	void* child_pid;
	runningStatus child_status;
	int  alarm_type;
}CHILD_PROCESS_INFO_S,*LCHILD_PROCESS_INFO_S;

typedef struct
{
	unsigned char *data;
	int size;
    int type;
}Packet_t;


int onDeviceLogin(int type, char* ip, unsigned short port, unsigned long error, unsigned long times, unsigned long param);
int onDeviceMessage(char* cmd, char* buf, unsigned long* size, unsigned long param);
int onDeviceStream(unsigned long id, char* cmd, char* buf, unsigned long size, unsigned long param);
void* onSendStream(void *lpParameter);
void* onEncodeh264(void* lpParameter);

int   initFFmpeg();
void  destroyFFmpeg();
Packet_t* popFrame();
void  pushFrame(Packet_t* pData);
void  clearAllFrame();
int   getFrameCount();

void pushRawData(unsigned char* pData);
unsigned char* popRawData();
int  getRawDataCount();
void  clearAllRawData();

Packet_t* GetPacket(int size);
void FreePacket(Packet_t* packet);
int writeFile(const char* _fileName, void* _buf, int _bufLen);
int readFile(const char* _fileName, void* _buf, int _bufLen);

//////////////////////////////////////////////////////////////////////////
static AVCodec *_avcodec = NULL;
static AVCodecContext *_avcontext = NULL;
static AVFrame *_yuv12frame = NULL;
static AVFrame *_yuv420pframe = NULL;
static std::queue<unsigned char*> _rawQueue;
static std::queue<Packet_t*> _frameQueue;
static pthread_mutex_t _frameMutex;
static pthread_mutex_t _rawMutex;
static pthread_mutex_t _counterMutex;
static bool _openStream = false;
static bool _instanceRun = false;
static int _width, _height, _srcfps, _dstfps, _count;

int onDeviceLogin(int type, char* ip, unsigned short port, unsigned long error, unsigned long times, unsigned long param)
{
    return 0; 
}

int onDeviceMessage(char* cmd, char* buf, unsigned long* size, unsigned long param)
{
    //set net config
	if(strcmp(cmd, SET_PU_IPINFO) == 0)
	{
		//LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;
	}
    else if(strcmp(cmd,GET_PU_IPINFO)== 0) 	//get net config
	{
		LPNET_CMU_IP_INFO basic = (LPNET_CMU_IP_INFO)buf;        
		basic->net_id = 2;
		strncpy(basic->ip_addr, "192.168.2.145", 15);
		basic->ctrl_port = 1001;
		strncpy(basic->net_mask,"255.255.255.0",15);
		strncpy(basic->dns_addr[0], "192.168.1.1", 15);
		strncpy(basic->gate_way, "192.168.1.1", 15);
	}
    else if(strcmp(cmd,SET_PU_TIME) == 0) 	//set device time
	{
        
	}
	else if(strcmp(cmd, OPER_PU_CONTROLPTZ) == 0)  //set ptz
	{
		//LPNET_CMU_OPER_CTRL_PTZ basic = (LPNET_CMU_OPER_CTRL_PTZ)buf;
	}
	else if(strcmp(cmd,SET_PU_PRESETPTZ) == 0)	//set presetpoint
	{
        
	}
	else if(strcmp(cmd, OPER_PU_QUERYPULOCALSTORAGEFILES) == 0) //set record
	{

        
	}
    else
    {
        
    }
    
	return 0; 
}

int onDeviceStream(unsigned long id, char* cmd, char* buf, unsigned long size, unsigned long param)
{    
	//video request
	if(strcmp(cmd, "Start_MEDIA") == 0)
	{
        printf("-----------onDeviceStream reciv Start_MEDIA\n");

        LPNET_MDU_START_MEDIA start_media = (LPNET_MDU_START_MEDIA)buf;
        
        //realtime video 
		if(start_media->request_type == 0)
		{
            if (_openStream == true)
            {
                printf("-----------onDeviceStream repeate\n");
                return 0;
            }
            else
            {
                printf("-----------onDeviceStream thread run\n");

                _openStream = true;
                static unsigned long videoid = id;
                static pthread_t _encodeThreadId;
                static pthread_t _sendThreadId;
                pthread_create(&_encodeThreadId, NULL, onEncodeh264, NULL);
                pthread_create(&_sendThreadId, NULL, onSendStream, &videoid);
                pthread_detach(_encodeThreadId);
                pthread_detach(_encodeThreadId);
                _count = 0;
                _dstfps = 0;
            }
		}
		else if(start_media->request_type == 1) //history video
		{
            
		}
		else if(start_media->request_type == 2) //download
		{
            
		}
	}
	else if(strcmp(cmd, "Stop_MEDIA") == 0) //video stop
	{
        printf("-----------onDeviceStream reciv Stop_MEDIA\n");
        
        _openStream = false;
        _count = 0;
        _dstfps = 0;
	}
	else if(strcmp(cmd, "Start_AUDIO") == 0) //audio
	{
        
	}
	else if(strcmp(cmd, "Stop_AUDIO") == 0) //audio stop
	{
        
	}
	else if(strcmp(cmd, "Control_MEDIA") == 0) //playback
	{
        
	}
    
	return 0; 
}

int registerPu(PuRegisterInf puInf)
{
    if (_instanceRun)
    {
        printf("-------instance already run!\n");
        return -1;
    }
    
    net_info_t info = {0};
    sprintf(info.ms_info.pu_id, "%s", puInf.puid);
    sprintf(info.ms_info.name, "%s", puInf.name);
    sprintf((char*)info.ms_info.passwd, "%s", puInf.psw);
    sprintf(info.ms_info.ip, "%s", puInf.ip);
    
    info.ms_info.port = puInf.port;
    info.ms_info.company = puInf.company;
    info.ms_info.pu_type = puInf.putype;
    info.select_platform = MINGSOFT_PLATFORM;
    
    _width = puInf.width;
    _height = puInf.height;
    _srcfps = puInf.fps;
    
    net_initlib(info, onDeviceLogin, onDeviceMessage, onDeviceStream, MINGSOFT_PLATFORM);
    _instanceRun = true;
    return 0;
}


void destoryPu()
{
    _openStream = false;
    net_fililib(MINGSOFT_PLATFORM);
    _instanceRun = false;
}


int initFFmpeg()
{
    if (_avcodec != NULL)
    {
        printf("-----------initFFmpeg repeate init error\n");
        return -1;
    }
    
     avcodec_register_all();
    _avcodec = avcodec_find_encoder(CODEC_ID_H264);
    
    if (_avcodec == NULL)
    {
        printf("-----------avcodec_find_decoder error\n");
        return -1;
    }
    
    _avcontext = avcodec_alloc_context3(_avcodec);
    
    if (_avcontext == NULL)
    {
        _avcodec = NULL;
        printf("-----------avcodec_alloc_context3 error\n");
        return -1;
    }
    
    _avcontext->pix_fmt = PIX_FMT_YUV420P;
    _avcontext->codec_type = AVMEDIA_TYPE_VIDEO;
    _avcontext->width  = _width;
    _avcontext->height = _height;
    _avcontext->time_base.num = 1;
    _avcontext->time_base.den = _srcfps;
    _avcontext->gop_size = _srcfps * 2;
    _avcontext->keyint_min = _srcfps;
    _avcontext->max_b_frames = 0;
    _avcontext->bit_rate = _width * _height * 3 / 2;
    _avcontext->rc_max_rate = _avcontext->bit_rate;
    _avcontext->rc_min_rate = _avcontext->bit_rate;
    _avcontext->bit_rate_tolerance = _avcontext->bit_rate;
    _avcontext->rc_buffer_size = _avcontext->rc_max_rate;
    _avcontext->rc_initial_buffer_occupancy = _avcontext->rc_buffer_size*3/4;
    _avcontext->rc_buffer_aggressivity= (float)1.0f;
    _avcontext->rc_initial_cplx= 0.5;
    _avcontext->thread_count = 4;

    _avcontext->refs = 4;
    _avcontext->flags |= CODEC_FLAG_LOOP_FILTER;
    _avcontext->level = 30;
    
    _avcontext->qcompress = (float)0.6f;
    _avcontext->qmin = 10; 
    _avcontext->qmax = 30;   
    _avcontext->max_qdiff = 4;
    
    if (avcodec_open2(_avcontext, _avcodec, NULL) < 0)
    {
        avcodec_close(_avcontext);
        av_free(_avcontext);
        _avcontext = NULL;
        _avcodec = NULL;
        printf("------------avcodec_open2 failed\n");
        return -1;
    }
    
    _yuv12frame = avcodec_alloc_frame();
    _yuv420pframe = avcodec_alloc_frame();
    
    if (_yuv12frame == NULL || _yuv420pframe == NULL)
    {
        avcodec_close(_avcontext);
        av_free(_avcontext);
        _avcontext = NULL;
        _avcodec = NULL;
       
        if(_yuv12frame != NULL)
        {
            av_free(_yuv12frame);
            _yuv12frame = NULL;
        }
        
        if(_yuv420pframe != NULL)
        {
            av_free(_yuv420pframe);
            _yuv420pframe = NULL;
        }
        
        printf("-----------avcodec_alloc_frame error\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

void destroyFFmpeg()
{
    if (_avcodec != NULL)
    {
        _avcodec = NULL;
    }
    
    if(_avcontext != NULL)
    {
        avcodec_close(_avcontext);
        av_free(_avcontext);
        _avcontext = NULL;
    }
    
    if(_yuv12frame != NULL)
    {
        av_free(_yuv12frame);
        _yuv12frame = NULL;
    }
    
    if(_yuv420pframe != NULL)
    {
        av_free(_yuv420pframe);
        _yuv420pframe = NULL;
    }    
}

int writeFile(const char* _fileName, void* _buf, int _bufLen)
{
    FILE * fp = NULL;
    if( NULL == _buf || _bufLen <= 0 ) return (-1);
    
    NSArray * documents = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);NSString * documentDirectory = [documents objectAtIndex:0];

    NSString *path = [documentDirectory stringByAppendingPathComponent:@"h264.dat"];
    
    fp = fopen([path UTF8String], "a+");
    
    if( NULL == fp )
    {
        return (-1);
    }
    
    fwrite(_buf, _bufLen, 1, fp); 
    
    fclose(fp);
    fp = NULL;
    
    return 0;
}

int readFile(const char* _fileName, void* _buf, int _bufLen)
{
    FILE* fp = NULL;
    if( NULL == _buf || _bufLen <= 0 ) return (-1);
    
    fp = fopen(_fileName, "rb"); 
    
    if( NULL == fp )
    {
        return (-1);
    }
    
    fread(_buf, _bufLen, 1, fp); 
    
    fclose(fp);
    return 0;        
}


bool onRecvRawData(unsigned char *pData, int size)
{
    if (_openStream == false)
    {
        return false;
    }
    
    unsigned char* ptr = (unsigned char*)malloc(size);
    if (ptr != NULL)
    {
        memcpy(ptr, pData, size);
        pushRawData(ptr);
        return true;
    }
    else
    {
        printf("---- malloc error\n");
        return false;
    }
}


/* --- rgb
 uint8_t *rgb_src[3] = {src, NULL, NULL};
 int rgb_stride[3] ={4*width, 0, 0};
 
 tmp = (unsigned char*)av_malloc(width*height*4);
 
 if (tmp == NULL)
 {
 return NULL;
 }
 
 _yuv420pframe = avcodec_alloc_frame();
 avpicture_fill((AVPicture*)_yuv420pframe, tmp, PIX_FMT_YUV420P, width, height);
 
 SwsContext* scxt = sws_getContext(width, height, AV_PIX_FMT_BGR32,
 width, height,
 PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
 
 int retValue = sws_scale(scxt, rgb_src, rgb_stride,
 0, height, _yuv420pframe->data, _yuv420pframe->linesize);
 
 */


void* onEncodeh264(void* lpParameter)
{
    pthread_mutex_init(&_frameMutex, NULL);
    pthread_mutex_init(&_rawMutex, NULL);
    pthread_mutex_init(&_counterMutex, NULL);

    int retValue = initFFmpeg();
    
    if(retValue < 0)
    {
        printf("ffmpeg init failed\n");
        _openStream = false;
        usleep(10000);
    }

    int64_t frameid = 0;
    
    while (_openStream == true)
    {
        unsigned char *src = popRawData();
        
        if (src == NULL)
        {
            usleep(1);
        }
        else
        {
            avpicture_fill((AVPicture*)_yuv12frame, src, PIX_FMT_NV12,
                           _width, _height);
            
            unsigned char *tmp = (unsigned char*)av_malloc(_width*_height*3);
            
            if (tmp == NULL)
            {
                printf("malloc error\n");
                break;
            }
            
            avpicture_fill((AVPicture*)_yuv420pframe, tmp, PIX_FMT_YUV420P, _width, _height);
            
            struct  SwsContext *scxt = sws_getContext(_width, _height, PIX_FMT_NV12, _width, _height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);
            
            retValue = sws_scale(scxt,(const uint8_t**)_yuv12frame->data, _yuv12frame->linesize, 0, _height, _yuv420pframe->data, _yuv420pframe->linesize);
            
            sws_freeContext(scxt);
            free(src);
            
            AVPacket packet;
            av_init_packet(&packet);
            packet.data = NULL;
            packet.size = 0;
            
            _yuv420pframe->width = _width;
            _yuv420pframe->height = _height;
            _yuv420pframe->pts = frameid * (_avcontext->time_base.num * 1000 / _avcontext->time_base.den);
            _yuv420pframe->owner = _avcontext;
            frameid++;
            
            avcodec_encode_video2(_avcontext, &packet, _yuv420pframe, &retValue);
            free(tmp);
            
            if (retValue <= 0)
            {
                
            }
            else
            {                
                Packet_t *dst = GetPacket(packet.size);
                if (dst != NULL)
                {
                    memcpy(dst->data, packet.data, packet.size);
                    dst->size = packet.size;
                    pushFrame(dst);
                    dst = NULL;
                }
                else
                {
                    av_free_packet(&packet);
                    printf("------------GetPacket failed\n");
                    break;
                }
            }
            
            av_free_packet(&packet);
        }
    }
    
    destroyFFmpeg();
    clearAllFrame();
    clearAllRawData();
    
    pthread_mutex_destroy(&_frameMutex);
    pthread_mutex_destroy(&_rawMutex);
    pthread_mutex_destroy(&_counterMutex);

    return 0;
}

void* onSendStream(void* lpParameter)
{
    printf("------------onSendStream thread run\n");

    unsigned long *videoId = (unsigned long*)lpParameter;
    
    while (_openStream == true)
    {
        Packet_t *ptr = popFrame();
        
        if (ptr == NULL)
        {
            usleep(1);
        }
        else
        {
            int retValue = net_send_stream(*videoId, (char*)ptr->data, ptr->size, 0);
            
            if (retValue == -1)
            {
                NSLog(@"faild to send data");
            }
            
            FreePacket(ptr);
            
            pthread_mutex_lock(&_counterMutex);
            _count += ptr->size;
            _dstfps++;
            pthread_mutex_unlock(&_counterMutex);
        }
    }

    return 0;
}


void pushFrame(Packet_t* pData)
{
    pthread_mutex_lock(&_frameMutex);
    _frameQueue.push(pData);
    pthread_mutex_unlock(&_frameMutex);
}

Packet_t* popFrame()
{
    Packet_t *ptr = NULL;
    pthread_mutex_lock(&_frameMutex);
    if (!_frameQueue.empty())
    {
        ptr = _frameQueue.front();
        _frameQueue.pop();
    }
    pthread_mutex_unlock(&_frameMutex);
    return ptr;
}

int  getFrameCount()
{
    pthread_mutex_lock(&_frameMutex);
    int count = _frameQueue.size();
    pthread_mutex_unlock(&_frameMutex);
    return count;
}

void  clearAllFrame()
{
    pthread_mutex_lock(&_frameMutex);
    while (!_frameQueue.empty())
    {
        Packet_t *ptr = _frameQueue.front();
        _frameQueue.pop();
        FreePacket(ptr);
    }
    pthread_mutex_unlock(&_frameMutex);
}

void pushRawData(unsigned char* pData)
{
    pthread_mutex_lock(&_rawMutex);
    _rawQueue.push(pData);
    pthread_mutex_unlock(&_rawMutex);
}

unsigned char* popRawData()
{
    unsigned char *ptr = NULL;
    pthread_mutex_lock(&_rawMutex);
    if (!_rawQueue.empty())
    {
        ptr = _rawQueue.front();
        _rawQueue.pop();
    }
    pthread_mutex_unlock(&_rawMutex);
    return ptr;
}

int  getRawDataCount()
{
    pthread_mutex_lock(&_rawMutex);
    int count = _rawQueue.size();
    pthread_mutex_unlock(&_rawMutex);
    return count;
}

void  clearAllRawData()
{
    pthread_mutex_lock(&_rawMutex);
    while (!_frameQueue.empty())
    {
        unsigned char *ptr = _rawQueue.front();
        _rawQueue.pop();
        free(ptr);
    }
    pthread_mutex_unlock(&_rawMutex);
}

Packet_t* GetPacket(int size)
{
    unsigned char *data = (unsigned char*)malloc(size);
    
    if (data)
    {
        memset(data, 0, size);
    }
    else
    {
        return NULL;
    }
    
    Packet_t *packet = (Packet_t*)malloc(sizeof(Packet_t));
    
    if (packet)
    {
        packet->data = data;
        packet->size = 0;
    }
    else
    {
        return NULL;
    }
    
    return packet;
}

void FreePacket(Packet_t* packet)
{
    if (packet)
    {
        if (packet->data)
        {
            free(packet->data);
            packet->data = NULL;
        }
        
        free(packet);
        packet = NULL;
    }
}

int getSendDataSize()
{
    pthread_mutex_lock(&_counterMutex);
    int retValue = _count;
    _count = 0;
    pthread_mutex_unlock(&_counterMutex);
    return retValue;
}

int getSendFrameSize()
{
    pthread_mutex_lock(&_counterMutex);
    int retValue = _dstfps;
    _dstfps = 0;
    pthread_mutex_unlock(&_counterMutex);
    return retValue;
}



