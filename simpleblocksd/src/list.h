//
//  list.h
//  utils
//
//  Created by David Harris on 1/17/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#ifndef utils_list_h
#define utils_list_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

//#include <errno.h>


typedef struct list_s
{
	long		count;
	void**		array;
}list_t;


void* (*l_free_rec)(void*);
int (*l_find)(void*);
int (*l_compare)(void*,void*);


list_t* list_new(void)
{
	list_t		*l;

	l=malloc(sizeof(struct list_s));
	if(l!=NULL)
	{
		l->count=0;
		l->array=NULL;
	}
	return l;
}

void list_free(list_t* l,void (*l_free_rec)(void*))
{
	if(l!=NULL)
	{
		if(l->array!=NULL)
		{
			int32_t		i;

			for(i=0;i<l->count;i++)
				if(l->array[i]!=NULL)
				{
					if(l_free_rec!=NULL)
						(*l_free_rec)(l->array[i]);
					else
						free(l->array[i]);
					free(l->array[i]);
				}
		}
		free(l);
	}
}

void list_add(list_t* l,void* d)
{
	if(l!=NULL)
	{
		if(d!=NULL)
		{
			l->array=realloc(l->array,l->count*sizeof(d)+1);
			l->array[l->count]=d;
			l->count++;
		}
	}
}

void list_remove(list_t* l,void* d)
{
	if(l!=NULL)
	{
		if(d!=NULL)
		{
			int32_t		i;

			for(i=l->count;i>=0;i--)
			{
				if(l->array[i]==d)
					break;
			}
			if(i>=0)
			{
				l->array[l->count]=NULL;
				if(i<l->count)
					memmove((l->array)+sizeof(void*)*i,(l->array)+sizeof(void*)*i+1,l->count-i);
				l->count--;
			}
		}
	}
}
//,int (*l_find)(void*)
int32_t list_find(list_t* l,void* d,int comp(void*,void*))
{
	if(l!=NULL)
	{
		if(d!=NULL)
		{
			int32_t		i;

			for(i=l->count;i>=0;i--)
			{
				if(comp(d,l->array[i]))
					return i;
			}
		}
	}
	return -1;
}



#ifdef __cplusplus
}
#endif

#endif
