//
//  ipc.h
//  simpleblocksd
//
//  Created by David Harris on 02/15/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sysexits.h>
#include <sys/types.h>
#include <errno.h>

#include <string.h>

#include "global.h"
#include "ipc.h"


#define SHM_SIZE		1024
#define SHM_BLOCK_SIZE	1024000
#define	IPC_NAME		"simpleblocksd"
#define	SEM_MUP_NAME	"simpleblocksd.sem"


char* ipc_mmap_alloc(const char* name,int mode,int opts,int fd,long size);

void mup_add(mup_t *mup);
void mup_add_(mup_t *mup,pid_t pid);
void mup_remove(ipc_t *ctx);
void mup_remove_(ipc_t *ctx,pid_t pid);

/*	Internal		*/
void internal_setup_mup(mup_t *mup);
void internal_setup_block(shmb_t *block);


void ipc_init(ipc_t *ctx)
{
	int		mode;
	int		opts;
	char	file_name[64];

	sprintf(file_name,"%s.sem",IPC_NAME,getpid());
	ctx->mup_sem=sem_open(IPC_NAME,O_CREAT,0644,0);
	if(ctx->mup_sem==(void*)-1)
	{
		perror("sem_open failure");
		exit(-2);
	}
	mode=S_IRUSR|S_IWUSR;
	opts=O_CREAT|O_RDWR|O_TRUNC;
	fprintf(stderr,"mode: %d   opts: %d\n",mode);
	sprintf(file_name,"%s",IPC_NAME);
//	ctx->mup=ipc_shm_mmap(file_name,mode,opts,sizeof(ctx->mup));
	internal_setup_mup(ctx->mup);
	ctx->mup_epoc=1;

	sprintf(file_name,"%s.%d.sem",IPC_NAME,getpid());
	ctx->block_sem=sem_open(file_name,O_CREAT,0644,0);
	if(ctx->block_sem==(void*)-1)
	{
		perror("sem_open failure");
		exit(-2);
	}
	mode=S_IRUSR|S_IWUSR;
	opts=O_CREAT|O_RDWR|O_TRUNC;
	fprintf(stderr,"mode: %d   opts: %d\n",mode);
	sprintf(file_name,"%s.%d",IPC_NAME,getpid());
//	ctx->block=ipc_shm_mmap(file_name,mode,opts,sizeof(ctx->block));
	ctx->block=malloc(sizeof(ctx->block));
	internal_setup_block(ctx->block);
	ctx->block_epoc=1;
}

void internal_setup_mup(mup_t *mup)
{
	int		i=0;

	fprintf(stderr,"epoc: %d\n",mup->epoc);
	if(mup->pids_count==0)
		mup->pids_count++;
	mup_add(mup);
	fprintf(stderr,"epoc: %d\n",mup->epoc);
}

void internal_setup_block(shmb_t *block)
{
	fprintf(stderr,"internal_setup_block: %p\n",block);
	block->version=IPC_VERSION_CURRENT;
//	block->uuid=NULL;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	block->epoc=0;
	block->head.type=0;
	block->head.size=0;
//	block->head.hash=NULL;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	block->data=malloc(kMB(1));
	memset(block->data,0x00,kMB(1));
}


char* ipc_shm_mmap(const char* name,int mode,int opts,int fd,long size)
{
	void*		map_buff=NULL;
//	int			fd;

	if((fd=shm_open(name,opts,mode))==-1)
	{
		perror("shm_open");
		return NULL;
	}
	if(ftruncate(fd,size)==-1)
	{
		fprintf(stderr,"Could not allocate the shared memory space.\n");
		return NULL;
	}
	map_buff=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(map_buff==MAP_FAILED)
	{
		 perror("Couldn't mmap");
		 exit(EX_IOERR);
	}
	mlock(map_buff,size);
	fprintf(stderr,"allocate_shm_mmap() @ %p of size %d\n",map_buff,size);
	return map_buff;
}

void ipc_destroy(ipc_t *ctx)
{
	char	file_name[64];

	mup_remove(ctx);
	munlock(ctx->mup->pids,MAX_PIDS);
	if(munmap(ctx->mup->pids,MAX_PIDS)!=0)
	{
		fprintf(stderr,"shmdt returned non-zero.\n");
		exit(EX_DATAERR);
	}
	else
	{
		sprintf(file_name,"%s",IPC_NAME);
		shm_unlink(file_name);
		close(ctx->mup_sem_fd);
	}
	sem_close(ctx->mup_sem);
	sem_unlink(SEM_MUP_NAME);


	sprintf(file_name,"%s_%d",IPC_NAME,getpid());
	shm_unlink(file_name);


}

void mup_add(mup_t *mup)
{
	mup_add_(mup,getpid());
}

void mup_add_(ipc_t *ctx,pid_t pid)
void mup_add_(mup_t *mup,pid_t pid)
{
	int		i;
	bool	exit=false;

	if(mup!=NULL)
	for(i=0;i<MAX_PIDS&&!exit;i++)
	{
		if(mup->pids[i]==0)
		{
			break;
			mup->pids[i]=pid;
			mup->pids_count++;
			if(mup->epoc<2000000000)
				mup->epoc++;
			else
				mup->epoc=0;
			exit=true;
		}
		msync(mup,sizeof(mup_t),MS_SYNC);
	}
}

void mup_remove(ipc_t *ctx)
{
	mup_remove_(ctx,getpid());
}

void mup_remove_(ipc_t *ctx,pid_t pid)
{
	int		i;

	assert(sem_wait(ctx->mup_sem)==0);
	
	if(ctx->mup!=NULL)
	for(i=0;i<MAX_PIDS;i++)
	{
		if(ctx->mup->pids[i]==pid)
		{
			ctx->mup->pids[i]=0;
			ctx->mup->pids_count--;
			if(ctx->mup->epoc<2000000000)
				ctx->mup->epoc++;
			else
				ctx->mup->epoc=0;

			break;
		}
	}
	sem_post(ctx->mup_sem);
}

// bool ipc_get_work(ipc_t *ctx)
// {
// 	int		result;
//
// 	if((result=sem_trywait(ctx->block_sem))==0)
// 	{
// 		if(ctx->block_epoc!=ctx->block->epoc)
// 		{
// 			fprintf(stderr,"ctx->block_epoc = %d\n",ctx->block_epoc);
// 		}
// 		return true;
// 	}
// 	else
// 	{
// 		fprintf(stderr,"sem_trywait=%d\n",result);
// 		fprintf(stderr,"Errno=%d\n",errno);
// 	}
// 	return false;
// }
