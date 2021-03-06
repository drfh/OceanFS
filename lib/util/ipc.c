//
//  ipc.c
//  util
//
//  Created by David Harris on 02/15/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>

#include <sysexits.h>
#include <sys/mman.h>

#include "global.h"
#include "ipc.h"


/** Defines **/
#define SHM_BLOCK_SIZE	1024000


/** Protoypes **/
char* ipc_mmap_alloc(const char* name,int mode,int opts,int *fd,long size);

void mup_add(mup_t *mup);
void mup_add_(mup_t *mup,pid_t pid);
void mup_remove(ipc_t *ctx);
void mup_remove_(ipc_t *ctx,pid_t pid);

/*	Internal		*/
void internal_setup_mup(mup_t *mup);
void internal_setup_block(shmb_t *block);


void ipc_init(ipc_t *ctx,const char* ipc_name)
{
	int		mode;
	int		opts;
	char	file_name[64];
	int		str_len;

	bzero(file_name,64);
	str_len=strlen(ipc_name)+1;
	assert(str_len<250);
	ctx->ipc_name=malloc(str_len);
	strcpy(ctx->ipc_name,ipc_name);
	//	Setup MUP shared memory
	sprintf(file_name,"%s",ipc_name);
	ctx->mup_sem=sem_open(file_name,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP,0);
	assert(ctx->mup_sem!=SEM_FAILED);
	sem_post(ctx->mup_sem);

	mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP;
	opts=O_CREAT|O_RDWR;
	ctx->mup=(mup_t*)ipc_mmap_alloc(file_name,mode,opts,&ctx->mup_shm_fd,sizeof(mup_t));
	sem_wait(ctx->mup_sem);
	internal_setup_mup(ctx->mup);
	sem_post(ctx->mup_sem);
	ctx->mup_epoc=1;

	//	Setup BLOCK shared memory
	sprintf(file_name,"%s.%d",ipc_name,getpid());
	ctx->block_sem=sem_open(file_name,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,1);
	assert(ctx->block_sem!=SEM_FAILED);
	sem_post(ctx->block_sem);

//	mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP;
//	opts=O_CREAT|O_RDWR|O_TRUNC;
	sprintf(file_name,"%s.shm.%d",ipc_name,getpid());
	ctx->block=(shmb_t*)ipc_mmap_alloc(file_name,mode,opts,&ctx->block_shm_fd,sizeof(struct shmb)+kMB(1));
	internal_setup_block(ctx->block);
	ctx->block_epoc=1;
}

void internal_setup_mup(mup_t *mup)
{
	if(mup->version==0)
	{
		mup->version=1;
		mup->mup_expanse=1;
		if(mup->epoc==0)
			mup->epoc=1;
	}
	mup_add(mup);
}

void internal_setup_block(shmb_t *block)
{
	fprintf(stderr,"internal_setup_block: %p\n",(void*)block);
	block->version=IPC_VERSION_CURRENT;
//	block->uuid=NULL;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	block->epoc=0;
	block->head.type=0;
	block->head.size=0;
//	block->head.hash=NULL;//{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	block->data=malloc(kMB(1));
	memset(block->data,0x00,kMB(1));
}

char* ipc_mmap_alloc(const char* name,int mode,int opts,int *fd,long size)
{
	void*		map_buff=NULL;

	if((*fd=shm_open(name,opts,mode))==-1)
	{
		perror("shm_open");
		return NULL;
	}
	if(ftruncate(*fd,size)==-1)
	{
		fprintf(stderr,"Could not allocate the shared memory space.\n");
		return NULL;
	}
	map_buff=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,*fd,0);
	if(map_buff==MAP_FAILED)
	{
		 perror("Couldn't mmap");
		 exit(EX_IOERR);
	}
	mlock(map_buff,size);
//	fprintf(stderr,"ipc_shm_mmap(%s) @ %p of size %d\n",name,map_buff,size);
	return map_buff;
}

void ipc_destroy(ipc_t *ctx)
{
	char	file_name[64];

	mup_remove(ctx);
	munlock(ctx->mup,sizeof(mup_t));
	if(munmap(ctx->mup,sizeof(mup_t))!=0)
	{
		fprintf(stderr,"shmdt returned non-zero.\n");
		exit(EX_DATAERR);
	}
	else
	{
		sprintf(file_name,"%s",ctx->ipc_name);
	//	shm_unlink(file_name);
	//	close(ctx->mup_shm_fd);
	}
	sem_close(ctx->mup_sem);
	sprintf(file_name,"%s",ctx->ipc_name);
	sem_unlink(file_name);
//	shm_unlink(file_name);

	sprintf(file_name,"%s.%d",ctx->ipc_name,getpid());
	sem_unlink(file_name);
	sprintf(file_name,"%s.shm.%d",ctx->ipc_name,getpid());
	shm_unlink(file_name);
}

void mup_add(mup_t *mup)
{
	mup_add_(mup,getpid());
}

void mup_add_(mup_t *mup,pid_t pid)
{
	int		i;
	bool	exit=false;

	errno=0;
	if(mup!=NULL)
	for(i=0;i<MAX_PIDS&&!exit;i++)
	{
		if(mup->pids[i]!=0)
			if(kill(mup->pids[i],0)==-1&&errno==ESRCH)
				mup->pids[i]=0;
		if(mup->pids[i]==0)
		{
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
	mup_t	*mup=ctx->mup;

	assert(sem_wait(ctx->mup_sem)==0);
	if(mup!=NULL)
	{
		bool	exit=false;
		int		i;

		for(i=0;i<MAX_PIDS&&exit==false;i++)
		{
			if(mup->pids[i]==pid)
			{
				mup->pids[i]=0;
				mup->pids_count--;
				if(mup->epoc<2000000000)
					mup->epoc++;
				else
					mup->epoc=0;
				exit=true;
			}
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
