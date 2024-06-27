cmake_minimum_required(VERSION 3.10)

# Set the C++ standard to version 20 and require its usage.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(concurrency VERSION ${PROJECT_VESRION})

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

add_subdirectory(Deadlock)

add_subdirectory(Dining_philosopfers)

add_subdirectory(LockFreeStack)

add_subdirectory(Echo)

add_subdirectory(Guarded)

