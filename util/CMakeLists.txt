cmake_minimum_required(VERSION 3.7)

set(util_headers "global.h")
set(util_headers ${util_headers} "log.h")

set(util_sources "log.c")
set(util_sources ${util_sources} "base64.c")
set(util_sources ${util_sources} "md5.c")
set(util_sources ${util_sources} "sha256.c")
set(util_sources ${util_sources} "uuid4.c")


set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

add_library(util OBJECT ${util_sources})

target_include_directories(util PUBLIC ${PROJECT_SOURCE_DIR}/util)

