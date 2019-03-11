#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sysexits.h>
#include <sys/types.h>

#include "ipc.h"


#define SHM_SIZE		1024
#define SHM_BLOCK_SIZE	1024000
#define	MUP_SHM_NAME	"simpleblocksd"
#define	SEM_MUP_NAME	"simpleblocksd.sem"


void* ipc_mmap_alloc(const char* name,int mode,int opts,int fd,long size);

void mup_add(ipc_t *ctx);
void mup_add_(ipc_t *ctx,pid_t pid);
void remove_from_mup(ipc_t *ctx);
void remove_from_mup_(ipc_t *ctx,pid_t pid);


void ipc_init(ipc_t *ctx)
{
	int		mode;
	int		opts;
	char	file_name[64];

	ctx->mup_sem=sem_open(SEM_MUP_NAME,O_CREAT,0644,0);
	if(ctx->mup_sem==(void*)-1)
	{
		perror("sem_open failure");
		exit();
	}
	mode=S_IRUSR|S_IWUSR;
	opts=O_CREAT|O_RDWR|O_TRUNC;
	fprintf(stderr,"mode: %d   opts: %d\n",mode);
	sprintf(file_name,"%s",MUP_SHM_NAME);
	ctx->mup=allocate_shm_map(file_name,mode,opts,sizeof(ctx));
	internal_setup_mup(ctx->mup);
}

void* ipc_shm_mmap(const char* name,int mode,int opts,int fd,long size)
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
	fprintf(stderr,"allocate_shm_mmap() @ %p of size %d\n",record,size);
	return map_buff;
}

void ipc_destroy(ipc_t *ctx)
{
	remove_from_mup();
	munlock(ctx->mup->pids,MAX_PIDS);
	if(munmap(ctx->mup->pids,MAX_PIDS)!=0)
	{
		fprintf(stderr,"shmdt returned non-zero.\n");
		exit(EX_DATAERR);
	}
	else
	{
		char	file_name[64];

		sprintf(file_name,"%s",SHM_NAME);
		shm_unlink(file_name);
		close(sem_fd);
	}
	sem_close(page_sem);
	sem_unlink(SEM_MUP_NAME);


	sprintf(file_name,"%s_%d",SHM_NAME,pid());
	shm_unlink(file_name);


}

void mup_add(ipc_t *ctx)
{
	mup_add_(ctx,pid());
}

void mup_add_(ipc_t *ctx,pid_t pid)
{
	int		i;

	if(ctx->page!=NULL)
	for(i=0;i<MAX_PIDS;i++)
	{
		if(ctx->mup->pids[i]==0)
		{
			ctx->mup->pids[i]=pid();
			break;
		}
	}
}

void mup_remove(ipc_t *ctx)
{
	mup_remove_(ctx,pid());
}

void mup_remove_(ipc_t *ctx,pid_t pid)
{
	int		i;

	sem_wait(ctx->mup_sem);

	if(ctx->mup!=NULL)
	for(i=0;i<MAX_PIDS;i++)
	{
		if(ctx->mup->pids[i]==pid)
		{
			ctx->mup->pids[i]=0;
			break;
		}
	}
	sem_post(ctx.mup_sem);
}

bool ipc_get_work(ipc_t *ctx)
{
	sem_wait(ctx->block_sem);

	if(ctx->block_epoc!=ctx->block->epoc)
	{
		;
	}
	sem_post(ctx->block_sem);
}
