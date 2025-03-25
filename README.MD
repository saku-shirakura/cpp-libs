# cpp-libs

[![test](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml/badge.svg?branch=develop&event=pull_request)](https://github.com/saku-shirakura/cpp-libs/actions/workflows/googletest.yml)

C++用の雑多なライブラリです。

以下の内容を含んでいます。

1. ArgumentParser [例](docs/examples/ArgumentParser.md)

   コマンドライン引数用のパーサーです。

2. ParseUtil [例](docs/examples/ParseUtil.md)

   パーサの実装に使えるユーティリティです。

3. PrintHelper [例](docs/examples/PrintHelper.md)
   
   シンプルなCUI用ユーティリティです。

4. CodePageGuard [例](docs/examples/CodePageGuard.md)
   
   Windows環境で、コンソール出力にUTF-8を使用するためのクラスです。他の文字コードも設定できます。

   linux環境などをターゲットにコンパイルすれば、クラスの関数やコンストラクタは全て空の関数が定義されます。

5. EnableAnsiEscapeSequence [例](docs/examples/EnableAnsiEscapeSequence.md)

   Windows環境で、コンソール出力にAnsiエスケープシーケンスを使用するためのユーティリティです。

   `EnableAnsiEscapeSequence::enable()`で使用でき、`EnableAnsiEscapeSequence::isEnabled()`で有効か否かを確認できます。

## CMakeプロジェクトにこのライブラリを含める

```cmake
include(FetchContent)
FetchContent_Declare(
        cpp-libs
        GIT_REPOSITORY https://github.com/saku-shirakura/cpp-libs.git
        GIT_TAG v0.1.3-alpha.2 # 必要に応じてバージョンをあげること。
)

FetchContent_MakeAvailable(cpp-libs)

# 以下は各自のプロジェクトの内容に合わせる。
add_executable(example example.cpp)
target_link_libraries(example cpp-libs)
```

## リンク
- [リポジトリ](https://github.com/saku-shirakura/cpp-libs)

- [ライセンス(MIT)](https://github.com/saku-shirakura/cpp-libs/blob/main/LICENSE)
