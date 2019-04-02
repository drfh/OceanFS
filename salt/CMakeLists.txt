cmake_minimum_required(VERSION 3.7)
project(salt)


set(sources "src/main.c")
#set(sources ${sources} "src/blocks.c")
#set(sources ${sources} "src/storage.c")
#set(sources ${sources} "src/spacemap.c")

set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

# target_include_directories(util PUBLIC )
# include_directories("../")

add_executable(salt ${sources})
#target_compile_options(salt PRIVATE -j)
target_link_libraries(salt "util" "uuid4")# libpeers)

install(TARGETS salt DESTINATION bin)
