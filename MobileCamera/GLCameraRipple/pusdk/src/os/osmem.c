#include "stdafx.h"
#include "osmem.h"


/* 整数对齐 */
#define ALIGN_SIZE(size)	((size+align) & ~align)
/* 指针((char*)p+size)对齐 */
#define ALIGN_PTR(p,size)	((size+align+(int32_t)p) & ~align)


/* 内存节点 */
typedef struct _MemNode  
{
#ifdef _MEM_DEBUG
	char file[16];
	int32_t line;
#endif
	int32_t size;
	struct _MemNode *next;
}SMemNode;

typedef struct
{
	/* 工作缓冲区 */
	char *buf;
	/* 工作缓冲区大小 */
	int32_t size;
	/* 对齐字节数 */
	int32_t align;

	/* 分配链表 */
	SMemNode *list;
	/* 最前的空闲块结点 */
	SMemNode *first;
	/* tail是最后的空闲块结点 */
	SMemNode *tail;

	/* 历史上最大占用内存数 */
	int32_t history_max_size;
}SMemHeader;

/*
static const char *os_mem_get_file_name( const char *path )
{
	int32_t n = 0;

	while( path[n] != 0 )
		n++;
	n--;
	while( 0 < n )
	{
		if( path[n] == '/' || path[n] == '\\' )
			return path+n+1;
		n--;
	}
	return path;
}
*/


/* 创建内存管理模块 */
void* os_mem_create( char *buf, int32_t size, int32_t align )
{
#if 1 //统一使用16字节对齐
	SMemHeader *xm = (SMemHeader *)(((unsigned long)buf+15)&(~15));
	align = 15;
	memset( xm, 0, sizeof(SMemHeader) );
	xm->align = 16;
	xm->buf = (char *)ALIGN_PTR( xm, sizeof(SMemHeader) );
	xm->size = buf + size - xm->buf;
#else
	SMemHeader *xm = (SMemHeader *)(((align-1+(int32_t)buf)/align)*align);
	
	memset( xm, 0, sizeof(SMemHeader) );
	xm->align = align;
	xm->buf = (char *)ALIGN_PTR( xm, sizeof(SMemHeader) );
	xm->size = buf + size - xm->buf;
#endif
	return xm;
}

/* 销毁内存管理模块 */
void os_mem_destroy( void* h )
{
}

/* 清空 */
void os_mem_reset( void* h )
{
	SMemHeader *xm = (SMemHeader *)h;

	xm->list = NULL;
	xm->first = NULL;
	xm->tail = NULL;
	xm->history_max_size = 0;
}

/* 检查内存节点 */
int32_t os_mem_check( void* h, PMemCheckCallback pProc, unsigned long lParam )
{
	SMemHeader *xm = (SMemHeader *)h;
	SMemNode *node = xm->list;
	int32_t align = xm->align-1;
	char *p;
	while( node && pProc )
	{
		p = (char*)ALIGN_PTR(node,sizeof(SMemNode));

#ifdef _MEM_DEBUG
		pProc( p, node->size, node->file, node->line, lParam );
#else
		pProc( p, node->size, "", -1, lParam );
#endif

		node = node->next;
	}
	return xm->history_max_size;
}

/* 找到内存p在链表中的位置 */
static SMemNode* find(void* h, void *p)
{
	SMemHeader *xm = (SMemHeader *)h;
	SMemNode *node = xm->list;
	int32_t align = xm->align-1;
	
	while( node )
	{
		if( p == (char*)ALIGN_PTR(node,sizeof(SMemNode)) )
			break;
		node = node->next;
	}
	return node;
}

/* 扫描链表看是否有free块的内存符合申请的要求; */
static void* malloc_old( void* h, int32_t size ,const char *file, int32_t line)
{
	SMemHeader *xm = (SMemHeader *)h;
	SMemNode *node, *new_node;
	int32_t align = xm->align-1;
	char *end;
	int32_t len;
	int32_t firstlen;

	/* 检查头和buf之间的空隙; */
	if( xm->first==NULL )
	{
		if( xm->list == NULL )
			return NULL;

		node = xm->list;
		len = ((int32_t)node) - ALIGN_PTR(xm->buf,sizeof(SMemNode));
		if( size <= len )
		{
			/* 填充NODE */
			new_node = (SMemNode *)xm->buf;
			memset( new_node, 0, sizeof(SMemNode) );
#ifdef _MEM_DEBUG
			if( file )
			{
				strncpy( new_node->file, os_mem_get_file_name(file), sizeof(new_node->file)-1 );
				new_node->line = line;
			}
#endif
			new_node->size = ALIGN_SIZE( size );
			
			/* 插入到链表头; */
			xm->list = new_node;
			new_node->next = node;
			xm->first = new_node;

			return (char*)ALIGN_PTR(new_node, sizeof(SMemNode)); 
		}
		firstlen = len;
	}
	else
	{
		node = xm->first;
		firstlen = 0;
	}
	
	/* 检查相临两块之间的间隙是否大于下一块的长度; */
	while( node->next )
	{
		end = (char*)ALIGN_PTR(node,sizeof(SMemNode)) + ALIGN_SIZE(node->size);
		len = ((char*)node->next) - (char*)ALIGN_PTR(end, sizeof(SMemNode));
		
		/* 如果第一空闲块太小，不够最低分配字节数，就修改这个指针 */
		if( firstlen <= (int32_t)align )
		{
			xm->first = node;
			firstlen = len;
		}
		
		/* 找到符合要求的块; */
		if( size <= len )
		{
			/* 填充NODE */
			new_node = (SMemNode *)end;
			memset( new_node, 0, sizeof(SMemNode) );
#ifdef _MEM_DEBUG
			if( file )
			{
				strncpy( new_node->file, os_mem_get_file_name(file), sizeof(new_node->file)-1 );
				new_node->line = line;
			}
#endif
			new_node->size = ALIGN_SIZE( size );
			
			/* 插入到链表; */
			new_node->next = node->next;
			node->next = new_node;
			
			return (char*)ALIGN_PTR(new_node, sizeof(SMemNode)); 
		}
		node=node->next;
	}
	return NULL;
}

