cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME serialization)

project(${TARGET_NAME} LANGUAGES C CXX)

add_library(${TARGET_NAME} INTERFACE)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE header_files CONFIGURE_DEPENDS include/*)
target_sources(${TARGET_NAME} INTERFACE ${header_files})

target_include_directories(${TARGET_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(test)
