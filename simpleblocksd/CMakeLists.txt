cmake_minimum_required(VERSION 3.7)
project(simpleblocksd)


set(sources "src/main.c")
set(sources ${sources} "src/ipc.c")
#set(sources ${sources} "src/blocks.c")
#set(sources ${sources} "src/storage.c")
#set(sources ${sources} "src/spacemap.c")
#set(sources ${sources} "src/bencode.c")

set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

# target_include_directories(util PUBLIC )
# include_directories("../")

add_executable(simpleblocksd ${sources})
#target_compile_options(simpleblocksd PRIVATE -j)
target_link_libraries(simpleblocksd util)# libpeers)

install(TARGETS simpleblocksd DESTINATION bin)
