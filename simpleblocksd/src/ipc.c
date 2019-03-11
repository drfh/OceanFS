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


void internal_setup_mup(void);
void internal_setup_shmb(void);
void* allocate_shm_map(const char* name,int mode,int opts,int fd,long size);


void remove_from_mup(void);
void remove_from_mup_(pid_t pid);


struct shm_t		g_shm;
//struct region		*g_sh_rgn;
//struct ipc_group	*g_shm_group;

void ipc_init(void)
{
	int		mode;
	int		opts;
	char	file_name[64];

	g_shm.page_sem=sem_open(SEM_MUP_NAME,O_CREAT,0644,0);
	if(g_shm.page_sem==(void*)-1)
	{
		perror("sem_open failure");
		exit();
	}
	mode=S_IRUSR|S_IWUSR;
	opts=O_CREAT|O_RDWR|O_TRUNC;
	fprintf(stderr,"mode: %d   opts: %d\n",mode);
	sprintf(file_name,"%s",MUP_SHM_NAME);
	g_shm.page=allocate_shm_map(file_name,mode,opts,sizeof(g_shm));
	internal_setup_mup(g_shm.page);

}


void internal_setup_mup(void)
{
//	g_shm->page.;
}

void internal_setup_shmb(void)
{
	;
}


void ipc_destroy(void)
{
	remove_from_mup();
	munlock(g_shm.page->pids,MAX_PIDS);
	if(munmap(g_shm.page->pids,MAX_PIDS)!=0)
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

void mup_add(void)
{
	mup_add_(pid());
}

void mup_add_(void)
{
	int		i;

	if(g_shm.page!=NULL)
	for(i=0;i<MAX_PIDS;i++)
	{
		if(g_shm.page->pids[i]==0)
		{
			g_shm.page->pids[i]=pid();
			break;
		}
	}
}

void mup_remove(void)
{
	mup_remove_(pid());
}

void mup_remove_(pid_t pid)
{
	int		i;

	sem_wait(g_shm.page_sem);

	if(g_shm.page!=NULL)
	for(i=0;i<MAX_PIDS;i++)
	{
		if(g_shm.page->pids[i]==pid)
		{
			g_shm.page->pids[i]=0;
			break;
		}
	}
	sem_post(g_shm.page_sem);
}
