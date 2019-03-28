//
//  main.c
//  simpleblocksd
//
//  Created by David Harris on 12/20/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <getopt.h>
#include <errno.h>
#include <sysexits.h>
#include <sched.h>
#include <unistd.h>

#include <signal.h>

#include "global.h"
#include "ipc.h"
#include "uuid4.h"
#include "storage.h"
#include "usage.h"

/** Defines **/
#define	kSLEEP_TICKS_DEFAULT	1000*10*1
#define	kSLEEP_TICKS_SLOW		10000*10*1
#define	kATUOEXIT_TIME			5

/** Prototypes **/
void init_globals(void);
void init_signals(void);
void signal_handler(int signum);

void alocate_file(const char* path,size_t size);
void parse_args(int argc,char const *argv[]);
void insert_file(stor_t *disk,char* data_pool,char* file_path);
stor_t* load_disk_path(char* path,bool create);


struct global{
	bool		done;
	bool		create;
	bool		caching;
	char*		disk_dir;
	int			verbose;
	uint64_t	disk_size;
};
struct	global		g;

void init_globals(void)
{
	g.done=false;
	g.create=false;
	g.disk_dir=NULL;
	g.verbose=1;
	g.disk_size=kMB(128);
}

void init_signals(void)
{
	signal(SIGINT,signal_handler);
	signal(SIGHUP,signal_handler);
	signal(SIGTERM,signal_handler);
}

void signal_handler(int signum)
{
	printf("\nCaught signal %d\n",signum);
//	||signum==SIGHUP||signum==SIGTERM
	if(signum==SIGINT)
	{
		if(g.done==true)
			exit(-1);
		g.done=true;
		fprintf(stderr,"enter ^C again to FORCE EXIT(-1)\n");
	}
}

int main(int argc,char const **argv)
{
	stor_t		*d;
	char*		name=malloc(128);
	int			return_val=0;
	ipc_t		ctx;

	fprintf(stderr,"-----------------\n");
	fprintf(stderr,"simpleblocksd - v0.1\n");
	fprintf(stderr,"-----------------\n");

	/*	Setup program globals	*/
	init_globals();
	fprintf(stderr,"Parse args...\n");
	parse_args(argc,(const char **)argv);


	/*	Create IPC communication context	*/
	fprintf(stderr,"Setup IPC...\n");
	ipc_init(&ctx);

	/*	Check to see if file exists	*/
	fprintf(stderr,"Create disk...\n");
	if(access(g.disk_dir,F_OK)==-1)
	{
		if(g.create==true)
		{
//			alocate_file(g.disk_dir,g.size);
		}
		else
		{
			fprintf(stderr,"File not found: %s\n",g.disk_dir);
			fprintf(stderr,"-c --create flag not set.\n");
			return_val=1;
		}
	}

	d=load_disk_path(g.disk_dir,g.create);
	if(d==NULL)
	{
		fprintf(stderr,"Can't open directory!\n");
		return_val=2;
	}

	shmb_t		*block=ctx.block;
	uint32_t	sleep_ticks=kSLEEP_TICKS_DEFAULT;
#ifdef kATUOEXIT_TIME
	time_t		start_t,end_t;
	time(&start_t);
#endif

	while(!g.done)
	{
		sched_yield();
		if(ipc_get_work(&ctx)==true)
#ifdef kATUOEXIT_TIME
		time(&end_t);
		g.done=!(difftime(end_t,start_t)<kATUOEXIT_TIME);
#endif
		{
			printf("Block epoc(%0.8x)-> Message type: %0.2x\n",ctx.block_epoc,block->head.type);
			if(block->head.type==0)
			{
//				fprintf(stderr,"Empty header\n");
				sleep_ticks=kSLEEP_TICKS_SLOW;
			}
			else if(block->head.type==100)
			{
				sleep_ticks=kSLEEP_TICKS_DEFAULT;
				printf("");
			}

//			ipc_work_close(&ctx);
		}
		sem_post(ctx.block_sem);
		usleep(sleep_ticks);
	}

	/*	Cleanup		*/
	fprintf(stderr,"\n");
	fprintf(stderr,"Cleanup...\n");
	fprintf(stderr,"Freeing cache...\n");

	fprintf(stderr,"Closing desk...\n");
//	stor_free(d);
	fprintf(stderr,"Closing IPC...\n");
	ipc_destroy(&ctx);
	fprintf(stderr,"Exiting...\n");
	free(name);
	return return_val;
}

