#ifndef __OSMEM_H__
#define __OSMEM_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* 内存泄漏检查回调函数 */
typedef void (*PMemCheckCallback)( char *buf, int32_t size, const char *file, int32_t line, unsigned long lParam );


/* 创建内存管理模块 */
void* os_mem_create( char *buf, int32_t size, int32_t align );
/* 销毁内存管理模块 */
void os_mem_destroy( void* h );
/* 清空 */
void os_mem_reset( void* h );
/* 检查内存节点，返回历史上曾经消耗过的最大尺寸 */
int32_t os_mem_check( void* h, PMemCheckCallback pProc, unsigned long lParam );

/* 分配内存 */
void *os_mem_alloc( void* h, int32_t size, const char *file, int32_t line, int32_t *memsize );
/* 释放内存 */
int32_t os_mem_free( void* h, void *p, const char *file, int32_t line );
/* 复制字符串 */
char *os_mem_strdup( void* h, const char *s, const char *file, int32_t line );
/* 重新申请内存 */
void *os_mem_realloc( void* h, void *p, int32_t size, const char *file, int32_t line );


#ifdef __cplusplus
}
#endif

#endif /* __OSMEM_H__ */
