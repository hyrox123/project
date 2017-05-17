#ifndef __MS_MDU_STREAM_POOL_H__
#define __MS_MDU_STREAM_POOL_H__

#include "ms_mdu_stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

void* mdu_stream_pool_create( int maxnum );

void  mdu_stream_pool_destroy( void* pool, int timeout );

void* mdu_stream_pool_find( void* pool, LPNET_MDU_START_MEDIA lp_media, int timeout  );

void* mdu_stream_pool_open( void* pool, LPNET_MDU_START_MEDIA lp_media, int timeout);

void  mdu_stream_pool_try_close( void* pool, void* stream, int timeout );

void  mdu_stream_pool_close( void* pool, void* stream, int timeout );

void  mdu_stream_pool_clear( void* pool, int timeout );

void  mdu_stream_pool_print( void* pool );

void* mdu_stream_pool_find( void* pool, LPNET_MDU_START_MEDIA lp_media, int timeout);

void mdu_stream_pool_try_close( void* pool, void* stream, int timeout );
#ifdef __cplusplus
}
#endif

#endif/*__MS_MDU_STREAM_POOL_H__*/
