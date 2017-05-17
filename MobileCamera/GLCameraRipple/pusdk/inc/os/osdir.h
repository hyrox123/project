#ifndef __OSDIR_H__
#define __OSDIR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define DFTYPE_NULL		0//��Ч
#define DFTYPE_FILE		1//��ʾ�ļ�
#define DFTYPE_DIR		2//��ʾĿ¼
#define DFTYPE_LINK		3//����


/* ����Ŀ¼: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_dir_create( const char *root, const char *dir );

/* ɾ��Ŀ¼������������Ŀ¼���ļ�: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_dir_delete( const char *root, const char *dir );

/* Ŀ¼�Ƿ����: ����-1=�����ڣ�0=���� */
int32_t os_dir_exist( const char *root, const char *dir );

/* ��һ��·�� */
void *os_dir_open( const char *root, const char *dir, const char *filter );

/* �ر�һ��·�� */
int32_t os_dir_close( void* handle );

/* ȡ��Ŀ¼�µ�һ���ļ�����Ŀ¼: ����type=DFTYPE_FILE,... */
int32_t os_dir_get_first( void* handle, int32_t *type, char *file, int32_t size );

/* ȡ��Ŀ¼����һ���ļ�����Ŀ¼: ����type=DFTYPE_FILE,...  */
int32_t os_dir_get_next( void* handle, int32_t *type, char *file, int32_t size );

/* ȡ�ô���ʣ��ռ�(MB) */
unsigned long os_dir_get_free_space( const char *root, const char *dir );

/* �ü�·����ȥ��'.'��'..'����'\'��Ϊ'/'��ȥ����Ч�ַ� */
char* os_dir_trim_path( char *path );


#ifdef __cplusplus
}
#endif


#endif /* __OSDIR_H__ */
