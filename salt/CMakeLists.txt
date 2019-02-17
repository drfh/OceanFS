cmake_minimum_required(VERSION 3.7)

project(salt)
#	Storage Abstraction Library Tool (SALT)



set(sources "src/main.c" )

add_executable(salt ${sources})
target_link_libraries(salt libpeers)
