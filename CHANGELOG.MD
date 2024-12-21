# 更新履歴

## [未発表]

## [v0.1.1-alpha] - 2024-12-20

### お知らせ

このバージョンより、コミットメッセージの接頭辞AddをFeatureに変更します。

### 新機能

- クラスOptionValueに明示的な型変換演算子を追加しました。

### 変更点

-

shorthand.hがヘッダファイルに含まれていたため、それを取り除きました。これは、利用時に発生する予期せぬ不具合を減らすためです。(
グローバル名前空間の汚染)
- 対象のヘッダファイル: ArgumentParser.h

## [v0.1.0-alpha.1] - 2024-12-17

### 破壊的な変更

以下の変更は、v0.1.0-alphaに含め忘れたものです。

- includeディレクトリの構成を変更。

  ヘッダファイルを、`net_ln3/cpp_lib`ディレクトリに移動しました。
- 名前空間を`com_sakushira`から`net_ln3`に変更しました。

## [v0.1.0-alpha] - 2024-12-17

### 破壊的な変更

- includeディレクトリの構成を変更。

  ヘッダファイルを、`com_sakushira_cpp_lib`ディレクトリに移動しました。

  この変更により、従来のコードはヘッダファイルへのパスを修正しない限りコンパイルできません。

- クラスArgumentParserを変更。オプションに型を指定できるようにし、旧名前付き引数をオプションという名称に統一しました。
    - オプションの表現方法を次の通り変更しました。

      従来: `option=value`, 変更後: `--option value`

### 新機能

- クラス ParseUtil::StringValidatorを追加。このクラスは、文字列が正常な形式かを判定します。
    - isValidSigned(): 文字列が符号付き整数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
    - isValidUnsigned(): 文字列が符号無し整数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
    - isValidDouble(): 文字列が倍制度浮動小数点数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
    - isValidLongDouble(): 文字列が倍制度浮動小数点数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
    - isValidBoolean(): 文字列がtrueまたはfalseに一致するかを判定します。この関数では、大文字・小文字の違いは無視されます。
- ParseUtilに新たな関数appendAllを追加しました。
    - 与えられた文字列の配列を結合し、その文字列ごとの間にglueを挿入します。
- long longやunsigned shortのように、定義が長い型を省略するエイリアスを追加しました。
- 列挙型OptionTypeを追加。これは、オプションの型を指定するために使用します。
- クラスOptionValueを追加。これは、オプションがとりえる型をまとめ、文字列や真偽型として取得するために使用します。
- クラスOptionNameを追加。これは、オプションの型を指定する場合に、その名称と型を紐づけるために使用します。
- クラスOptionAliasを追加。これは、オプションの名称とその別名を紐づけるために使用します。

## [v0.0.1-alpha] - 2024-12-05

### 新機能

- ParseUtilクラス
    - 実装された関数の詳細は、
      [ドキュメント](https://docs.sakushira.com/cpp-libs/classcom__sakushira_1_1cpp__lib_1_1ParseUtil.html)
      を参照してください。
- ArgumentParserクラス
    - 実装された機能の詳細は、
      [ドキュメント](https://docs.sakushira.com/cpp-libs/classcom__sakushira_1_1cpp__lib_1_1ArgumentParser.html)
      を参照してください。
