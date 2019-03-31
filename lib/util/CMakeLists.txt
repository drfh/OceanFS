cmake_minimum_required(VERSION 3.7)
set(CMAKE_BUILD_TYPE Debug)

set(util_headers "global.h")
set(util_headers ${util_headers} "log.h")
set(util_headers ${util_headers} "base64.h")
set(util_headers ${util_headers} "md5.h")
set(util_headers ${util_headers} "sha256.h")
# set(util_headers ${util_headers} "md5.h")

set(util_sources "log.c")
set(util_sources ${util_sources} "base64.c")
set(util_sources ${util_sources} "md5.c")
set(util_sources ${util_sources} "sha256.c")
# set(util_sources ${util_sources} "uuid4.c")


# set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

add_library(util ${util_sources} ${util_headers})

# target_link_libraries(util uuid4)

target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
# target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
