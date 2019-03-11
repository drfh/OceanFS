cmake_minimum_required(VERSION 3.7)
project(blocksd)

set(${sources} "../src/main.c")
set(${sources} "../src/log.c")
set(${sources} "../src/ipc.c")
set(${sources} "../src/blocks.c")
set(${sources} "../src/storage.c")
set(${sources} "../src/spacemap.c")
set(${sources} "../src/md5.c")
set(${sources} "../src/sha256.c")
set(${sources} "../src/uuid4.c")
set(${sources} "../src/bencode.c")

add_executable(simpleblocksd ${sources})

install(TARGETS simpleblocksd DESTINATION bin)
