# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.5)

# Define the project name and specify the programming language.
project(Stack LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(tests)