void alocate_file(const char* path,size_t size)
{
	FILE		*f;
	char		*buff;
	size_t		i;

	if(size<kMB(16))
	{
		fprintf(stderr,"size is too small:  %ld\n",size);
	}
	f=fopen(path,"w+");
	if(f!=NULL)
	{
		buff=malloc(kMB(1));
		for(i=0;i<size;i+=kMB(1))
		{
			fwrite(buff,kMB(1),1,f);
		}
		fclose(f);
		free(buff);
	}
}


void parse_args(int argc,char const *argv[])
{
	int		c;
//	int		digit_optind = 0;
//	int		this_option_optind = optind ? optind : 1;
	int		option_index = 0;

	static struct option long_options[]={
		{"dir", required_argument, 0, 'd'},
//		{"size", required_argument, 0, 's'},
		{"caching", required_argument, 0, 'c'},
		{"verbose", no_argument, 0, 'v'},
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 0},
		{"usage", no_argument, 0, 0},
		{0, 0, 0, 0}
	};

	while((c=getopt_long(argc,(char**)argv,"d:s:cv",long_options,&option_index))!=-1)
	{
	//	printf("optarg: %s\n",optarg);
	//	printf("argv[optind]: %s\n",argv[optind]);
	//	printf("optind: %d\n",optind);
	//	printf("------\n");
		switch(c)
		{
			case 0:
			//	printf("option %s @ %d\n",long_options[option_index].name,option_index);
				if(strcmp(long_options[option_index].name,"name")==0)
				{
					printf("name set to: %s\n",optarg);
				}
				else if(strcmp(long_options[option_index].name,"help")==0)
				{
					usage();
				}
				else if(strcmp(long_options[option_index].name,"version")==0)
				{
					printf("version\n");
				}
			//	printf("\n");
				break;
			case 'c':
				g.caching=atoi(optarg);
				printf("Caching is turned: %s\n",(g.caching?"ON":"OFF"));
				break;
			case 'd':
				if(g.verbose>0)
					printf("path to disk: %s\n",optarg);
				g.disk_dir=malloc(strlen(optarg)+1);
				strcpy(g.disk_dir,optarg);
				break;
			case 's':
				printf("version\n");
//				g.port=atoi(optarg);
//				if(g.port==0)
//					printf("-p %s returned 0 so the default port will be used.\n",optarg);
//				printf("The size of the disk requested: %s\n",optarg);
				break;
			case '?':
				printf("?? getopt returned character code 0%o ??\n", c);
				break;
			default:
				printf("default: getopt returned character code 0%o ??\n", c);
		}
	}
}


stor_t* load_disk_path(char* path,bool create)
{
	stor_t		*d=NULL;

	/*  if file exists */
	if(access(path,F_OK)==-1)
	{
		if(g.verbose==1)
			fprintf(stderr,"File not at path. access() returned -1\n");
		if(create==true)
		{
		//	(path,size);
		}
	}

	if(access(path,F_OK)!=-1)
	{
		/*	Open disk device as a file	*/
//		d=stor_load(path);
		if(d==NULL)
		{
			fprintf(stderr,"Failed to load path. stor_load(path) returned NULL\n");
			exit(EX_IOERR);
		}
	}
	else
	{
		if(d==NULL)
		{
			//d=stor_init(path);
		}
	}
	return d;
}

void insert_file(stor_t *disk,char* data_pool,char* file_path)
{
	FILE		*f;
	void*		buff=NULL;
	uuid_t		uuid=NULL;
	size_t		size=0;
	size_t		offset=0;

	/*	Open the file for reading	*/
	if((f=fopen(file_path,"r"))==NULL)
	{
		fprintf(stderr,"Unable to open file for reading: %s\n",file_path);
		exit(EX_IOERR);
	}

	/*	Create metablock and add it to the pool	*/

	/*	Read in the blocks and add them to the pool	*/

	/*	Then append the metablock in the MDS pool	*/
	size=kMB(1);
	buff=malloc(kMB(1)+1);
	offset=0;
	while(!feof(f))
	{
		size=fread(buff,1,kMB(1),f);
		if(size)
		{
		//	uuid=stor_pool_add_block(disk,pool,data,size);
		//	uuid=stor_add_block(disk,data_pool,buff,size);
			printf("file block o: %10d  size: %10d  uuid: %s\n",(int)offset,(int)size,uuid);
			offset+=size;
		//	stor_add_block(disk1,pool_uuid,data,size);
		}
	}
	free(buff);
	fclose(f);
}
