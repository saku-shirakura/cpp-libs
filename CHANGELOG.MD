# 更新履歴

## [未発表]

### 予定バージョン

v0.1.0-alpha

### 破壊的な変更

- includeディレクトリの構成を変更。

    ヘッダファイルを、`com_sakushira_cpp_lib`ディレクトリに移動しました。

    この変更により、従来のコードはヘッダファイルへのパスを修正しない限りコンパイルできません。

### 新機能

- クラス ParseUtil::StringValidatorを追加。このクラスは、文字列が正常な形式かを判定します。
  - isValidSigned(): 文字列が符号付き整数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
  - isValidUnsigned(): 文字列が符号無し整数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
  - isValidDouble(): 文字列が倍制度浮動小数点数として正しいかを判定し、正しい場合、それが正常な範囲内かを判定します。
  - isValidBoolean(): 文字列がtrueまたはfalseに一致するかを判定します。この関数では、大文字・小文字の違いは無視されます。
- ParseUtilに新たな関数appendAllを追加しました。
  - 与えられた文字列の配列を結合し、その文字列ごとの間にglueを挿入します。
- long longやunsigned shortのように、定義が長い型を省略するエイリアスを追加しました。

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
