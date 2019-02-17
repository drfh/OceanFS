//
//  ipc.h
//  storage
//
//  Created by David Harris on 02/15/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//

#ifndef storage_ipc_h
#define storage_ipc_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>


void sharedmem_init(void);
void sharedmem_destroy(void);


#ifdef __cplusplus
}
#endif

#endif
