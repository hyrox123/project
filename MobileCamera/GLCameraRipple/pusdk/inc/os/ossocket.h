#ifndef __OSSOCKET_H__
#define __OSSOCKET_H__


#include "stdafx.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 创建socket, 返回-1表示失败, type=SOCK_STREAM, ... , protocol=IPPROTO_TCP,... */
int32_t os_socket_create( int32_t af, int32_t type, int32_t protocol );

/* 销毁socket */
void os_socket_destroy( int32_t skt );

/* 连接远端 */
int32_t os_socket_connect( int32_t skt, char* strip, uint16_t port, int32_t timeout);

/* 绑定接收地址和端口 */
int32_t os_socket_bind( int32_t skt, char* strip, uint16_t port );

/* 开始监听, max为悬挂队列长度，建议设为5 */
int32_t os_socket_listen( int32_t skt, int32_t max );

/* 等待远程连接 */
int32_t os_socket_accept( int32_t skt, char* strip, uint16_t *port );

/* 输入输出控制 */
int32_t os_socket_ioctl( int32_t skt, int32_t type, uint32_t *val );

/* 设置参数 */
int32_t os_socket_opt( int32_t skt, int32_t level, int32_t optname, const char* optval, int32_t optlen );

/* 检测状态,timeout单位ms, timeout=-1表示无限等待 */
int32_t os_socket_select( int32_t maxfd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, int32_t timeout );
ssize_t socket_writen(int fd, const void *vptr, size_t n);
/* 发送数据 */
int32_t os_socket_send( int32_t skt, void *buf, int32_t size, int32_t timeout );

/* 接收数据 */
int32_t os_socket_recv( int32_t skt, void *buf, int32_t size , int32_t timeout);

/* 加入多播组 */
int32_t os_socket_join( int32_t skt, char* strip );

/* 退出组播组 */
int32_t os_socket_drop( int32_t skt, char* strip );

/* 发送数据 */
int32_t os_socket_sendto( int32_t skt, char* strip, uint16_t port, void *buf, int32_t size, int32_t timeout );

/* 接收数据 */
int32_t os_socket_recvfrom( int32_t skt, char* strip, size_t *port, void *buf, int32_t size, int32_t timeout );
int32_t DS_Connect(int32_t iSocket, char* pcIPAddr, uint16_t iPort, int32_t iNsec);

#ifdef __cplusplus
}
#endif

#endif /* __OSSOCKET_H__ */
