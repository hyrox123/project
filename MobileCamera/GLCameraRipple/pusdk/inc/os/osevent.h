#ifndef __OSEVENT_H__
#define __OSEVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* 创建信号量 */
void* os_event_create(char* name);

/* 销毁信号量 */
void os_event_destroy(char *name);

/* 等待信号量, time(ms)=-1表示无限等待 */
int32_t os_event_wait( void* event, int32_t timeout );

/* 点亮信号量 */
void os_event_give( void* event );


#ifdef __cplusplus
}
#endif

#endif /* __OSEVENT_H__ */
