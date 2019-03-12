//
//  ipc.h
//  simpleblocksd
//
//  Created by David Harris on 02/15/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#ifndef simpleblocksd_ipc_h
#define simpleblocksd_ipc_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>



/* Structs and data types	*/
/*	Meet Up Page	~1k page*/
#define	MAX_PIDS		250
struct mup
{
	uint32_t	epoc;
	uint32_t	pids_count;
	pid_t		pids[MAX_PIDS];
};
typedef	struct mup		mup_t;


/*	Shared Memory Block	*/
struct shmb
{
	uint32_t	epoc;
	struct
	{
		uint32_t	type;
		uint32_t	size;
		char		hash[32];
//		char		pad[0];
	}head;
	caddr_t			data;
};
typedef	struct shmb		shmb_t;


/*	Shared Memory data type	*/
struct ipc_s
{
	sem_t		*mup_sem;
	uint32_t	mup_epoc;
	int			mup_sem_fd;
	mup_t		*mup;

	sem_t		*block_sem;
	uint32_t	block_epoc;
	int			block_sem_fd;
	shmb_t		*block;
};
typedef	struct ipc_s		ipc_t;


/*	Declarations	*/


/* Prototypes	*/
void ipc_init(ipc_t *ctx);
void ipc_destroy(ipc_t *ctx);

void ipc_get_work(ipc_t *ctx);




#ifdef __cplusplus
}
#endif

#endif
