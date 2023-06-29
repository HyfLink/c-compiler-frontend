cmake_minimum_required(VERSION 3.0.0)
project(mcc VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g -Wall -Wextra)

include_directories(${CMAKE_SOURCE_DIR}/src)

aux_source_directory(${CMAKE_SOURCE_DIR}/src MCC_SOURCES)

add_executable(mcc ${MCC_SOURCES})
