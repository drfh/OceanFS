//
//  config.c
//  salt
//
//  Created by David Harris on 03/25/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "config.h"

void load_config(const char* path)
{
	config_t		*config=NULL;
	FILE			*file_conf=NULL;

	if((file_conf=fopen(path,"r"))==NULL)
	{
		config=malloc(sizeof(config_t));
		assert(config!=NULL);


	}

}
