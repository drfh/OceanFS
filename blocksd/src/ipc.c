#include "ipc.h"


char	*_g_shared_memory_buff;
int		_g_shmid;


void sharedmem_init(void)
{
	char	c;
	key_t	key;
	char	*s;

	fprintf(stderr,"size of key_t: %d\n",sizeof(key_t));
	key='Bs'; /* Object storage*/
	/*
	 * Create the segment.
	 */
	if((_g_shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}

	/*
	 * Now we attach the segment to our data space.
	 */
	if((_g_shared_memory_buff = shmat(_g_shmid, NULL, 0)) == (char *) -1)
	{
		perror("shmat");
		exit(1);
	}

	/*
	 * Now put some things into the memory for the
	 * other process to read.
	 */
	s = shm;

	for(c = 'a'; c <= 'z'; c++)
		*s++ = c;
	*s = NULL;

	/*
	 * Finally, we wait until the other process
	 * changes the first character of our memory
	 * to '*', indicating that it has read what
	 * we put there.
	 */
	while (*shm != '*')
		sleep(1);

	exit(0);
}


void sharedmem_destroy(void)
{
	if(shmdt(_g_shared_memory_buff))
	{
		fprintf(stderr,"shmdt returned non-zero.\n",);
	}
	else
	{
		shmctl(_g_shmid,IPC_RMID,NULL);
	}
}
