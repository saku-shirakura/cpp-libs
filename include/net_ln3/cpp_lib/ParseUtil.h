// MIT License
//
// Copyright (c) 2024 Saku Shirakura <saku@sakushira.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/**
 * @file ParseUtil.h
 * @date 24/12/03
 * @author saku shirakura (saku@sakushira.com)
 * @since v0.0.1-alpha
 */

#ifndef NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H
#define NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <regex>
#include <numeric>
#include <set>

namespace net_ln3::cpp_lib {
    /**
     * @brief ParseUtilで範囲外の値を指定した場合に送出される例外
     * @since v0.0.1-alpha
     * */
    class ParseUtilOutOfRange final : std::out_of_range {
    public:
        explicit ParseUtilOutOfRange(const std::string& message) : std::out_of_range(message) {
        }
    };

    /**
     * @brief パーサの実装に便利なユーティリティクラス
     * @details このクラスのメンバ関数はすべて静的関数です。
     * @details このクラスはインスタンス化できません。
     * @since v0.0.1-alpha
     * */
    class ParseUtil {
    public:
        ParseUtil() = delete;

        /**
         * @brief 範囲を指定して部分配列を取得する。
         * @param array 対象となる配列
         * @param beg 開始位置
         * @param end 終了位置
         * @returns beg < end: begから始まりendで終わる範囲の部分配列
         * @returns beg > end: beg以降のすべての要素からなる部分配列
         * @returns beg = end: end以前のすべての要素からなる部分配列
         * @throws ParseUtilOutOfRange begまたはendのいずれかがarrayの範囲を超過しています。
         * @since v0.0.1-alpha
         */

        template <class T>
        [[maybe_unused]] static std::vector<T> slice(std::vector<T> array, size_t beg, size_t end);

        /**
         * @brief 文字列を文字配列に変換する。
         * @param str 変換対象の文字列
         * @return vector<char>に変換されたstr
         * @details 入力文字列をaとしたとき、「toString(toArray(a)) == a」となります。
         * @since v0.0.1-alpha
         * */
        [[maybe_unused]] static std::vector<char> toArray(std::string str);

        /**
         * @brief 文字配列を文字列に変換する。
         * @param array 変換対象の配列
         * @return stringに変換されたarray
         * @details 入力配列をaとしたとき、「toArray(toString(a)) == a」となります。
         * @since v0.0.1-alpha
         * */
        [[maybe_unused]] static std::string toString(std::vector<char> array);

        /**
         * @brief 範囲を指定して部分文字列を取得する。
         * @details この関数は、文字列をtoArrayで変換し、その文字配列に対するsliceを使用してます。
         * @param str 対象となる文字列
         * @param beg 開始位置
         * @param end 終了位置
         * @returns beg < end: begから始まりendで終わる範囲の部分文字列
         * @returns beg > end: beg以降のすべての文字からなる部分文字列
         * @returns beg = end: end以前のすべての文字からなる部分文字列
         * @throws ParseUtilOutOfRange begまたはendのいずれかがstrの範囲を超過しています。
         * @since v0.0.1-alpha
         */

        [[maybe_unused]] static std::string slice(std::string str, size_t beg, size_t end);

        /**
         * @brief delimを基準に分割し、結果の配列を返す。
         * @details 出力にdelimは含まれません。
         * @details delimを基準に文字が存在するかを問わず配列化するため、以下のような入力になります。
         * @details delimは「,」とする。",a,,b,"を入力した場合{"", "a", "", "b", ""}となる。
         * @details strに空文字列を与えた場合、空文字列のみを含む配列を返します。
         * @details これは入力(str)にdelimが含まれない場合の動作に等しいです。
         * @param str 分割対象の文字列
         * @param delim 分割の基準となる文字列
         * @returns delim.empty(): 1文字単位で分割され、文字列に変換された配列
         * @returns else: delimを基準に分割された文字列の配列
         * @since v0.0.1-alpha
         */
        [[maybe_unused]] static std::vector<std::string> split(const std::string& str, const std::string& delim = " ");

        /**
         * @brief inputの値をすべて連結し、結果を返します。
         * @param input 入力配列
         * @param glue 連結時に要素間に挿入する文字列
         * @return 連結した結果
         * @since v0.1.0-alpha
         */
        [[maybe_unused]] static std::string appendAll(
            const std::vector<std::string>& input,
            const std::string& glue = ""
        );

        /**
         * @brief 文字列が、整数や真偽型に変換可能かどうかを判定します。
         * @details このクラスのメンバ関数はすべて静的関数です。
         * @details このクラスはインスタンス化できません。
         * @since v0.1.0-alpha
         * */
        class StringValidator {
        public:
            StringValidator() = delete;

            /**
             * @brief 入力がlong long型に変換可能かを判定します。
             * @param str_ 符号付き整数を表した文字列
             * @return 判定結果
             * @since v0.1.0-alpha
             * @throws std::regex_error メモリが不足している場合に投げられます。詳細は`std::regex_match`のドキュメントを確認してください。
             */
            [[maybe_unused]] static bool isValidSigned(const std::string& str_);

            /**
             * @brief 入力がunsigned long long型に変換可能かを判定します。
             * @param str_ 符号なし整数を表した文字列
             * @return 判定結果
             * @since v0.1.0-alpha
             * @throws std::regex_error メモリが不足している場合に投げられます。詳細は`std::regex_match`のドキュメントを確認してください。
             */
            [[maybe_unused]] static bool isValidUnsigned(const std::string& str_);

            /**
             * @brief 入力が`long double`型に変換可能かを判定します。
             * @param str_ 倍精度浮動小数点数を表した文字列
             * @return 判定結果
             * @since v0.1.0-alpha
             * @throws std::regex_error メモリが不足している場合に投げられます。詳細は`std::regex_match`のドキュメントを確認してください。
             */
            [[maybe_unused]] static bool isValidLongDouble(const std::string& str_);

            /**
             * @brief 入力が`double`型に変換可能かを判定します。
             * @param str_ 倍精度浮動小数点数を表した文字列
             * @return 判定結果
             * @since v0.1.0-alpha
             * @throws std::regex_error メモリが不足している場合に投げられます。詳細は`std::regex_match`のドキュメントを確認してください。
             */
            [[maybe_unused]] static bool isValidDouble(const std::string& str_);

            /**
             * @brief 入力が真偽型を表現しているかを判定します。
             * @param str_ 真偽を表した文字列(trueまたはfalse, 文字の大小は無視されます。[ignore case])
             * @return 判定結果
             * @since v0.1.0-alpha
             * @throws std::regex_error メモリが不足している場合に投げられます。詳細は`std::regex_match`のドキュメントを確認してください。
             */
            [[maybe_unused]] static bool isValidBoolean(const std::string& str_);

        private:
            /**
             * @brief 判定器用のヘルパー関数
             * @param str_ 判定対象の文字列
             * @param _error_checker 変換不可の場合に例外を送出するか、それについての真偽型を返す関数。例外でエラーを示す場合は、return true;としてください。
             * @return 判定結果
             * @since v0.1.0-alpha
             */
            static bool _validateHelper(const std::string& str_,
                                        const std::function<bool(const std::string&)>& _error_checker) noexcept;

            const static std::regex _signedPattern;
            const static std::regex _unsignedPattern;
            const static std::regex _longDoublePattern;
            const static std::regex _doublePattern;
            const static std::regex _booleanPattern;
        };
    };
}

#endif //NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H
