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




	/* 创建会话，成功后处于暂停状态 */
	void* mdu_stream_create( LPNET_MDU_START_MEDIA lp_media);
		
	/* 销毁会话(如果试图失败则会强制销毁)，返回0表示成功,-1表示失败 */
	int32_t mdu_stream_destroy( void* stream, int32_t timeout );

	/* 比较会话参数，返回0表示成功,-1表示失败 */
	int32_t mdu_stream_compare( void* stream, LPNET_MDU_START_MEDIA lp_media);	
		
	/* 修改会话执行参数 */
	void mdu_stream_set_media( void* stream, LPNET_MDU_START_MEDIA lp_media);
	
	/* 取得会话执行参数 */
	void mdu_stream_get_media( void* stream, LPNET_MDU_START_MEDIA* lp_media);
	
	/* 暂停 */
	void mdu_stream_pause( void* stream );

	
	/* 继续 */
	void mdu_stream_resume( void* stream );
	
	/* 是否正在运行:1=是,0=否 */
	//int32_t mdu_stream_is_running( void* stream );
	
	/* 是否在用户执行函数中没有返回，1=yes,0=no */
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


