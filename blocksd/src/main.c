//
//  main.c
//  storage
//
//  Created by David Harris on 12/20/2018.
//  Copyright (c) 2018 MegaApps. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

#include <errno.h>

#include <sysexits.h>
#include <sched.h>
#include <unistd.h>

#include "ipc.h"
#include "storage.h"

/** Defines **/
//#define kDEFAULT_PORT		8321
//#define	kNUM_PEER_TEST		10

/** Prototypes **/
void alocate_file(const char* path,size_t size);
void parse_args(int argc,char const *argv[]);
void insert_file(stor_t *disk,char* data_pool,char* file_path);
stor_t* load_disk_path(char* path,bool create);
void list_pools(stor_t *disk);

void add_pool_and_print_uuid(stor_t *d,const char* name);


struct global{
	bool		done;
	bool		create;
	char*		disk_dir;
	int			verbose;
	u_int_64	disk_size;
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

int main(int argc,char const **argv)
{
	stor_t		*d;
	char*		name=malloc(128);
	int			return_val=0;

	fprintf(stderr,"-----------------\n");
	fprintf(stderr,"stordir - v0.2\n");
	fprintf(stderr,"libPeers\n");
	fprintf(stderr,"-----------------\n");

//	int			i,j=kNUM_PEER_TEST;
//	char		address[24];
//	char		*tmp_str1;

	/*	Setup program globals	*/
	init_globals();
	fprintf(stderr,"Parse args...\n");
	parse_args(argc,(const char **)argv);

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

/*	list_pools(disk1);

	uuid_t		data_uuid=NULL;
	uuid_t		meta_data_uuid=NULL;
	uuid_t		other_uuid=NULL;

	add_pool_and_print_uuid(disk1,"data");
	add_pool_and_print_uuid(disk1,"metadata");
	add_pool_and_print_uuid(disk1,"some other pool");
	add_pool_and_print_uuid(disk1,"ISOs");
	add_pool_and_print_uuid(disk1,"VM Storage 1");
	add_pool_and_print_uuid(disk1,"CDN");

/*	strcpy(name,"metadata");
	meta_data_uuid=stor_pool_create(disk1,name);
	printf("uuid: %s\n",meta_data_uuid);

	strcpy(name,"other pool");
	other_uuid=stor_pool_create(disk1,name);
	printf("uuid: %s\n",other_uuid);
*/
/*	strcpy(name,"./test_file1.dat");
	insert_file(disk1,data_uuid,name);

	strcpy(name,"./test_file2.dat");
	insert_file(disk1,data_uuid,name);

	strcpy(name,"./test_file2.dat");
	insert_file(disk1,data_uuid,name);
*/

//	sleep(1);
	fprintf(stderr,"\n");

	fprintf(stderr,"Cleanup...\n");
	free(name);
	stor_free(d);
	fprintf(stderr,"Exiting...\n");
	return return_val;
}

void alocate_file(const char* path,size_t size)
{
	FILE		*f;
	char		*buff;
	size_t		i;

	if(size<kMB(16))
	{
		fprintf(stderr,"size is too small:  %d\n",size);
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
/*	if(argc>=2)
	{
		fprintf(stderr,"%s\n","error - Argument number not correct");
	}
*/
}

void add_pool_and_print_uuid(stor_t *d,const char* name)
{

	uuid_t		data_uuid=NULL;
	uuid_t		meta_data_uuid=NULL;
	uuid_t		other_uuid=NULL;

	strcpy(name,"data");
	data_uuid=stor_pool_create(d,name);
	printf("uuid: %s name: %s\n",data_uuid,name);
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
		if(d==NULL&&)
		{
			d=stor_init(path);
		}
	}
	return d;
}

void list_pools(stor_t *disk)
{
	int		i,j=0;
	uuid_t	disk_uuid=malloc(UUID4_LEN+1);
	pool_t	**pool_lst;
	char**	uuid_list;
	char*	pool_uuid;

	stor_disk_uuid(disk,disk_uuid);
	printf("List pools for storage: %s\n",disk_uuid);
	stor_pool_list_copy(disk,pool_lst,&j);
//	stor_pool_uuid_list_copy(disk,&uuid_list);
//	j=stor_pool_count(disk);
	printf("Count: %d\n",j);
	printf("-----------------------\n");
	while((pool_uuid=uuid_list[i++])!=NULL)
//	for(i=0;i<j;i++)
	{
		printf("(%d)  %s\n",i,pool_uuid);
	}
	printf("-----------------------\n");
}


void insert_file(stor_t *disk,char* data_pool,char* file_path)
{
	FILE		*f;
	uuid_t		uuid=NULL;
	void*		buff;
	size_t		size;
	size_t		offset;

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
			uuid=stor_add_block(disk,data_pool,buff,size);
			printf("file block o: %10d  size: %10d  uuid: %s\n",offset,size,uuid);
			offset+=size;
		//	stor_add_block(disk1,pool_uuid,data,size);
		}
	}
	free(buff);
	fclose(f);
}