/* 分配内存 */
void *os_mem_alloc( void* h, int32_t size, const char *file, int32_t line, int32_t *memsize )
{
	SMemHeader *xm = (SMemHeader*)h;
	SMemNode *tail = NULL;
	SMemNode *node;
	int32_t align = xm->align-1;
	char *p;

	size += (ALIGN_SIZE(sizeof(SMemNode)) - sizeof(SMemNode));

	/* 从free块分配 */
	p = (char *)malloc_old( h, size, file, line );
	if( p )
	{
		if( xm->history_max_size < (p-xm->buf) + size )
			xm->history_max_size = (p-xm->buf) + size;
		if( memsize )
			*memsize = sizeof(SMemNode) + ALIGN_SIZE( size );
		return p;
	}
	/* 第一次申请 */
	if( xm->list == NULL )
		node = (SMemNode *)xm->buf;
	else
	{
		tail = xm->tail;
		
		p = (char*)ALIGN_PTR( tail, sizeof(SMemNode) );	/* 最后那个结点的buf */
		node = (SMemNode *)ALIGN_PTR( p, tail->size );	/* 最后那个结点的buf的尾，可以作为下一结点地址 */
	}
	p = (char*)ALIGN_PTR( node, sizeof(SMemNode) );	/* 获取结点的buf */

	/* 检查是否有足够的内存 */
	//if( xm->buf+xm->size < p+size )
	if((int32_t)(xm->buf+xm->size-p) <= size )//size 可能较大
	{
		if( memsize )
			*memsize = 0;
		return NULL;
	}
	/* 填充node */
	memset( node, 0, sizeof(SMemNode) );
	node->size = ALIGN_SIZE( size );
#ifdef _MEM_DEBUG
		if( file )
		{
			strncpy( node->file, os_mem_get_file_name(file), sizeof(node->file)-1 );
			node->line = line;
		}
#endif
	
	/* 把当前节点挂到链表尾部 */
	if( tail == NULL )
		xm->list = node;
	else
		tail->next = node;

	if( xm->history_max_size < (p-xm->buf) + size )
		xm->history_max_size = (p-xm->buf) + size;
	
	xm->tail = node;

	if( memsize )
		*memsize = sizeof(SMemNode) + ALIGN_SIZE( size );

	return p;
}

/* 重新申请内存 */
void *os_mem_realloc( void* h, void *p, int32_t size, const char *file, int32_t line )
{
	void *buf = NULL;
	SMemHeader *xm = (SMemHeader*)h;
	SMemNode* node = NULL;
	int32_t align = xm->align-1;
	
	/* 定位 */
	if( p )
	{
		node = find( h, p );
		if( node &&
			(size<=node->size ||
			(int32_t)ALIGN_PTR(node,sizeof(SMemNode))+size <= 
			(int32_t)( node->next ? node->next : (SMemNode*)(xm->buf + xm->size)) ) )
		{
			node->size = ALIGN_SIZE( size );
			return p;
		}
	}
	
	/* 内存不够,重新分配; */
	buf = os_mem_alloc( h, size, file, line, NULL );
	if( p )
	{
		/* 拷贝内容; */
		memcpy( buf, p, node ? node->size:size );
		/* 释放原来的内存 */
		os_mem_free( h, p, file, line );
	}
	
	return buf;
}

/* 复制字符串 */
char *os_mem_strdup( void* h, const char *s, const char *file, int32_t line )
{
	char *p = (char*)os_mem_alloc( h, strlen(s)+1, file, line, NULL  );
	if( p )
		memcpy(p,s,strlen(s)+1);
	return p;
}



/* 释放内存 */
int32_t os_mem_free( void* h, void *p, const char *file, int32_t line )
{
	SMemHeader *xm = (SMemHeader *)h;
	SMemNode *node;
	SMemNode *last = NULL;
	int32_t align = xm->align-1;
	int32_t size = 0;

	/* 寻找node */
	for( node=xm->list; node; node=node->next )
	{
		if( p == (void*)ALIGN_PTR(node,sizeof(SMemNode)) )
			break;
		last = node;
	}

	if( node )
	{
		size = sizeof(SMemNode) + node->size;

		/* 释放的为头节点 */
		if( node == xm->list )
			xm->list = node->next;
		else
			last->next = node->next;

		if( xm->first >= node )
			xm->first = last;
		if( xm->tail == node )
			xm->tail = last;
	}

	return size;
}

