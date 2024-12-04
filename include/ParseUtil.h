/**
 * @file
 * @date 24/12/03
 * @brief ファイルの説明
 * @details ファイルの詳細
 * @author saku shirakura (saku@sakushira.com)
 */


#ifndef NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H
#define NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <regex>
#include <numeric>

namespace com_sakushira::cpp_lib {
    class ParseUtilOutOfRange : std::out_of_range {
    public:
        explicit ParseUtilOutOfRange(const std::string &message) : std::out_of_range(message) {}
    };

/**
 * @brief パーサの実装に便利なユーティリティクラス
 * @details このクラスのメンバ関数はすべて静的関数です。
 * @details このクラスはインスタンス化できません。
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
         */

        template<class T>
        [[maybe_unused]] static std::vector<T> slice(std::vector<T> array, size_t beg, size_t end);

        /**
         * @brief 文字列を文字配列に変換する。
         * @param str 変換対象の文字列
         * @return vector<char>に変換されたstr
         * @details 入力文字列をaとしたとき、「toString(toArray(a)) == a」となります。
         * */
        [[maybe_unused]] static std::vector<char> toArray(std::string str);

        /**
         * @brief 文字配列を文字列に変換する。
         * @param array 変換対象の配列
         * @return stringに変換されたarray
         * @details 入力配列をaとしたとき、「toArray(toString(a)) == a」となります。
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
         * @returns else: delimを基準に分割されたた文字列の配列
         */
        [[maybe_unused]] static std::vector<std::string> split(const std::string &str, const std::string &delim = " ");

    };
}

#endif //NAND2TETRIS_C_LANGUAGE_PARSEUTIL_H
