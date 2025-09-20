cmake_minimum_required(VERSION 4.1.1)
project(CAS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wpedantic -Ofast)
endif()

file(GLOB_RECURSE CAS_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(CAS ${CAS_SOURCES})

message(STATUS "Sources: ${CAS_SOURCES}")