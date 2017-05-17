#include "stdafx.h"
#include "ossys.h"
#include "ossocket.h"
#include "help.h"

/* ����socket, af=AF_INET,..., type=SOCK_STREAM, ... , protocol=IPPROTO_TCP,... */
int32_t os_socket_create( int32_t af, int32_t type, int32_t protocol )
{
	//UDP AF_INET, SOCK_DGRAM, 0
	//TCP AF_INET, SOCK_STREAM, 0
	int32_t skt;
	int32_t bufsize = 210*1024;
	struct linger lgr;

	skt = socket( af, type, protocol );
	if( skt != -1 )
	{
		lgr.l_onoff=1;
		lgr.l_linger=0;
        //BOOL bDontLinger = FALSE; 
		setsockopt( skt, SOL_SOCKET , SO_LINGER , &lgr , sizeof(lgr) );//�ӳٹر�����
		setsockopt( skt, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize, sizeof(bufsize) );//���ܻ������Ĵ�С
		setsockopt( skt, SOL_SOCKET, SO_SNDBUF, (char *)&bufsize, sizeof(bufsize) );//���ͻ������Ĵ�С

        //setsockopt(s,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));
		printf("skt <<<%d>>>\n",skt);
	}
    else
    {
        printf("[os_socket_create]%s %d",strerror(errno),errno);
        LOG("[os_socket_create]%s %d",strerror(errno),errno);
    }
	return skt;
}

/* ����socket */
void os_socket_destroy( int32_t skt )
{
	if( skt != -1 )
		close( skt );
}

