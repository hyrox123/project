#ifndef _MS_CMU_REG_H_
#define _MS_CMU_REG_H_

int cmu_register(int skt);

int cmu_keepalive(int skt);

int handle_reg_msg(unsigned short cmd_id, int skt, unsigned int cseq, char* buf, int len);

int cmu_upLoad_alarm(int skt,char *prt);

int prase_cmu_reg_msg(int skt, char* buf, int len);
int prase_cmu_kpl_msg(int skt, char* buf, int len, char * info);
int find_xml_time(const char **buf,char  *info);
#endif/*_MS_CMU_REG_H_*/


