//
//  storage.h
//  libpeers
//
//  Created by David Harris on 12/20/2018.
//  Copyright (c) 2018 MegaApps. All rights reserved.
//

#ifndef libpeers_storage_h
#define libpeers_storage_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

//#include <stdtypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "uuid4.h"


#define kPOOL_NAME_LEN		128


typedef	struct pool_s	pool_t;
struct pool_s
{
	uuid_t		uuid;
	char*		name;
	uuid_t		parent_uuid;

	uuid_t		root;	/* Pool's ROOT block */
};


typedef	struct block_s	block_t;
struct block_s
{
	block_t*		next;
};


struct stor_s
{
	int				status;
	uuid_t			uuid;
	char*			name;

	char*			dir;
	FILE*			dev;

	u_int32_t		p_count;
	pool_t**		pools;

	u_int32_t		b_count;
	block_t**		blocks;
};
typedef	struct stor_s	stor_t;

stor_t*	stor_init(const char* dir);

/**
 * Frees the memory ofa stor_t instance.
 *
 * @param s		stor_t* of the instance being freed.
 *				.
 * @see stor_free
 */
void stor_free(stor_t *s);

stor_t*	stor_load(const char* dir);


void stor_disk_uuid(stor_t *s,uuid_t uuid);


/****************************************************************/
/*	Pool functions												*/
/****************************************************************/
uuid_t stor_pool_create(stor_t *s,char* name);
void stor_pool_destroy(stor_t *s,uuid_t uuid);

u_int32_t stor_pool_count(stor_t *s);
uuid_t stor_pool_uuid_for_name(stor_t *s,char* name);
void stor_pool_list_copy(stor_t *s,uuid_t** list,u_int32_t *len);
//void stor_pool_uuid(stor_t *s,uuid_t uuid);




/****************************************************************/
/*	Block functions												*/
/****************************************************************/
uuid_t stor_block_add(stor_t *s,uuid_t p,char* data,long len);
uuid_t stor_block_remove(stor_t *s,uuid_t p,char* data,long len);



#ifdef __cplusplus
}
#endif

#endif
