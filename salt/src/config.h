//
//  config.h
//  salt
//
//  Created by David Harris on 03/25/2019.
//  Copyright (c) 2019 MegaApps. All rights reserved.
//
// salt mup -d
// salt get --cluster cluster
// salt get --swarm UUID --
// salt --config filename.json read --node UUID --block SHA256 > test_file.test
// salt --config filename.json write --node UUID --block SHA256 < test_file.test
//

#ifndef salt_config_h
#define salt_config_h

#ifdef __cplusplus
extern "C" {
#endif

struct config_s
{
	char		cluster_uuid[32];
	struct
	{
		char	name[64];
		char	uuid[32];
	}persona;
	char		*certificate;
	char		*ip_hint[16];
};
typedef struct config_s		config_t;


void load_config(const char* path);




#ifdef __cplusplus
}
#endif

#endif
