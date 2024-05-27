cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(concurrency VERSION ${PROJECT_VESRION})

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(Deadlock)

add_subdirectory(Dining_philosopfers)

add_subdirectory(Echo)

add_subdirectory(Guarded)

