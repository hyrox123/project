#ifndef __OSMEM_H__
#define __OSMEM_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* �ڴ�й©���ص����� */
typedef void (*PMemCheckCallback)( char *buf, int32_t size, const char *file, int32_t line, unsigned long lParam );


/* �����ڴ����ģ�� */
void* os_mem_create( char *buf, int32_t size, int32_t align );
/* �����ڴ����ģ�� */
void os_mem_destroy( void* h );
/* ��� */
void os_mem_reset( void* h );
/* ����ڴ�ڵ㣬������ʷ���������Ĺ������ߴ� */
int32_t os_mem_check( void* h, PMemCheckCallback pProc, unsigned long lParam );

/* �����ڴ� */
void *os_mem_alloc( void* h, int32_t size, const char *file, int32_t line, int32_t *memsize );
/* �ͷ��ڴ� */
int32_t os_mem_free( void* h, void *p, const char *file, int32_t line );
/* �����ַ��� */
char *os_mem_strdup( void* h, const char *s, const char *file, int32_t line );
/* ���������ڴ� */
void *os_mem_realloc( void* h, void *p, int32_t size, const char *file, int32_t line );


#ifdef __cplusplus
}
#endif

#endif /* __OSMEM_H__ */
