cmake_minimum_required(VERSION 3.6)
project(dvdbackup)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(DvdRead REQUIRED)
find_package(IOKit REQUIRED)
find_package(CoreFoundation REQUIRED)

include_directories($IOKit_INCLUDE_DIRS})
include_directories($CoreFoundation_INCLUDE_DIRS})

add_subdirectory(src)
