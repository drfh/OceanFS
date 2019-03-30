cmake_minimum_required(VERSION 3.7)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Third-party
add_subdirectory(uuid4)

add_subdirectory(util)
add_subdirectory(libpeers)
