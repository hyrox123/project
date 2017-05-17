#ifndef _MS_CMU_h_
#define _MS_CMU_h_


/*
�������

1. �ŵ� TCP CLIENT 
2. ע��ע��
3. ά������
4. ��ȡ/�����豸����
5. �����豸�澯��Ϣ
6. PTZ����
7. ͸��ͨ������
8. ¼���ѯ
9. ��־��ѯ

8. ȡ�ö�Ӧ��MDU��Ϣ
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