ssize_t  socket_writen(int fd, const void *vptr, size_t n)
{	
	ssize_t nwritten;
	size_t nleft=n;
	const char *ptr=(char *)vptr;
	register int c=0;

	while (nleft > 0) 
	{
		if ( (nwritten = send(fd, ptr, nleft,0)) <= 0) 
		{			
			if (nwritten < 0 && errno == EINTR)
			{
				printf("rrr %d %d %s\n",fd,errno,strerror(errno));
				nwritten = 0;   /* and call write() again */
			}
			else
			{
                printf("rrr %d %d %s\n",fd,errno,strerror(errno));
                return (-1);    /* error */
            }
				
		}		

		if (++c>50)
		{
			printf("nwritten %d\n",nwritten);
			return -1;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

/* ����Զ�� */
int32_t os_socket_connect( int32_t skt, char* strip, uint16_t port, int32_t timeout )
{
	struct sockaddr_in addr;
	uint32_t unblock = 1;
	fd_set rset;
	struct timeval tv;
	int32_t len;

	memset( &addr, 0, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(strip);
	addr.sin_port = htons(port);

	len = sizeof(struct sockaddr);

	if( timeout < 0 )
		return connect( skt, (struct sockaddr*)&addr, len);

	//����Ϊ������״̬
	if( ioctl( skt, FIONBIO, &unblock ) < 0 )
		return -1;

	//��ʼ����
	if((connect( skt, (struct sockaddr *)&addr, sizeof(addr) )) == -1)
	{
        printf("[os_socket_connect]%s %d\n",strerror(errno),errno);
        LOG("[os_socket_connect]%s %d\n",strerror(errno),errno);
    }
    

	//�ȴ����ӷ���
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;
	FD_ZERO( &rset );
	FD_SET( (uint32_t)skt, &rset );
    //int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout); 
	if( select( skt+1, 0, &rset, 0, &tv ) <= 0 )
	{
        printf("[os_socket_connect] select \n");
        LOG("[os_socket_connect] select \n");
        return -1;
    }
		

	//�������ģʽ
	unblock = 0;
	return ioctl( skt, FIONBIO, &unblock );

}


/* �󶨽��յ�ַ�Ͷ˿�, severʹ��bind()�뱾���󶨣�clientʹ��connect()��Զ���������� */
int32_t os_socket_bind( int32_t skt, char* strip, uint16_t port )
{
	struct sockaddr_in addr;
	memset( &addr, 0, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(strip);
	addr.sin_port = htons(port);
	return bind ( skt, (struct sockaddr *)&addr, sizeof(addr) );
}


/* ��ʼ����, maxΪ���Ҷ��г��ȣ�������Ϊ5 */
int32_t os_socket_listen( int32_t skt, int32_t max )
{
	return listen( skt, max );
}

/* �ȴ�Զ������ */
int32_t os_socket_accept( int32_t skt, char* strip, uint16_t *port )
{
	struct sockaddr_in from;
	socklen_t slen = sizeof(from);

	/*�ȴ�CLIENT������*/
	skt = accept( skt, (struct sockaddr *)&from, &slen );
	if( 0 <= skt )
	{
		sprintf(strip, "%s", inet_ntoa( from.sin_addr) );
		*port = ntohs( from.sin_port );
	}
	return skt;
}

/* ����������� */
int32_t os_socket_ioctl( int32_t skt, int32_t type, uint32_t *val )
{
	return ioctl( skt, (long)type, val );
}

/* ���ò��� */
int32_t os_socket_opt( int32_t skt, int32_t level, int32_t optname, const char* optval, int32_t optlen )
{
      #if 0
      int optval_t;  
      optval_t = 1;  
      //int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);  
      setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &optval_t, sizeof(optval_t));
      #ifdef SO_REUSEPORT  
      optval_t = 1;  
      setsockopt(skt, SOL_SOCKET, SO_REUSEPORT, &optval_t, sizeof(optval_t));
      #endif
      #endif
	return setsockopt( skt, level, optname, optval, optlen );
}

/* ���״̬,timeout��λms, timeout=-1��ʾ���޵ȴ� */
int32_t os_socket_select( int32_t maxfd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, int32_t timeout )
{
	struct timeval tv;

	if( 0 <= timeout )
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		return select( maxfd, readfds, writefds, exceptfds, &tv );
	}
	else
		return select( maxfd, readfds, writefds, exceptfds, NULL );
}


/* �������� */
int32_t os_socket_send( int32_t skt, void *buf, int32_t size, int32_t timeout )
{

	struct timeval tv;
//	uint32_t unblock = 1;
	fd_set wset;

	if( timeout < 0 ){
		//printf("send\n");
		
		
        size = send( skt, buf, size, 0);
		if( size<0 )
        {
			printf("\n==== Net Send Stream error  errno:%d,%s=====\n",errno,strerror(errno));
			return -1;
		}
        //printf("skt/size<<<%d/%d>>>\n",skt,size);
        return size;
	}
	else 
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		FD_ZERO( &wset );
		FD_SET( (uint32_t)skt, &wset );

		if( select( skt+1, 0, &wset, 0, &tv ) <= 0 ){
		printf("\n==== NET ERROR AT  SELECT FUNCTION  TIME ON os_socket_send=====\n");
        LOG("\n==== NET ERROR AT  SELECT FUNCTION  TIME ON os_socket_send=====\n");
		return -1;
		}
	//	sleep(1);
		//printf("send\n");
		size = send( skt, buf, size, 0);
		if( size<=0 )
        {
			printf("\n==== Net Send Stream error  errno:%d,%s=====\n",errno,strerror(errno));
            LOG("\n==== NET ERROR AT  SELECT FUNCTION  TIME ON os_socket_send=====\n");
			return -1;
		}
        //printf("skt/size<<<%d/%d>>>\n",skt,size);
	}
	

	return size;
}

/* �������� */
int32_t os_socket_recv( int32_t skt, void *buf, int32_t size, int32_t timeout )
{
	struct timeval tv;
//	uint32_t unblock = 1;
	fd_set rset;

	if( timeout < 0 )
		return recv( skt, buf, size, 0 );//timeoutʲôʱ��С��0
	else
	{
// 		//����Ϊ������״̬
// 		if( ioctlsocket( skt, FIONBIO, &unblock ) < 0 )
// 			return -1;

		//�ȴ����ݵ���
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		FD_ZERO( &rset );
		FD_SET( (uint32_t)skt, &rset );
		if( select( skt+1, &rset, 0, 0, &tv ) <= 0 ){
			//printf("\n==== NET  ERROR  IN  SELECT FUNCTION  TIME ON os_socket_recv=====\n");
			return 0;//select����0Ϊ��
		}

		//��ʼ����
		size = recv( skt, buf, size, 0 );
		if( size==0 ){

			//printf("\n==== NET ERROR AT  RECIVE  PACKET TIME ON os_socket_recv=====\n");
			return -1;

		}

// 		//�ָ�Ϊ����
// 		unblock = 0;
// 		ioctlsocket( skt, FIONBIO, &unblock );
		return size;
	}
}


/* ����ಥ��, ip�������鲥��ַ */
int32_t os_socket_join( int32_t skt, char* strip )
{
	int32_t ttl = 127;
	int32_t fFlag = 1;
	struct ip_mreq mreq;

	memset( &mreq, 0, sizeof(mreq) );
	mreq.imr_multiaddr.s_addr = inet_addr(strip);
	mreq.imr_interface.s_addr = INADDR_ANY;

	/* ������ȱʡ�Ż����� */
	setsockopt( skt, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl) );
	setsockopt( skt, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag) );

	/* ����ಥ�� */
	return setsockopt( skt, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq) );
}


