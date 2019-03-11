//
//  storage.c
//  simpleblocksd
//
//  Created by David Harris on 12/20/2018.
//  Copyright (c) 2018 MegaApps. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//	#include <file.h>
#include <errno.h>
#include <time.h>

#include "storage.h"


stor_t*	stor_alloc(void);

int l_compare_uuid(void* a,void* b);


stor_t*	stor_init(const char* dir)
{
	stor_t		*s;

	s=stor_alloc();
	if(s!=NULL)
	{
		s->status=0;

		s->uuid=malloc(UUID4_LEN);
		uuid4_generate(s->uuid);

		s->name=malloc(128);

		;
	}
	return NULL;
}


stor_t*	stor_alloc(void)
{
	stor_t		*s=malloc(sizeof(stor_t));
	if(s)
	{
		s->uuid=NULL;
		s->status=0;
		s->name=NULL;
		s->dir=NULL;
		s->dev=NULL;
		s->pools=NULL;
		s->blocks=NULL;
	}
	return s;
}


void stor_free(stor_t *s)
{
	if(s!=NULL)
	{
	//	if(g.verbose>0)
	//		printf("free: %s -> uuid\n",s->name);
		free(s->uuid);

	//	if(g.verbose>0)
	//		printf("free: %s -> dir\n",s->name);
		free(s->dir);

	//	if(g.verbose>0)
	//		printf("free: %s -> pools\n",s->name);
		free(s->pools);

	//	if(g.verbose>0)
	//		printf("free: %s -> blocks\n",s->name);
		free(s->blocks);

	//	if(g.verbose>0)
	//		printf("free: %s -> blocks\n",s->name);

		free(s->name);
		free(s);
	}
}

stor_t*	stor_load(const char* dir)
{
	stor_t		*s=stor_alloc();

	if(s)
	{
		FILE*		file=NULL;

		if(dir)
		{
			if((file=fopen(dir,"r"))==NULL)
			{
				fgets(s->uuid,UUID4_LEN,file);
				fgets(s->name,128,file);

				/* Read pools */
//				pool_t		*pt=NULL;
				while(0)
				{
					pool_t		*p=malloc(sizeof(pool_t));


					p->uuid=malloc(UUID4_LEN);
					p->parent_uuid=malloc(UUID4_LEN);
					p->uuid=malloc(kPOOL_NAME_LEN);

					fgets(p->uuid,UUID4_LEN,file);
					fgets(p->name,128,file);
					fgets(p->parent_uuid,UUID4_LEN,file);

				}

				/* Read blocks */
				while(0)
				{
					;
				}
			}
			else
		    {
		      printf("An error occurred while opening the file.\n");
		    }
		}
		s->name=malloc(128);

	}
	else
	{
		fprintf(stderr,"%s: %s  %d\n",__FILE__,__FUNCTION__,__LINE__);
	}
	return s;
}

void stor_disk_uuid(stor_t *s,uuid_t uuid)
{
	if(s!=NULL)
		if(s->uuid!=NULL)
			if(uuid!=NULL)
				strncpy(uuid,s->uuid,UUID4_LEN);
}

u_int32_t stor_pool_count(stor_t *s)
{
	if(s!=NULL)
	{
		return s->p_count;
	//	return s->pool_lst;
	}
	return 0;
}

uuid_t stor_pool_create(stor_t *s,char* name)
{
	pool_t		*p=malloc(sizeof(pool_t));
//	pool_t		*tp=NULL;

	p->uuid=malloc(UUID4_LEN);
	uuid4_generate(p->uuid);
	strcpy(p->name,name);
	s->pool_lst[s->p_count]=p;

	s->p_count++;
	return p->uuid;
}

void stor_pool_destroy(stor_t *s,uuid_t uuid)
{
	int32_t 		i=0;
	if(s!=NULL)
	if(uuid!=NULL)
	{
		i=stor_pool_find_uuid_index(s,uuid);
		if(i==-1)
		{
			printf(stderr,"can't find uuid: %s\n",uuid);
		}
		else
		{
			pool_t		*p;

			printf("remove pool @ index: %d\n",i);
			p=s->pool_lst[i];
			printf("pool index:%d  name: %s\n",i,p->name);
			free(s->pool_lst[i]);

		}
	}
}

u_int_32 stor_pool_find_uuid_index(stor_t *s,uuid_t uuid)
{
	s_int_32 		ret=0;
	s_int_32		i;

	for(i=0;i<s->p_count;i++)
		if(strncmp(s->pool_lst[i],uuid,UUID4_LEN)==0)
			return i;
	return -1;
}

//void stor_pool_list_copy(stor_t *s,pool_t** list,u_int32_t *len)
void stor_pool_list_copy(stor_t *s,uuid_t** list,u_int32_t *len)
{
	if(s!=NULL)
	{
		int32_t		i;
		pool_t		p;

		len=s->p_count;
		if(len)
		{
			list=malloc(sizeof(list)*len+1);
			for(i=0;i<len;i++)
			{
				p=malloc(UUID4_LEN);
			//	strcpy(*list[i],s->pools[i]->uuid);
				memcpy(p,s->pools[i]->uuid,UUID4_LEN);
				*list[i]=p;
			}
		}
	}
}

/*
int l_compare_uuid(void* a,void* b)
{
	return strncmp((pool_t*)a->uuid,(pool_t*)b->uuid,UUID4_LEN);
}

int l_compare_name(void* a,void* b)
{
	return strncmp((pool_t*)a->name,(pool_t*)b->name,255);
}
*/
/*
void stor_pool_uuid(stor_t *s,uuid_t uuid)
{
	if(s!=NULL)
	{
		;
		if()
		{
			if(uuid!=NULL)
			{
				strncpy(uuid,p->uuid,UUID4_LEN);
			}
		}
	}
}
*/
uuid_t stor_pool_uuid_for_name(stor_t *s,char* name)
{
	pool_t		*p=s->pools;

	while(p!=NULL)
	{
		if(strncmp(p->uuid,name,UUID4_LEN)==0)
			return p->uuid;
		p=p->next;
	}
	return NULL;
}
