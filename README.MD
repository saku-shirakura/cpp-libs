# cpp-libs

[![CMake on a single platform](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml/badge.svg)](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml)

C++用の雑多なライブラリです。

以下の内容を含んでいます。

1. argument-parser

   コマンドライン引数用のパーサーです。

2. ParseUtil
   
   パーサの実装に使えるユーティリティです。
## CMakeプロジェクトにこのライブラリを含める

```cmake
include(FetchContent)
FetchContent_Declare(
        cpp-libs
        GIT_REPOSITORY https://github.com/saku-shirakura/win-cbt-app.git
        GIT_TAG v0.0.1-alpha # 必要に応じてバージョンをあげること。
)

FetchContent_MakeAvailable(cpp-libs)

# 以下は各自のプロジェクトの内容に合わせる。
add_executable(example example.cpp)
target_link_libraries(example cpp-libs)
```