/* �˳��鲥��, ip�������鲥��ַ  */
int32_t os_socket_drop( int32_t skt, char* strip )
{
	struct ip_mreq mreq;
	memset( &mreq, 0, sizeof(mreq) );
	mreq.imr_multiaddr.s_addr = inet_addr(strip);
	mreq.imr_interface.s_addr = INADDR_ANY;
	return setsockopt( skt, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq) );
}

/* �������� */
int32_t os_socket_sendto( int32_t skt, char* strip, uint16_t port, void *buf, int32_t size, int32_t timeout )
{
	struct timeval tv;
//	uint32_t unblock = 1;
	fd_set wset;

	struct sockaddr_in sn;
	int32_t slen = sizeof(sn);

	memset( &sn, 0, sizeof(sn) );
	slen = sizeof(sn);
	sn.sin_family = AF_INET;
	sn.sin_addr.s_addr = inet_addr(strip);
	sn.sin_port=  htons(port);

	if( timeout < 0 )
		return sendto( skt, buf, size, 0, (struct sockaddr *)&sn, slen );
	else 
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		FD_ZERO( &wset );
		FD_SET( (uint32_t)skt, &wset );
		if( select( skt+1, 0, &wset, 0, &tv ) <= 0 )
			return 0;

		size = sendto( skt, buf, size, 0, (struct sockaddr *)&sn, slen );
		if( size<=0 )
			{
                LOG("[os_socket_sendto] sendto failed  %d %s %d %s\n ",skt,buf,errno,strerror(errno));
				return -1;
			}

			
		return size;
	}
}

/* �������� */
int32_t os_socket_recvfrom( int32_t skt, char* strip, size_t *port, void *buf, int32_t size, int32_t timeout )
{
	struct timeval tv;
//	uint32_t unblock = 1;
	fd_set rset;

	struct sockaddr_in from;
	socklen_t slen = sizeof( from );

	memset( &from, 0, sizeof(from) );


	if( timeout < 0 )
		return recvfrom( skt, buf, size, 0, (struct sockaddr *)&from, &slen );
	else
	{
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		FD_ZERO( &rset );
		FD_SET( (uint32_t)skt, &rset );
		if( select( skt+1, &rset, 0, 0, &tv ) <= 0 )
			return 0;

		slen = recvfrom( skt, buf, size, 0, (struct sockaddr *)&from, &slen );
		if( slen==0 )
			return -1;

		sprintf(strip, "%s", inet_ntoa( from.sin_addr) );
		*port = ntohs( from.sin_port );

		return slen;
	}
}
int32_t DS_Connect(int32_t iSocket, char* pcIPAddr, uint16_t iPort, int32_t iNsec)
{
    int iFlag;
    int iRet;
    int iError = 0;
    fd_set stRset;
    fd_set stWset;
    socklen_t stLen;
    struct timeval stTval;
    struct sockaddr_in stServerIP;


    memset(&stServerIP, 0, sizeof(stServerIP));
    stServerIP.sin_family = AF_INET;      
    stServerIP.sin_port = htons(iPort);
    stServerIP.sin_addr.s_addr = inet_addr(pcIPAddr);

	if(NULL == pcIPAddr)
    {
        printf("pcIPAddr is null!\n");
        return 1;
    }

	iFlag = fcntl(iSocket, F_GETFL, 0);
    fcntl(iSocket, F_SETFL, iFlag | O_NONBLOCK);

	  iRet = connect(iSocket, 
	               (struct sockaddr *)(&stServerIP), 
	               sizeof(stServerIP));

	if(iRet < 0)
    {
        if(errno != EINPROGRESS)
        {
            return -1;
        }
    }

	if(0 == iRet)
	{
		   goto done;
	}

	FD_ZERO(&stRset);
    FD_SET(iSocket, &stRset);
    stWset = stRset;
    stTval.tv_sec = iNsec;
    stTval.tv_usec = 0;
    iRet = select(iSocket + 1, 
                  &stRset, 
                  &stWset, 
                  NULL, 
                  iNsec ? &stTval : NULL);

	
    if(0 == iRet)
    {
    	printf("\n clsoe file _1 \n");
        iRet = close(iSocket);
		if(iRet < 0)
			perror(" close file falie");
		
        return -2;
    }


	if(FD_ISSET(iSocket, &stRset) || FD_ISSET(iSocket, &stWset))
	{
		stLen = sizeof(iError);
		iRet = getsockopt(iSocket, 
						  SOL_SOCKET, 
						  SO_ERROR, 
						  &iError, 
						  &stLen);
		if(iRet < 0)
		{
			return -1;
		}
	}

done:
    fcntl(iSocket, F_SETFL, iFlag);
    if(iError != 0)
    {
    	printf("\n clsoe file _2 \n");
       iRet = close(iSocket);
	   	if(iRet < 0)
			perror(" close file falie");
	
        return -2;
    } 


	return 0;


}




