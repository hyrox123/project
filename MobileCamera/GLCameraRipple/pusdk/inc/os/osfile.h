#ifndef __OSFILE_H__
#define __OSFILE_H__

#define OS_FILE_READ_ONLY		1
#define OS_FILE_WRITE_ONLY		2
#define OS_FILE_READ_WRITE		4

#ifdef __cplusplus
extern "C"
{
#endif


/* �������ļ�: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_file_rename( const char *srcfile, const char *dstfile );

/* �����ļ�: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_file_copy( const char *srcfile, const char *dstfile );

/* ɾ���ļ�: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_file_delete( const char *file );

/* �ļ��Ƿ����: ����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_file_exist( const char *file );

/* ȡ���ļ��ߴ��������ʱ��: size=bytes, time=utc-ms, ����0��ʾ�ɹ�,-1��ʾʧ��*/
int32_t os_file_get_info( const char *file, int64_t *size64, int64_t *time64 );

/* ���������ļ����һ���޸�ʱ��:size=bytes, time=utc-s,����0��ʾ�ɹ�,-1��ʾʧ�� */
int32_t os_file_set_info( const char* file, int64_t time64 );


/* ���ļ�, mode=OS_FILE_READ_ONLY */
void* os_file_open( const char *file, int32_t mode );

/* �ر��ļ� */
void os_file_close( void *fp );

/* ��λ�ļ�λ��: base=SEEK_SET, ... */
void os_file_seek( void *fp, int64_t pos, int base );

/* ȡ���ļ�λ�� */
int64_t os_file_tell( void *fp );

/* ���ļ�: ����0��ʾ�ļ�����,-1��ʾʧ��,>0��ʾ��ȡ�ĳߴ� */
int32_t os_file_read( void *fp, void *buf, int32_t size );

/* д�ļ� */
int32_t os_file_write( void *fp, void *buf, int32_t size );

/* �ļ��Ƿ����: ����1��ʾ����,0��ʾδ���� */
int32_t os_file_eof( void *fp );


#ifdef __cplusplus
}
#endif

#endif /* __OSFILE_H__ */
