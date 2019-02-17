cmake_minimum_required(VERSION 3.7)
project(blocksd)

set(sources "src/main.c" "src/storage.c" "src/blocks.c" "src/spacemap.c" "src/ipc.c")

add_executable(blocksd ${sources})

install(TARGETS blocksd DESTINATION bin)
