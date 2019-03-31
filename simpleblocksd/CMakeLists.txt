cmake_minimum_required(VERSION 3.7)
project(simpleblocksd)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
# add_executable(simpleblocksd ${sources} $<TARGET_OBJECTS:util>)
# add_dependencies(simpleblocksd $<TARGET_OBJECTS:util> $<TARGET_OBJECTS:uuid4>)
add_dependencies(simpleblocksd util uuid4)
#target_compile_options(simpleblocksd PRIVATE -j)
target_link_libraries(simpleblocksd "util" "uuid4")# libpeers)
# target_link_libraries(simpleblocksd "lib/util")# libpeers)

install(TARGETS simpleblocksd DESTINATION bin)
