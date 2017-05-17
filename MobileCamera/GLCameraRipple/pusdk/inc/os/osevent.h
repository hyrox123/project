#ifndef __OSEVENT_H__
#define __OSEVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif


/* �����ź��� */
void* os_event_create(char* name);

/* �����ź��� */
void os_event_destroy(char *name);

/* �ȴ��ź���, time(ms)=-1��ʾ���޵ȴ� */
int32_t os_event_wait( void* event, int32_t timeout );

/* �����ź��� */
void os_event_give( void* event );


#ifdef __cplusplus
}
#endif

#endif /* __OSEVENT_H__ */
