cmake_minimum_required(VERSION 3.13)
project(OceanFS LANGUAGES C)

#if(EXISTS "libpeer")
#	add_subdirectory(libpeer)
#endif(EXISTS "libpeer")

#if(EXISTS "simpleblocksd")
	message("*********simpleblocksd")
	add_subdirectory(simpleblocksd)
#endif(EXISTS "simpleblocksd")

#if(EXISTS "salt")
#	add_subdirectory(salt)
#endif(EXISTS "salt")

#add_subdirectory(oceanfs)

if(EXISTS "test")
	add_subdirectory(test)
endif(EXISTS "test")
