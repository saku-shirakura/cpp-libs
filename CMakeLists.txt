cmake_minimum_required(VERSION 3.28)
project(cpp-libs)

set(CMAKE_CXX_STANDARD 20)

add_library(cpp-libs STATIC
        src/ArgumentParser.cpp
        src/ParseUtil.cpp
        src/PrintHelper.cpp
        src/multi_platform_util.cpp
)

target_include_directories(cpp-libs PUBLIC include)

if (${MSVC})
    target_compile_options(cpp-libs PRIVATE "/utf-8")
endif ()


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

add_executable(cpp-libs-test test/ParseUtil/unit_test_ParseUtil.cpp
        test/ArgumentParser/unit_test_ArgumentParser.cpp
        test/ArgumentParser/unit_test_OptionValue.cpp
        test/PrintHelper/unit_test_PrintHelper.cpp)

target_link_libraries(cpp-libs-test PRIVATE gtest_main cpp-libs)

gtest_discover_tests(cpp-libs-test)

if (${MSVC})
    target_compile_options(cpp-libs-test PRIVATE "/utf-8")
endif ()
