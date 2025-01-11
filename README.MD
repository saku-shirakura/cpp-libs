# cpp-libs

[![test](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml/badge.svg?branch=develop&event=pull_request)](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml)

C++用の雑多なライブラリです。

以下の内容を含んでいます。

1. ArgumentParser

   コマンドライン引数用のパーサーです。

2. ParseUtil

   パーサの実装に使えるユーティリティです。

3. PrintHelper
   
   シンプルなCUI用ユーティリティです。

4. CodePageGuard
   
   Windows環境で、コンソール出力にUTF-8を使用するためのクラスです。他の文字コードも設定できます。

   linux環境などをターゲットにコンパイルすれば、クラスの関数やコンストラクタは全て空の関数が定義されます。

5. EnableAnsiEscapeSequence

   Windows環境で、コンソール出力にAnsiエスケープシーケンスを使用するためのユーティリティです。

   `EnableAnsiEscapeSequence::enable()`で使用でき、`EnableAnsiEscapeSequence::isEnabled()`で有効か否かを確認できます。

## CMakeプロジェクトにこのライブラリを含める

```cmake
include(FetchContent)
FetchContent_Declare(
        cpp-libs
        GIT_REPOSITORY https://github.com/saku-shirakura/win-cbt-app.git
        GIT_TAG v0.1.2-alpha # 必要に応じてバージョンをあげること。
)

FetchContent_MakeAvailable(cpp-libs)

# 以下は各自のプロジェクトの内容に合わせる。
add_executable(example example.cpp)
target_link_libraries(example cpp-libs)
```

## リンク
- [リポジトリ](https://github.com/saku-shirakura/cpp-libs)

- [ライセンス(MIT)](https://github.com/saku-shirakura/cpp-libs/blob/main/LICENSE)
