#include "stdafx.h"
#include "help.h"
#include "ms_pu_glob.h"
#define LOG_SERVER_PORT 17777

char g_log_server[20]="";
char gb_g_log_server[20] = "";

int print_outmsg(const char *fmt, ...)
{
    //printf("[LOG .%s.%d][%s]",__FUNCTION__,__LINE__,HOSTMGR_NAME);
    int fd = 0;
    int type=16;
    struct sockaddr_in lServerAddr;
    socklen_t lServerlAddrLen = sizeof(lServerAddr);
    char m_pszLine[2048];
    char log_serverIp[20];
    char *s;
    struct timeval tv;
    int timeout = 1000;
    fd_set wset;
    va_list	ap;
    int size = 0;
    if(select_param == MINGSOFT_PLATFORM)
    {
        strncpy(log_serverIp,g_log_server,sizeof(g_log_server));
        //printf("log_serverIp %s \n",log_serverIp);
    }
    else if(select_param == GB28181_PLATFORM)
    {
        strncpy(log_serverIp,gb_g_log_server,sizeof(gb_g_log_server));
        //printf("log_serverIp %s \n",log_serverIp);

    }
    else
    {
        printf("wrong select_param\n");
    }
    if(strlen(log_serverIp) > 0)
    {

        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd < 0)
        {
            fprintf(stderr,"socket failed %d %s \n",errno,strerror(errno));
            return -1;
        }
        
        memset(&lServerAddr, 0, sizeof(lServerAddr));
        lServerAddr.sin_family = AF_INET;
        lServerAddr.sin_port = htons(LOG_SERVER_PORT);
        lServerAddr.sin_addr.s_addr = inet_addr(log_serverIp);

        memset(m_pszLine, 0, sizeof(m_pszLine));
        
        sprintf(m_pszLine,"[%s]%d [%s]",ms_g_info.ms_info.pu_id,type,VERSION);
        
        
        s = m_pszLine+strlen(m_pszLine);

        va_start(ap, fmt);
        vsprintf(s, fmt, ap);
        /*
        用 法: int vsprintf(char *string, char *format, va_list param);
        将param 按格式format写入字符串string中
        */
        va_end(ap);
        //	printf("[LOG]%s\n",m_pszLine);
        
        tv.tv_sec = timeout/1000;
	    tv.tv_usec = (timeout%1000)*1000;
	    FD_ZERO( &wset );
	    FD_SET( (uint32_t)fd, &wset );
    //int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout); 
	    if( select( fd+1, 0, &wset, 0, &tv ) <= 0 )
	    {
            printf("timeout\n");
            return -1;
        }
        size = sendto(fd, 
                    m_pszLine, 
                    strlen(m_pszLine), 
                    0, 
                    (struct sockaddr *)&lServerAddr, 
                    lServerlAddrLen);
        if(size < 0)
        {
            fprintf(stderr,"sendto failed %d %s \n",errno,strerror(errno));
            close(fd);
            return -1;
            
        }
        else
        {
            //printf("m_pszLine %s\n",m_pszLine);
        }
        if(fd > 0)
        {
           close(fd);
        }
        return 0;
    }
    else
    {
        printf("log_server_ip is Ilegal\n");
        va_start(ap, fmt);
        va_end(ap);
        return -1;
    }

    return -1;
}


