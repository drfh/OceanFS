cmake_minimum_required(VERSION 3.7)
project(util LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(util_headers "global.h")
set(util_headers ${util_headers} "log.h")
set(util_headers ${util_headers} "ipc.h")
set(util_headers ${util_headers} "base64.h")
set(util_headers ${util_headers} "md5.h")
set(util_headers ${util_headers} "sha256.h")

set(util_sources "log.c")
set(util_sources ${util_sources} "ipc.c")
set(util_sources ${util_sources} "base64.c")
set(util_sources ${util_sources} "md5.c")
set(util_sources ${util_sources} "sha256.c")


set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -lrt -lpthread ")
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} " -lrt -lpthread ")
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} " -lpthread -lrt ")

add_library(util SHARED ${util_sources} ${util_headers})

target_link_libraries("util" "uuid4" "pthread" "rt")

target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
