cmake_minimum_required(VERSION 3.28)
project(cpp-libs)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp-libs STATIC src/ArgumentParser.cpp)

target_include_directories(cpp-libs PUBLIC include)
