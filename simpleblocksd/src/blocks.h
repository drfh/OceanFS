//
//  blocks.h
//  simpleblocks
//
//  Created by David Harris on 12/20/2018.
//  Copyright (c) 2018 MegaApps. All rights reserved.
//



#ifndef simpleblocksd_blocks_h
#define simpleblocksd_blocks_h

#ifdef __cplusplus
extern "C" {
#endif


typedef	struct metablock_s	metablock_t;
struct metablock_s
{
	long		number_blocks;

	struct
	{
		uint64_t	length;
		char		block_hash[32];
	}*blocks_l;
};

// typedef	struct namedblock_s	namedblock_t;
// struct namedblock_s
typedef	struct tagblock_s	tagblock_t;
struct tagblock_s
{
	char		tag[64];

	char		data_hash[32];
	long		number_blocks;
	uint64_t	length;
};


typedef	struct block_s	block_t;
struct block_s
{
	uint8_t		version;
	char		hash[32];
	long		type;
	long		len;
	union
	{
		tagblock_t	*tag;
		metablock_t	*meta;
		caddr_t		buff;
	}block_u;
};


#ifdef __cplusplus
}
#endif

#endif
