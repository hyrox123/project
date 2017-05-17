//
//  processInf.h
//  mingview
//
//  Created by jason on 13-7-3.
//  Copyright (c) 2013年 jason. All rights reserved.
//

#ifndef __mingview__processInf__
#define __mingview__processInf__

typedef struct
{
    const char *puid;
    const char *name;
    const char *psw;
    const char *ip;
    int port;
    int company;
    int putype;
    int width;
    int height;
    int fps;
}PuRegisterInf;

int   registerPu(PuRegisterInf puinf);
void  destoryPu();
bool  onRecvRawData(unsigned char *pData, int size);
int   getSendDataSize();
int   getSendFrameSize();

#endif /* defined(__mingview__processInf__) */
