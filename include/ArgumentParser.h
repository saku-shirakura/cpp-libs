/**
 * @file ArgumentParser.h
 * @date 24/07/10
 * @brief コマンドライン引数をパースする
 * @details コマンドライン引数をもとに、名前付き引数と無名引数にパースします。
 * @author saku shirakura (saku@sakushira.com)
 */

/*
MIT License

Copyright (c) saku shirakura <saku@sakushira.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef DEVELOPER_TOOL_COMMANDPARSER_H
#define DEVELOPER_TOOL_COMMANDPARSER_H


#include <vector>
#include <string>
#include <unordered_map>

namespace com_sakushira::cpp_lib {
    class ArgumentParser {
    public:
        ArgumentParser();

        /**
         * @param argc コマンドライン引数の数
         * @param argv char*型のコマンドライン引数の配列
         * @overload
         */
        void parse(int argc, char *argv[]);

        /**
         * @brief 引数の配列をパースします。
         * @param args_
         * @details 引数の配列をパースし、名前付き引数と名前がない引数を分けて格納します。
         * @details 名前付き引数: &lt;name&gt;=&lt;value&gt;
         * @details パース後: std::map&lt;std::string, std::string&gt; named_arg{{&lt;name&gt;, &lt;value&gt;}}
         * @details 名前なし引数: &lt;value1&gt; &lt;value2&gt; &lt;value3&gt; ...
         * @details パース後: std::array&lt;std::string&gt; no-named_arg{ &lt;value1&gt;, &lt;value2&gt;, &lt;value3&gt;, ...}
         */
        void parse(const std::vector<std::string> &args_);

        /**
         * @brief 名前なし引数のリストを取得します。
         * @details 順番は、例えば`実行ファイル arg1 named=value arg2 arg3`と入力したならば、リストは、arg1, arg2, arg3とそのままの順番で返します。
         * @details 名前付き引数は別の関数で取得できます。詳しくは、get_named_argを参照してください。
         * */
        const std::vector<std::string> &get_args() const;


        /**
         * @brief 名前なし引数を取得します。
         * @param i 0以上の整数
         * @return 0から始まるリストのi番目の要素
         * @details i番目の要素が存在しない場合は、空文字列が返されます。
         */
        std::string get_arg(size_t i) const;

        /**
         * @brief 名前付き引数を取得します。
         * @param key 取得したい引数の名前
         * @return std::string 取得した引数の値
         * @details この関数では、存在しない引数名を指定した場合、空文字列を返します。
         */
        std::string get_named_arg(const std::string &key) const;

        /**
         * @brief 名前付き引数が存在するか確認します。
         * @param key 取得したい引数の名前
         * @return bool 名前付き引数が存在したかの結果
         */
        bool is_exist_named_arg(const std::string &key) const;

        /**
         * @brief 名前付き引数の値をint64_t取得します。
         * @param key 取得したい引数の名前
         * @param default_value エラーが発生した場合に使用される値
         * @return int64_t 名前付き引数の変換後の値
         * @details 名前付き引数を変換します。
         * @details 例外が捕捉された場合はdefault_valueを返します。
         * */
        int64_t get_named_arg_int64(const std::string &key, int64_t default_value) const noexcept;

        /**
         * @brief 名前付き引数の値をint64_t取得します。
         * @param key 取得したい引数の名前
         * @exception std::invalid_argument 変換に失敗しました。
         * @exception std::runtime_error 指定された引数の値は数値ではありませんでした。
         * @exception std::out_of_range 値は範囲外です。
         * @return int64_t 名前付き引数の変換後の値
         * */
        int64_t get_named_arg_int64(const std::string &key) const;

    private:
        std::vector<std::string> _args;
        std::unordered_map<std::string, std::string> _named_arg;
    };
}


#endif //DEVELOPER_TOOL_COMMANDPARSER_H
