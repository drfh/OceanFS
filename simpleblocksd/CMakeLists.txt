cmake_minimum_required(VERSION 3.7)
project(simpleblocksd)


set(sources "src/main.c")
#set(sources ${sources} " ../src/main.c")
set(sources ${sources} "src/log.c")
set(sources ${sources} "src/ipc.c")
#set(sources ${sources} "src/blocks.c")
#set(sources ${sources} "src/storage.c")
#set(sources ${sources} "src/spacemap.c")
set(sources ${sources} "src/md5.c")
set(sources ${sources} "src/sha256.c")
set(sources ${sources} "src/uuid4.c")
#set(sources ${sources} "src/bencode.c")

set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

add_executable(simpleblocksd ${sources})
#target_compile_options(simpleblocksd PRIVATE -j)

install(TARGETS simpleblocksd DESTINATION bin)
