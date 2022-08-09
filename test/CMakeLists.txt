cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME serialization_test)

enable_testing()
add_executable(${TARGET_NAME})

file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS src/*)
target_sources(${TARGET_NAME} PRIVATE ${test_sources})
target_include_directories(${TARGET_NAME} PRIVATE src)
target_link_libraries(${TARGET_NAME} PRIVATE serialization)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
target_link_libraries(${TARGET_NAME}
  PRIVATE
    GTest::gtest
    GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
