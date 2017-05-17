#ifndef __MS_MDU_H__
#define __MS_MDU_H__



int ms_mdu_initial();

void ms_mdu_release();

int32_t thrd_sock_mdu_fun(uint32_t param);
int32_t thrd_reg_mdu_fun(uint32_t param);
int32_t thrd_msg_mdu_fun(uint32_t param);

int mdu_socket_create(char* mdu_ip, unsigned int mdu_port);

void register_to_mdu(int skt, char* ip, unsigned int port);
void keepalive_to_mdu(int skt, char* ip, unsigned int port);
int handle_mdu_msg(int skt, char* buf, int len);

#endif/*__MS_MDU_H__*/


