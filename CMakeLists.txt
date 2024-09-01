cmake_minimum_required(VERSION 3.28)
project(cpp-libs)

set(CMAKE_CXX_STANDARD 20)

add_library(argument-parser STATIC src/command-parser/ArgumentParser.cpp src/command-parser/ArgumentParser.h)
