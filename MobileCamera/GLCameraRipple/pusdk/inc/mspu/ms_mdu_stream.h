#ifndef __MS_MDU_STREAM_H__
#define __MS_MDU_STREAM_H__

#include "stdafx.h"
#include "ms_pu_param.h"
#include "osqueue_g.h"
#include <ms_pu_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif




	/* �����Ự���ɹ�������ͣ״̬ */
	void* mdu_stream_create( LPNET_MDU_START_MEDIA lp_media);
		
	/* ���ٻỰ(�����ͼʧ�����ǿ������)������0��ʾ�ɹ�,-1��ʾʧ�� */
	int32_t mdu_stream_destroy( void* stream, int32_t timeout );

	/* �ȽϻỰ����������0��ʾ�ɹ�,-1��ʾʧ�� */
	int32_t mdu_stream_compare( void* stream, LPNET_MDU_START_MEDIA lp_media);	
		
	/* �޸ĻỰִ�в��� */
	void mdu_stream_set_media( void* stream, LPNET_MDU_START_MEDIA lp_media);
	
	/* ȡ�ûỰִ�в��� */
	void mdu_stream_get_media( void* stream, LPNET_MDU_START_MEDIA* lp_media);
	
	/* ��ͣ */
	void mdu_stream_pause( void* stream );

	
	/* ���� */
	void mdu_stream_resume( void* stream );
	
	/* �Ƿ���������:1=��,0=�� */
	//int32_t mdu_stream_is_running( void* stream );
	
	/* �Ƿ����û�ִ�к�����û�з��أ�1=yes,0=no */
	int32_t mdu_stream_is_busy( void* stream );
	


//	int32_t mdu_stream_receive_data( void* stream, char* buffer, size_t * size );


	/*typedef struct MyPacketet{
		int		MyPacketSize ;
		char	*MyPacket;
		int		MyPacketNum;
	};
	*/
#ifdef __cplusplus
}
#endif

#endif/*__MS_MDU_STREAM_H__*/


