//
//  main.c
//  salt
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
#include <ctype.h>

#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <sysexits.h>
#include <sched.h>
#include <signal.h>

#include "log.h"
#include "base64.h"
#include "md5.h"
// #include "sha265.h"

#include "config.h"
#include "parse.h"
#include "ipc.h"
#include "usage.h"

#define	__BUILD_VERSION__	26

/** Defines **/
#define	kSLEEP_TICKS_DEFAULT	1000*10*1
#define	kSLEEP_TICKS_SLOW		10000*10*1
#define	kATUOEXIT_TIME			2

#define kDEFAULT_CONFIG			"{\"cluster\":\"5F1FD3A9A4814D38B7C2BFD3C4B46F10\",\"number\":123,\"persona\":{\"name\":\"admin\",\"UUID\":\"02CF8D52C0A64487ADE82F1AA2272C93\"},\"cert\":null,\"ip_hints\":[\"10.10.0.101:9909\",\"10.10.0.102:9909\",\"10.10.0.103:9909\"]}3\0"

/** Prototypes **/
void init_globals(void);
void init_signals(void);
void signal_handler(int signum);
void parse_args(int argc,char const *argv[]);
char *strlwr(char *str);

char *strlwr(char *str)
{
	unsigned char	*p=(unsigned char*)str;

	while(*p)
	{
		*p=tolower((unsigned char)*p);
		p++;
	}
	return str;
}


struct global{
	bool		done;
	int			verbose;

	char*		config_path;
	config_t	*config;

	char		*action;
};
struct	global		g;

void init_globals(void)
{
	g.done=false;
	g.verbose=1;

	g.config_path=NULL;
	g.config=NULL;

	g.action=NULL;
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

void print_version(void)
{
	fprintf(stdout,"-----------------\n");
	fprintf(stderr,"salt - v0.2   build: %0.4x\n",__BUILD_VERSION__);
	fprintf(stderr,"-----------------\n");
}

int main(int argc,char const **argv)
{
	int			return_val=0;
	ipc_t		ctx;
	char		*path;
	char		*test_str;
	char		*b64_str;

	init_globals();
	print_version();
	init_signals();
	parse_args(argc,argv);

	printf("\n\n\n");
	test_str=malloc(strlen(kDEFAULT_CONFIG)+1);
	b64_str=malloc(strlen(kDEFAULT_CONFIG));
	strcpy(test_str,kDEFAULT_CONFIG);
//	sprintf(test_str,"Hello to all and others.");
	b64_str=b64_encode(test_str,strlen(test_str));
	printf("base256(%d): %s\n",strlen(test_str),test_str);
	// printf("Compressed(%d): \n",);
	printf("base64(%d): %s\n",strlen(b64_str),b64_str);

	if(g.done==false&&g.action!=NULL)
	{
		// FILE		*file;
		// char		*buff;
		// long		len;
		//
		// assert(file=fopen(path,"r"))==0);
		// fread();
		if(strcmp(strlwr(g.action),"status")==0)
		{
			printf("\n");
		}
		else if(strcmp(strlwr(g.action),"read")==0)
		{
			printf("action=> %s\n",g.action);

		}
		else if(strcmp(strlwr(g.action),"write")==0)
		{
			printf("action=> %s\n",g.action);
		}
		else if(strcmp(strlwr(g.action),"admin")==0)
		{
			printf("action=> %s\n",g.action);
		}
		else if(strcmp(strlwr(g.action),"disk")==0)
		{
			printf("action=> %s\n",g.action);
		}
		else
		{
			printf("unknown action=> '%s' %x %d\n",g.action,g.action,g.action);
		}
	}
	return return_val;
}


void parse_args(int argc,char const *argv[])
{
	int		c;
//	int		digit_optind = 0;
//	int		this_option_optind = optind ? optind : 1;
	int		option_index = 0;

	static struct option long_options[]={
		{"config", required_argument, 0, 'c'},
//		{"size", required_argument, 0, 's'},
		{"verbose", no_argument, 0, 'v'},
		{"quiet", no_argument, 0, 'q'},
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 0},
		{"usage", no_argument, 0, 0},
		{0, 0, 0, 0}
	};

	while((c=getopt_long(argc,(char**)argv,"c:vV",long_options,&option_index))!=-1&&!g.done)
	{
	//	printf("option[%d] %s\n",option_index,long_options[option_index].name,option_index);
		switch(c)
		{
			case 0:
				if(strcmp(long_options[option_index].name,"name")==0)
				{
					printf("name set to: %s\n",optarg);
				}
				else if(strcmp(long_options[option_index].name,"help")==0)
				{
					usage();
					g.done=true;
				}
				else if(strcmp(long_options[option_index].name,"config")==0)
				{
					printf("config path: %s\n",optarg);
				}
				else if(strcmp(long_options[option_index].name,"version")==0)
				{
					printf("version\n");
				}
				else
				{
					printf("long_options[option_index].name==%s\n",long_options[option_index].name);
				}
				break;
			case 'c':
				printf("Config arg: %s\n",optarg);
				break;
			case 's':
				printf("version\n");
//				g.port=atoi(optarg);
//				if(g.port==0)
//					printf("-p %s returned 0 so the default port will be used.\n",optarg);
//				printf("The size of the disk requested: %s\n",optarg);
				break;
			case '?':
				printf("?? getopt returned character code 0%o %c %d??\n", c);
				break;
			case 'v':
				if(g.verbose!=-1)
					g.verbose++;
				break;
			case 'q':
				g.verbose=-1;
				break;
			case 'V':
				g.done=true;
				print_version();
				break;
			default:
				printf("default: (!we sould never get here!) getopt returned character code 0%o ??\n", c);
		}
	}
}
