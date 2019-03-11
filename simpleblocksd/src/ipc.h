//
//  ipc.h
//  storage
//
//  Created by David Harris on 02/15/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#ifndef storage_ipc_h
#define storage_ipc_h

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
struct shm_s
{
	sem_t	page_sem;
	int		sem_fd;
	mup_t	*page;

	sem_t	block_sem;
	shmb_t	*block;
};
typedef	struct shm_s		shm_t;


/*	Declarations	*/
extern struct shm_t		g_shm;


/* Prototypes	*/
void ipc_init(void);
void ipc_destroy(void);

void ipc_get_work(void);




#ifdef __cplusplus
}
#endif

#endif
