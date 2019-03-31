cmake_minimum_required(VERSION 3.7)
project(mupt LANGUAGES C)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PROJECT_DESCRIPTION "MeetUpPage tool")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://www.github.com/DRFH/OceanFS")

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")


set(sources "mupt.c")


set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread")

add_executable(mupt ${sources})

install(TARGETS mupt DESTINATION bin)
