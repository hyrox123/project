#ifndef __OSSOCKET_H__
#define __OSSOCKET_H__


#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ����socket, ����-1��ʾʧ��, type=SOCK_STREAM, ... , protocol=IPPROTO_TCP,... */
int32_t os_socket_create( int32_t af, int32_t type, int32_t protocol );

/* ����socket */
void os_socket_destroy( int32_t skt );

/* ����Զ�� */
int32_t os_socket_connect( int32_t skt, char* strip, uint16_t port, int32_t timeout);

/* �󶨽��յ�ַ�Ͷ˿� */
int32_t os_socket_bind( int32_t skt, char* strip, uint16_t port );

/* ��ʼ����, maxΪ���Ҷ��г��ȣ�������Ϊ5 */
int32_t os_socket_listen( int32_t skt, int32_t max );

/* �ȴ�Զ������ */
int32_t os_socket_accept( int32_t skt, char* strip, uint16_t *port );

/* ����������� */
int32_t os_socket_ioctl( int32_t skt, int32_t type, uint32_t *val );

/* ���ò��� */
int32_t os_socket_opt( int32_t skt, int32_t level, int32_t optname, const char* optval, int32_t optlen );

/* ���״̬,timeout��λms, timeout=-1��ʾ���޵ȴ� */
int32_t os_socket_select( int32_t maxfd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, int32_t timeout );
ssize_t socket_writen(int fd, const void *vptr, size_t n);
/* �������� */
int32_t os_socket_send( int32_t skt, void *buf, int32_t size, int32_t timeout );

/* �������� */
int32_t os_socket_recv( int32_t skt, void *buf, int32_t size , int32_t timeout);

/* ����ಥ�� */
int32_t os_socket_join( int32_t skt, char* strip );

/* �˳��鲥�� */
int32_t os_socket_drop( int32_t skt, char* strip );

/* �������� */
int32_t os_socket_sendto( int32_t skt, char* strip, uint16_t port, void *buf, int32_t size, int32_t timeout );

/* �������� */
int32_t os_socket_recvfrom( int32_t skt, char* strip, size_t *port, void *buf, int32_t size, int32_t timeout );
int32_t DS_Connect(int32_t iSocket, char* pcIPAddr, uint16_t iPort, int32_t iNsec);

#ifdef __cplusplus
}
#endif

#endif /* __OSSOCKET_H__ */
