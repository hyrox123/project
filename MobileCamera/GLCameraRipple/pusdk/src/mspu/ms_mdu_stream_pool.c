#include "ms_mdu_stream_pool.h"
#include "ms_pu_glob.h"

#include "oslib.h"

typedef struct
{
	void** strms;
	int    maxnum;
	int    killnum;
	int    usenum;
	void*  mutex;
}stream_pool_t;

void* mdu_stream_pool_create( int maxnum )
{
	stream_pool_t *tp;
	int size;

	size = sizeof(stream_pool_t) + maxnum*sizeof(void*);
	tp = (stream_pool_t *)os_sys_malloc( size, __FILE__, __LINE__ );//[32]24
	if( tp )
	{
		memset( tp, 0, size );
		tp->maxnum = maxnum;
		tp->strms = (void**)(tp + 1);
		tp->mutex = os_mutex_create();//[]24
		if( tp->mutex == NULL )
		{
			os_sys_free( tp, __FILE__, __LINE__ );
			tp = NULL;
		}
	}
	return tp;
}

void mdu_stream_pool_destroy( void* pool, int timeout )
{
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;

	for( i=0; i<tp->maxnum; i++ )
	{
		if(tp->strms[i])
			mdu_stream_destroy( tp->strms[i], timeout );
	}

	if( tp->mutex )
		os_mutex_destroy( tp->mutex );

	os_sys_free( pool, __FILE__, __LINE__ );
    pool = NULL;
}

void* mdu_stream_pool_find( void* pool, LPNET_MDU_START_MEDIA lp_media, int timeout  )
{
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;
	
	void* stream = NULL;
	
	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
	{
        printf("5555555555555555555555555555555\n");
        return NULL;
    }
		
	
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->strms[i] == NULL )
			continue;

		if (mdu_stream_compare(tp->strms[i], lp_media) == 0)
		{
			stream = tp->strms[i];
			break;
		}
	}
	LOG ("[mdu_stream_pool_find] %d,%d\n",i,tp->maxnum);
    printf("[mdu_stream_pool_find] %d,%d\n",i,tp->maxnum);
	os_mutex_unlock( tp->mutex );

	 
	
	return stream;
}

void* mdu_stream_pool_open( void* pool, LPNET_MDU_START_MEDIA lp_media, int timeout)
{
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;
	void* stream = NULL;
	LPNET_MDU_START_MEDIA old_media = NULL;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
	{
        LOG("[mdu_stream_pool_open] mutex_lock failed\n");
        printf("[mdu_stream_pool_open] mutex_lock\n");
        return NULL;
    }
		
	
	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->strms[i] == NULL )
			continue;

		old_media = NULL;
		mdu_stream_get_media( tp->strms[i], &old_media);
		
		if( old_media == NULL && mdu_stream_is_busy( tp->strms[i] ) == 0 )
		{
			//LOG("\n============mdu_stream_pool_open_1===================\n");
			//LOG("\n============mdu_stream_pool_open_1_mediaType%d===================\n",lp_media->media.media_type);
			stream = tp->strms[i];
            //printf("[mdu_stream_pool_open] mdu set media\n");
			mdu_stream_set_media( stream, lp_media);

			mdu_stream_resume(stream);

			tp->usenum++;
			break;
		}
	}

	if( stream == NULL )
	{
		for( i=0; i<tp->maxnum; i++ )
		{
			if( tp->strms[i] == NULL )
			{
				//LOG("\n============mdu_stream_pool_open_2===================\n");
				stream = mdu_stream_create( lp_media );
				
				if(stream == NULL)
				{
                    LOG("create mdu_stream failed\n");
                    break;
                }
				mdu_stream_resume(stream);
				tp->strms[i] = stream;
				tp->usenum++;
				
				break;
			}
		}
	}

	if( stream == NULL )
	{
		for( i=0; i<tp->maxnum; i++ )
		{
			//LOG("\n============mdu_stream_pool_open_3===================\n");

			if( tp->strms[i] == NULL )
				continue;

			old_media = NULL;
			mdu_stream_get_media( tp->strms[i], &old_media );

			//if(old_media != NULL)
			if( old_media == NULL && mdu_stream_is_busy( tp->strms[i] ) == 0 )
			{
                LOG("[mdu_stream_pool_open][mdu_stream_destroy]\n");
				mdu_stream_destroy( tp->strms[i], timeout );
				tp->strms[i] = NULL;
				tp->killnum++;
				tp->usenum--;

				stream = mdu_stream_create( lp_media );
				if(stream == NULL)
					break;
				
				mdu_stream_resume(stream);
				tp->strms[i] = stream;
				tp->usenum++;
				break;
			}else
			{
				LOG("\n====old_media is NULL &&  stream_objct  busy don't  \n");
			}
		}
	}

	os_mutex_unlock( tp->mutex );
	return stream;
}

