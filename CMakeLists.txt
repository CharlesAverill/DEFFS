cmake_minimum_required(VERSION 3.13)
project(DEFFS C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

find_package(FUSE REQUIRED)
find_package(OpenSSL REQUIRED)

add_compile_options(-D_FILE_OFFSET_BITS=64)

include_directories(${FUSE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
link_libraries(${FUSE_LIBRARIES})
link_libraries(crypto)

add_executable(DEFFS src/deffs.c src/attr.c src/utils.c src/arguments.c src/rw.c src/crypto.c src/perms.c src/shamir.c)
