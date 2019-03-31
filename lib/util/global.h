//
//  global.h
//  oceanfs_util
//
//  Created by David Harris on 11/16/18.
//  Copyright (c) 2011 MegaApps. All rights reserved.
//


#ifndef libpeers_global_h
#define libpeers_global_h

#ifdef __cplusplus
extern "C" {
#endif

#define		kKB(x)		(x*1024)
#define		kMB(x)		(x*kKB(1000))
#define		kGB(x)		(x*kMB(1000))

#define		kKB4		(4096)
#define		kKB8		(8192)
#define		kKB16		(16384)
#define		kKB32		(32768)
#define		kKB64		(65536)

#define		k4KB		kKB4
#define		k8KB		kKB8
#define		k16KB		kKB16
#define		k32KB		kKB32
#define		k64KB		kKB64


#ifndef global
	extern struct	global		g;
#endif

#ifdef __cplusplus
}
#endif

#endif
