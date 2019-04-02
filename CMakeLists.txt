cmake_minimum_required(VERSION 3.7)
project(OceanFS LANGUAGES C)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PROJECT_DESCRIPTION "")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://www.github.com/DRFH/OceanFS")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

add_subdirectory(lib)

add_subdirectory(simpleblocksd)
add_subdirectory(salt)

add_subdirectory(oceanfs)

add_subdirectory(tools)
add_subdirectory(tests)
