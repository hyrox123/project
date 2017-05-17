#ifndef __OSDIR_H__
#define __OSDIR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define DFTYPE_NULL		0//无效
#define DFTYPE_FILE		1//表示文件
#define DFTYPE_DIR		2//表示目录
#define DFTYPE_LINK		3//连接


/* 创建目录: 返回0表示成功,-1表示失败 */
int32_t os_dir_create( const char *root, const char *dir );

/* 删除目录及旗下所有子目录和文件: 返回0表示成功,-1表示失败 */
int32_t os_dir_delete( const char *root, const char *dir );

/* 目录是否存在: 返回-1=不存在，0=存在 */
int32_t os_dir_exist( const char *root, const char *dir );

/* 打开一个路径 */
void *os_dir_open( const char *root, const char *dir, const char *filter );

/* 关闭一个路径 */
int32_t os_dir_close( void* handle );

/* 取得目录下第一个文件或子目录: 返回type=DFTYPE_FILE,... */
int32_t os_dir_get_first( void* handle, int32_t *type, char *file, int32_t size );

/* 取得目录下下一个文件或子目录: 返回type=DFTYPE_FILE,...  */
int32_t os_dir_get_next( void* handle, int32_t *type, char *file, int32_t size );

/* 取得磁盘剩余空间(MB) */
unsigned long os_dir_get_free_space( const char *root, const char *dir );

/* 裁减路径，去掉'.'和'..'，将'\'变为'/'，去掉无效字符 */
char* os_dir_trim_path( char *path );


#ifdef __cplusplus
}
#endif


#endif /* __OSDIR_H__ */
