#ifndef __OSFILE_H__
#define __OSFILE_H__

#define OS_FILE_READ_ONLY		1
#define OS_FILE_WRITE_ONLY		2
#define OS_FILE_READ_WRITE		4

#ifdef __cplusplus
extern "C"
{
#endif


/* 重命名文件: 返回0表示成功,-1表示失败 */
int32_t os_file_rename( const char *srcfile, const char *dstfile );

/* 复制文件: 返回0表示成功,-1表示失败 */
int32_t os_file_copy( const char *srcfile, const char *dstfile );

/* 删除文件: 返回0表示成功,-1表示失败 */
int32_t os_file_delete( const char *file );

/* 文件是否存在: 返回0表示成功,-1表示失败 */
int32_t os_file_exist( const char *file );

/* 取得文件尺寸和最后更新时间: size=bytes, time=utc-ms, 返回0表示成功,-1表示失败*/
int32_t os_file_get_info( const char *file, int64_t *size64, int64_t *time64 );

/* 重新设置文件最后一次修改时间:size=bytes, time=utc-s,返回0表示成功,-1表示失败 */
int32_t os_file_set_info( const char* file, int64_t time64 );


/* 打开文件, mode=OS_FILE_READ_ONLY */
void* os_file_open( const char *file, int32_t mode );

/* 关闭文件 */
void os_file_close( void *fp );

/* 定位文件位置: base=SEEK_SET, ... */
void os_file_seek( void *fp, int64_t pos, int base );

/* 取得文件位置 */
int64_t os_file_tell( void *fp );

/* 读文件: 返回0表示文件结束,-1表示失败,>0表示读取的尺寸 */
int32_t os_file_read( void *fp, void *buf, int32_t size );

/* 写文件 */
int32_t os_file_write( void *fp, void *buf, int32_t size );

/* 文件是否结束: 返回1表示结束,0表示未结束 */
int32_t os_file_eof( void *fp );


#ifdef __cplusplus
}
#endif

#endif /* __OSFILE_H__ */
