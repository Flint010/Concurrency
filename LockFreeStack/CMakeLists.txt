# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.5)

# Define the project name and specify the programming language.
project(Stack LANGUAGES CXX)

# Set the C++ standard to version 20 and require its usage.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
