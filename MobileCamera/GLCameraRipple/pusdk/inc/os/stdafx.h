#ifndef __STDAFX_H__
#define __STDAFX_H__


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>


#ifdef WIN32
	#include <sys\timeb.h>
	#include <Winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment( lib, "Ws2_32.lib" )
/*
	typedef char int8_t;
	typedef unsigned char uint8_t;

	typedef int int32_t;
    typedef unsigned int size_t;
	typedef unsigned long uint32_t;

	typedef short int16_t;
	typedef unsigned short uint16_t;
	
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;
*/
	

#else


	#include <errno.h>
	#include <inttypes.h>
	#include <semaphore.h>
	#include <time.h>
	#include <unistd.h>
	#include <syslog.h>
	#include <sys/ioctl.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/socket.h>
	#include <sys/wait.h>
	#include <sys/un.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <pthread.h>
	#include <netdb.h>
	//#include <linux/unistd.h>
	#include <dirent.h>
//	#include <bits/types.h>
	#include <unistd.h>
	#include <fcntl.h>
    #include <unistd.h>
    #include <sys/types.h>

	typedef unsigned long       DWORD;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
#endif	//win32






#ifndef __cplusplus

#ifndef bool
#define bool unsigned int
#endif	
#ifndef true
#define true 1
#endif	
#ifndef false
#define false 0
#endif 

	
#endif



#ifndef WIN32
#ifndef SOCKET
#define SOCKET int
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#ifndef	closesocket
#define closesocket close
#endif
#endif




#ifndef HANDLE
#define HANDLE void*
#endif



#ifndef NULL
#define NULL		0
#endif

#ifndef NO_WAIT
#define NO_WAIT       0
#endif

#ifndef WAIT_FOREVER
#define WAIT_FOREVER -1
#endif

#ifdef WORDS_BIGENDIAN
	#define IS_MULTICAST_IP(ip) ( 0xE0000000<=(ip&0xFF000000) && (ip&0xFF000000)<0xF0000000 )
#else
	#define IS_MULTICAST_IP(ip) ( 0xE0<=(ip&0xFF) && (ip&0xFF)<0xF0 )	
#endif

#endif //__STDAFX_H__

