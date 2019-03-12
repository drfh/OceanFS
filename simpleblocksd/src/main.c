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
#include <sys/types.h>
#include <getopt.h>
#include <errno.h>
#include <sysexits.h>
#include <sched.h>
#include <unistd.h>

#include "global.h"
#include "ipc.h"
#include "uuid4.h"
#include "storage.h"
#include "usage.h"

/** Defines **/


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
	printf("Caught signal %d\n",signum);
	if(signum==SIGINT)
	{
		if(g.done==true)
			exit(-1);
		g.done=true;
		fprintf("enter ^C again to exit(-1)\n");
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
	ipc_init(&ctx);

	/*	Check to see if file exists	*/
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

	shmb_t	*block=ctx.block;

	while(!g.done)
	{
		sched_yield();
		if(ipc_get_work(&ctx)==true)
		{
			printf("Block(%d)-> Message type: %d\n",ctx->block_epoc,block->head->type);
			if(block->head->type==100)
			{
				printf();
			}

			sem_post(ctx->block_sem);
		}
		sleep(1);
	}

	/*	Cleanup		*/
	fprintf(stderr,"\n");
	fprintf(stderr,"Cleanup...\n");
	fprintf(stderr,"Freeing cache...\n");

	fprintf(stderr,"Closing desk...\n");
	stor_free(d);
	fprintf(stderr,"Closing IPC...\n");
	ipc_destroy();
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
		{0, 0, 0, 0}
	};

	while((c=getopt_long(argc,(char**)argv,"d:s:cv",long_options,&option_index))!=-1)
	{
		switch(c)
		{
			case 0:
			//	printf("option %s @ %d\n",long_options[option_index].name,option_index);
				if(strcmp(long_options[option_index].name,"name")==0)
				{
					printf("name set to: %s\n",optarg);
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
//			case 's':
//				g.port=atoi(optarg);
//				if(g.port==0)
//					printf("-p %s returned 0 so the default port will be used.\n",optarg);
//				printf("The size of the disk requested: %s\n",optarg);
//				break;
			case '?':
				break;
			default:
				printf("?? getopt returned character code 0%o ??\n", c);
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
		d=stor_load(path);
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
			d=stor_init(path);
		}
	}
	return d;
}

void insert_file(stor_t *disk,char* data_pool,char* file_path)
{
	FILE		*f;
	void*		buff=NULL;
	/*	Open the file for reading	*/
	if((f=fopen(file_path,"r"))==NULL)
	{
		fprintf(stderr,"Unable to open file for reading: %s\n",file_path);
		exit(EX_IOERR);
	}
	free(buff);
	fclose(f);
}
