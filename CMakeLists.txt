cmake_minimum_required(VERSION 3.7)
project(OceanFS LANGUAGES C)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PROJECT_DESCRIPTION "")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://www.github.com/DRFH/OceanFS")

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")


#if(EXISTS "libpeer")
	add_subdirectory(util)
	#add_library(utils MODULE util)
#endif(EXISTS "libpeer")

#if(EXISTS "libpeer")
#	add_subdirectory(libpeer)
#endif(EXISTS "libpeer")

#if(EXISTS "simpleblocksd")
	add_subdirectory(simpleblocksd)
#endif(EXISTS "simpleblocksd")

# if(EXISTS "salt")
	add_subdirectory(salt)
# endif(EXISTS "salt")

# if(EXISTS "tools")
# 	add_subdirectory(tools)
# endif(EXISTS "tools")

#add_subdirectory(oceanfs)

if(EXISTS "test")
	add_subdirectory(test)
endif(EXISTS "test")
