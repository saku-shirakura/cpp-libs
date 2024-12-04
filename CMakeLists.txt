cmake_minimum_required(VERSION 3.28)
project(cpp-libs)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp-libs STATIC src/ArgumentParser.cpp src/ParseUtil.cpp)

target_include_directories(cpp-libs PUBLIC include)

###
# from https://github.com/google/googletest/tree/main/googletest#incorporating-into-an-existing-cmake-project
###
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/7927f8e93d2cc7788229ab446179fdfda15e75e4.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

###
# GoogleTest
###
include(GoogleTest)
enable_testing()

add_executable(cpp-libs-test test/unit_test_ParseUtil.cpp)
target_link_libraries(cpp-libs-test  gtest_main cpp-libs)

gtest_discover_tests(cpp-libs-test)