void mdu_stream_pool_try_close( void* pool, void* stream, int timeout )
{
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
	{
         LOG("[mdu_stream_pool_try_close] mutex_lock failed\n");
         printf("[mdu_stream_pool_try_close] mutex_lock failed\n");
		//return -1;
    }
       

	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->strms[i] == stream )
		{
			
			mdu_stream_set_media( stream, NULL);
            //printf("(((((((((((((((set media\n");
			
			mdu_stream_pause( stream );
			tp->usenum--;
			break;
		}
	}

	os_mutex_unlock( tp->mutex );

}



void mdu_stream_pool_close( void* pool, void* stream, int timeout )
{

	printf("\n======mdu_stream_pool_close=======\n");
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;
	unsigned int start, now;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
		return;

	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->strms[i] == stream )
		{
			mdu_stream_pause( stream );
			mdu_stream_set_media( stream, NULL );

			start = now = os_sys_get_time();
			while( mdu_stream_is_busy( stream ) )
			{
				os_mutex_unlock( tp->mutex );
				os_sys_usleep( 1000 );
				os_mutex_lock( tp->mutex, timeout );

				now = os_sys_get_time();
				if( start + timeout < now || now < start )
					break;
			}

			if( mdu_stream_is_busy( stream ) )
			{
				os_mutex_unlock( tp->mutex );
				mdu_stream_destroy( stream, timeout );
				os_mutex_lock( tp->mutex, timeout );

				tp->strms[i] = NULL;
				tp->killnum++;
			}
			break;
		}
	}

	os_mutex_unlock( tp->mutex );
}

void  mdu_stream_pool_clear( void* pool, int timeout )
{

	//LOG("\n======MDU_STREAM_POOL_CLEAR=======\n");
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i;
	unsigned int start, now;
	void* stream = NULL;

	if( os_mutex_lock( tp->mutex, timeout ) != 0 )
	{
        LOG("[mdu_stream_pool_clear] mutex_lock failed\n");
        return;
    }
		

	for( i=0; i<tp->maxnum; i++ )
	{
		stream = tp->strms[i];
		if( stream )
		{
			mdu_stream_pause( stream );
			mdu_stream_set_media( stream, NULL );
			tp->usenum--;

			start = now = os_sys_get_time();

			while( mdu_stream_is_busy( stream ) )
			{
				printf("\n=====mdu_stream_is_busy_1=======\n");
				os_mutex_unlock( tp->mutex );
				os_sys_usleep( 1000 );
				os_mutex_lock( tp->mutex, timeout );

				now = os_sys_get_time();
				if( start + timeout < now || now < start )
					break;
			}

			if( mdu_stream_is_busy( stream ) )
			{
				printf("\n========mdu_stream_is_busy_2============\n");
				os_mutex_unlock( tp->mutex );
				mdu_stream_destroy( stream, timeout );
				os_mutex_lock( tp->mutex, timeout );

				tp->strms[i] = NULL;
				tp->killnum++;
			}
		}
	}

	os_mutex_unlock( tp->mutex );
}

void mdu_stream_pool_print( void* pool )
{
	stream_pool_t *tp = (stream_pool_t *)pool;
	int i = 0, allocnum = 0, idlenum = 0, sicknum = 0;

	LPNET_MDU_START_MEDIA old_media = NULL;

	os_mutex_lock( tp->mutex, -1 );

	for( i=0; i<tp->maxnum; i++ )
	{
		if( tp->strms[i] )
		{
			old_media = NULL;
			mdu_stream_get_media( tp->strms[i], &old_media );

			if( old_media )
				allocnum++;
			else if( mdu_stream_is_busy( tp->strms[i] ) )
				sicknum++;
			else
				idlenum++;
		}
	}
	os_sys_print( NULL, 0, "streampool(%d): allocated=%d, idle=%d, sick=%d, killed=%d\n",
		tp->maxnum, allocnum, idlenum, sicknum, tp->killnum );

	os_mutex_unlock( tp->mutex );
}


