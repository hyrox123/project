#ifndef _MS_CMU_h_
#define _MS_CMU_h_


/*
管理服务

1. 信道 TCP CLIENT 
2. 注册注销
3. 维护心跳
4. 获取/设置设备参数
5. 接收设备告警信息
6. PTZ控制
7. 透明通道传输
8. 录像查询
9. 日志查询

8. 取得对应的MDU信息
*/
#define MINGSOFT_LIB  1

int ms_cmu_initial(char* cmu_ip, unsigned int cmu_port);

int ms_cmu_release();

int32_t thrd_sock_cmu_fun(uint32_t  param);
int32_t thrd_reg_cmu_fun(uint32_t param);
int32_t thrd_msg_cmu_fun(uint32_t param);
int32_t thrd_ptzmsg_cmu_fun(uint32_t param);
int32_t thrd_alarm_fun(uint32_t param);

int cmu_socket_create(char* cmu_ip, unsigned int cmu_port);

int get_msg_name(char* xml, char* msg_name);
int handle_cmu_msg(int skt, unsigned int type, char* buf, int len);

#endif/*_MS_CMU_h_*/


