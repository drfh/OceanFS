cmake_minimum_required(VERSION 3.7)
project(simpleblocksd LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS}" -lrt -lpthread ")

set(MAJOR 0)
set(MINOR 1)
set(MAINTENANCE 25)
set(VERSION_STRING "${MAJOR}.${MINOR}.${MAINTENANCE}")

configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
include_directories("${PROJECT_BINARY_DIR}")

set(sources "src/main.c")
#set(sources ${sources} "src/blocks.c")
#set(sources ${sources} "src/storage.c")
#set(sources ${sources} "src/spacemap.c")
#set(sources ${sources} "src/bencode.c")

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} " -lrt -lpthread ")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lrt -lpthread ")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -lrt -lpthread ")

add_executable(simpleblocksd ${sources})

add_dependencies(simpleblocksd util uuid4)

target_link_libraries(simpleblocksd "util" "uuid4" "pthread" "rt")

#install(TARGETS simpleblocksd DESTINATION bin)
