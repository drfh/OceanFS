cmake_minimum_required(VERSION 3.7 LANGUAGE C)
project(OceanFS)

add_subdirectory(libpeer)
add_subdirectory(blocksd)
add_subdirectory(salt)

add_subdirectory(oceanfs)
