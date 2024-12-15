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

#include "shorthand.h"
#include <cinttypes>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace com_sakushira::cpp_lib {
    /**
     * @brief 型指定に対応したオプションを扱える引数パーサです。
     * @details
     * このクラスでは以下の形式の引数をパースできます。
     * - 引数: 直前が「オプション名でない場合」又は「そのオプションが真偽型のフラグである場合」はすべて引数として扱います。
     * - オプション: ハイフン2回から始まる引数と引数の組み合わせで表現されます。ex: `--option val`
     * - フラグ: フラグは真偽型のオプションを指します。たとえば、`option`がフラグの場合「`--option false`」と入力したとき、`option`の値は`true`となり, `false`は引数となります。
     * - エイリアス: ハイフン1回から始まる引数で表現されます。オプションの別名として扱われ、型を継承します。紐づけられていない場合は、`InvalidAlias`として扱われます。ex: `-o val`
     * ## 例
     * `hello --name echo --flag -c helloMode helpers -l 100`
     *
     * `-c`は`command`のエイリアス, `-l`は`length`のエイリアスです。結果は以下のようになります。
     * - 引数: `[hello, helpers]`
     * - オプション: `[{name: "echo"}, {flag: true}, {command: "helloMode"}, {length: 100}]`
     *
     * @since v0.0.1-alpha
     * @version v0.1.0-alpha オプションの型を指定できるようにしました。また、オプションの形式を変更しました。(key=val -> --key val)
     * */
    class ArgumentParser {
    public:
        /**
         * @brief OptionValueが受け入れ可能な型を表す列挙型です。
         * @since v0.1.0-alpha
         */
        enum class OptionType {
            STRING,
            SIGNED,
            UNSIGNED,
            LONG_DOUBLE,
            BOOLEAN,
            NULLITY,
            ERROR
        };

        /**
         * @brief Optionで使用可能な型の値を格納します。
         * @details
         * ## 利用可能な型
         * - 文字列(`std::string`)
         * - 符号付き整数(`int64_t`)
         * - 符号なし整数(`uint64_t`)
         * - 倍精度浮動小数点数(`double`)
         * - 真偽型(`bool`)
         * 格納時に型の互換性確認が行われます。たとえば、`float`は`double`と互換性があります。
         *
         * @since v0.1.0-alpha
         */
        class OptionValue {
        public:
            /**
             * @brief このコンストラクタは、無効な値や空の値を表現するのに使用します。
             * @return nullptrが格納されたOptionValue
             * @since v0.1.0-alpha
             */
            OptionValue();

            /**
             * @brief 使用可能な型を含むOptionValueのインスタンスを生成します。
             * @details 型確認をした後に、それが使用可能な型であればインスタンスを生成し、そうでなければ例外を送出します。
             * @details 使用可能な方は以下の通りです。
             * @details short, int, long, long long,
             * @details unsigned short(us), unsigned int, unsigned long(ul), unsigned long long(ull),
             * @details double, float, std::string, bool
             * @details floatとdouble、usとulといったような同様の機能を持つ型については、それらのうちで最大の値を扱える型に正規化されます。
             * @details 例えば、floatであればdoubleとして扱われます。usであれば、ullとして扱われます。
             * @throw invalid_argument 入力された値は使用可能な型ではありません。これは、std::vector<int>()のように、扱えない型を渡した場合に送出されます。
             * @tparam T 入力の型
             * @param value_ 格納したい値の入力
             * @return 値を登録したOptionValueのインスタンス
             * @since v0.1.0-alpha
             */
            template<class T>
            static OptionValue createInstance(T value_) {
                return OptionValue(VContainer(value_));
            }

            /**
             * @brief クラスが格納している値を文字列として取得する。
             * @details 格納している値がNullの場合は、default_を返します。
             * @details 真偽型は、value ? "true" : "false"のように、状態に応じた値を返します。
             * @details 符号付き整数、符号なし整数、浮動小数点数はstd::to_string()によって文字列に変換され、その値が返されます。
             * @param default_ 文字列に変換できない場合に使用される値 初期値は空文字列("")
             * @return 格納している文字列
             * @since v0.1.0-alpha
             */
            [[nodiscard]] std::string getString(const std::string &default_ = "") const;

            /**
             * @brief クラスが格納している値を符号付き整数として取得する。
             * @details 格納している値が符号付き整数であればその値を返し、それ以外の場合はdefault_の値を返します。
             * @param default_ 値が符号付き整数でない場合に使用される値 初期値は0
             * @return 格納している符号付き整数
             * @since v0.1.0-alpha
             */
            [[nodiscard]] sll getSigned(sll default_ = 0) const;

            /**
             * @brief クラスが格納している値を符号なし整数として取得する。
             * @details 格納している値が符号なし整数であればその値を返し、それ以外の場合はdefault_の値を返します。
             * @param default_ 値が符号なし整数でない場合に使用される値 初期値は0
             * @return 格納している符号なし整数
             * @since v0.1.0-alpha
             */
            [[nodiscard]] ull getUnsigned(ull default_ = 0) const;

            /**
             * @brief クラスが格納している値を浮動小数点数として取得する。
             * @details 格納している値が浮動小数点数であればその値を返し、それ以外の場合はdefault_の値を返します。
             * @param default_ 値が浮動小数点数でない場合に使用される値 初期値は0.0l
             * @return 格納している浮動小数点数
             * @since v0.1.0-alpha
             */
            [[nodiscard]] long double getLongDouble(long double default_ = 0.0l) const;

            /**
             * @brief クラスが格納している値を真偽型として取得する。
             * @details 格納している値が真偽型であればその値を返し、それ以外の場合はdefault_の値を返します。
             * @param default_ 値が真偽型でない場合に使用される値 初期値はfalse
             * @return 格納している真偽値
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool getBoolean(bool default_ = false) const;

            /**
             * @brief クラスが格納している値がNullであるかを確認する。
             * @return 格納している値がNullであるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isNull() const;

            /**
             * @brief クラスが格納している値が文字列であるかを確認する。
             * @return 格納している値が文字列であるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isString() const;

            /**
             * @brief クラスが格納している値が符号付き整数であるかを確認する。
             * @return 格納している値の符号付き整数であるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isSigned() const;

            /**
             * @brief クラスが格納している値が符号なし整数であるかを確認する。
             * @return 格納している値が符号なし整数であるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isUnsigned() const;

            /**
             * @brief クラスが格納している値が浮動小数点数であるかを確認する。
             * @return 格納している値が浮動小数点数であるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isLongDouble() const;

            /**
             * @brief クラスが格納している値の型が真偽型であるかを確認する。
             * @return 格納している値の型が真偽型であるかどうか
             * @since v0.1.0-alpha
             */
            [[nodiscard]] bool isBoolean() const;

        private:
            /**
             * @brief OptionValueを格納するためのコンテナクラス
             * @since v0.1.0-alpha
             */
            class VContainer {
            public:
                VContainer() = delete;

                explicit VContainer(std::nullptr_t v_);

                explicit VContainer(const char *v_);

                explicit VContainer(std::string v_);

                explicit VContainer(long double v_);

                explicit VContainer(double v_);

                explicit VContainer(bool v_);

                explicit VContainer(int v_);

                explicit VContainer(unsigned v_);

                explicit VContainer(int64_t v_);

                explicit VContainer(uint64_t v_);

                /**
                 * @brief 保有する値の型を確認します。
                 * @tparam T 保有するか確認したい型
                 * @return Tと保有する値の型が等しいかどうかの真偽値
                 */
                template<class T>
                [[nodiscard]] bool checkType() const {
                    return std::holds_alternative<T>(_value);
                }

                /**
                 * @brief 保有する値を取得します。
                 * @tparam T 取得したい型
                 * @return 型がTの保有している値
                 * @exception std::invalid_argument 取得したい型と保有している値の型に互換性がありません。
                 */
                template<class T>
                [[nodiscard]] const T &getTypeValue() const {
                    if (checkType<T>())
                        return std::get<T>(_value);
                    throw std::invalid_argument("[" + std::string(__func__) + "] template type is incompatible.");
                }

            private:
                std::variant<std::nullptr_t, std::string, long double, bool, int64_t, uint64_t> _value;
            };

            /**
             * @brief インスタンスを新しく生成する。
             * @tparam T 格納する値の型
             * @param value_ 格納する値
             * @since v0.1.0-alpha
             */
            explicit OptionValue(VContainer value_): _container(std::move(value_)) {
            }

            VContainer _container;
        };

        /**
         * @brief オプションの名称と型を紐づけるクラスです。
         * @details このクラスで登録する内容は、`--option` のようなハイフンを２回重ねた省略形以外のオプションです。
         * @since v0.1.0-alpha
         */
        class OptionNames {
        public:
            /**
             * @brief 空のマップを利用するコンストラクタ
             * @since v0.1.0-alpha
             */
            OptionNames() = default;

            /**
             * @brief マップを登録するコンストラクタ
             * @param table_ オプション名とOptionTypeの紐づけが登録されたマップ
             * @since v0.1.0-alpha
             */
            explicit OptionNames(std::unordered_map<std::string, OptionType> table_);

            /**
             * @brief オプション名に紐づいた型を取得します。
             * @details オプションの型を取得します。設定値がnullptrの場合や存在しない場合、OptionType::NULLITYを返します。
             * @param option_name_ 取得したいオプション名
             * @return オプション名に設定されている型
             * @since v0.1.0-alpha
             */
            [[nodiscard]] OptionType getOptionType(const std::string &option_name_) const;

            /**
             * @brief 対応するオプションが存在するかを確認します。
             * @param option_name_ 確認したいオプション名
             * @return オプション名が存在するかどうか
             * @since v0.1.0-alpha
             */
            bool isExistOption(const std::string &option_name_) const;

            /**
             * @brief オプションを新しく登録する。
             * @details option_name_がすでに登録済みの場合は、失敗としてfalseを返します。
             * @param option_name_ 登録したいオプションの名称
             * @param type_ 登録するオプションの型
             * @return 登録が成功したかどうか
             * @since v0.1.0-alpha
             */
            bool addOption(const std::string &option_name_, OptionType type_);

            /**
             * @brief オプションを削除する。
             * @details option_name_が登録されていない場合は、失敗としてfalseを返します。
             * @param option_name_ 削除したいオプションの名称
             * @return 削除が成功したかどうか
             * @since v0.1.0-alpha
             */
            bool removeOption(const std::string &option_name_);

        private:
            std::unordered_map<std::string, OptionType> _name_type_table;
        };

        /**
         * @brief オプションのエイリアスとオプション名を紐づけるクラスです。
         * @details このクラスで登録する内容は、-o のようにハイフンを１回最初につけた省略形のオプションです。
         * @details オプションとしては、省略形でないものと同等として扱われます。
         */
        class OptionAlias {
        public:
            /**
             * @brief 空のマップを利用するコンストラクタ
             * @since v0.1.0-alpha
             */
            OptionAlias() = default;

            /**
             * @brief マップを登録するコンストラクタ
             * @param table_ エイリアスとオプション名の紐づけが登録されたマップ
             * @since v0.1.0-alpha
             */
            explicit OptionAlias(std::unordered_map<std::string, std::string> table_);

            /**
             * @brief エイリアスに紐づいたオプション名を取得します。
             * @details オプション名を取得します。設定値が存在しない場合、空文字列を返します。
             * @param alias_name_ 取得したいエイリアス
             * @return エイリアスに設定されているオプション名
             * @since v0.1.0-alpha
             */
            [[nodiscard]] std::string getOptionName(const std::string &alias_name_) const;

            /**
             * @brief 対応するエイリアスが存在するかを確認します。
             * @param alias_name_ 確認したいエイリアス
             * @return エイリアスが存在するかどうか
             * @since v0.1.0-alpha
             */
            bool isExistAlias(const std::string &alias_name_) const;

            /**
             * @brief エイリアスを新しく登録する。
             * @details alias_name_がすでに登録済みの場合は、失敗としてfalseを返します。
             * @param alias_name_ 登録したいエイリアス
             * @param option_name_ 登録するオプション名
             * @return 登録が成功したかどうか
             * @since v0.1.0-alpha
             */
            bool addAlias(const std::string &alias_name_, const std::string &option_name_);

            /**
             * @brief エイリアスを削除する。
             * @details alias_name_が登録されていない場合は、失敗としてfalseを返します。
             * @param alias_name_ 削除したいエイリアス
             * @return 削除が成功したかどうか
             * @since v0.1.0-alpha
             */
            bool removeAlias(const std::string &alias_name_);

        private:
            std::unordered_map<std::string, std::string> _name_alias_table;
        };

        /**
         * @brief オプションの型を指定しないパーサ
         * @details `_no_option_mapper`フラグを`true`にすることで、型指定を無効にします。
         * @since v0.0.1-alpha
         * @version v0.1.0-alpha 従来の動作をこのモードで使用できるようにしました。
         */
        ArgumentParser();

        /**
         * @brief オプションの型を指定するパーサ
         * @param type_ オプションの名称と型を紐づけたデータ
         * @since v0.1.0-alpha
         */
        explicit ArgumentParser(OptionNames type_);

        /**
         * @brief オプションの型を指定し、それに対してエイリアスを設定するパーサ
         * @param type_ オプションの名称と型を紐づけたデータ
         * @param alias_ オプションの名称とその別名を紐づけたデータ
         * @since v0.1.0-alpha
         */
        ArgumentParser(OptionNames type_, OptionAlias alias_);

        /**
         * @param argc_ コマンドライン引数の数
         * @param argv_ char*型のコマンドライン引数の配列
         * @overload
         * @since v0.0.1-alpha
         */
        void parse(int argc_, char *argv_[]);

        /**
         * @brief 引数の配列をパースします。
         * @param args_
         * @details 引数の配列をパースし、オプションと引数を分けて格納します。
         * @details valueは任意です。
         * @details オプションは次の形式を指します。 `--option_name value`
         * @details エイリアスは次の形式を指します。 `-alias_name value`
         * @details それ以外は、すべて引数として扱われます。
         * @since v0.0.1-alpha
         * @version v0.1.0-alpha パースできるオプションの形式を変更しました。(`key=val -> --key val`)
         */
        void parse(const std::vector<std::string> &args_);

        /**
         * @brief 引数のリストを取得します。
         * @details 順番は、例えば`実行ファイル arg1 --option value arg2 arg3`と入力したならば、リストは、`[arg1, arg2, arg3]`とそのままの順番で返します。
         * @details オプションは別の関数で取得できます。詳しくは、getOptionを参照してください。
         * @since v0.1.0-alpha
         * */
        const std::vector<std::string> &getArgs() const;

        /**
         * @brief 引数を取得します。
         * @param i_ 0以上の整数
         * @return 0から始まるリストのi番目の要素
         * @details i番目の要素が存在しない場合は、空文字列が返されます。
         * @since v0.1.0-alpha
         */
        std::string getArg(size_t i_) const;

        /**
         * @brief オプションを取得します。
         * @param option_name_ 取得したいオプションの名称
         * @param default_ 存在しない場合に返す値
         * @return OptionValue 取得した引数の値
         * @details 存在しないオプション名を指定した場合、default_を返します。
         * @since v0.1.0-alpha
         */
        OptionValue getOption(const std::string &option_name_,
                              const OptionValue &default_ = OptionValue()) const;

        /**
         * @brief オプションが存在するか確認します。
         * @param key_ 取得したいオプションの名前
         * @return bool オプションが存在したかの結果
         * @since v0.1.0-alpha
         */
        bool isExistOption(const std::string &key_) const;

        /**
         * @brief マッピングできなかったオプションを取得します。
         * @return マッピングできなかったオプションのオプション名をキーとしたマップ
         * @since v0.1.0-alpha
         */
        const std::unordered_map<std::string, std::vector<std::string> > &getInvalidOptions() const;

        /**
         * @brief オプション名は存在するが、値が指定の型に変更不可能である不正なオプションを取得します。
         * @return オプション名をキーとした、値と正常な型のマップ
         * @since v0.1.0-alpha
         */
        const std::unordered_map<std::string, std::vector<std::pair<std::string, OptionType> > > &
        getInvalidOptionTypes() const;

        /**
         * @brief オプションと紐づけられていなかったエイリアスを取得します。
         * @return 紐づけられていなかったエイリアス名をキーとした、値とのマップ
         * @since v0.1.0-alpha
         */
        const std::unordered_map<std::string, std::vector<std::string> > &getInvalidAlias() const;

    private:
        /**
         * @brief 引数から取得できる生のオプション名を接頭辞をとったオプション名に変換します。
         * @param option_arg_ 変換したい生のオプション名
         * @return 変換後の文字列
         * @since v0.1.0-alpha
         */
        static std::string _getOptionName(const std::string &option_arg_);

        /**
         * @brief 引数から取得できる生のエイリアスを接頭辞をとったエイリアスに変換します。
         * @param alias_arg_ 変換したい生のエイリアス
         * @return 変換後の文字列
         * @since v0.1.0-alpha
         */
        static std::string _getAliasName(const std::string &alias_arg_);

        /**
         * @brief 引数がオプション名を表しているかを判定します。
         * @param option_arg_ 判定したい引数
         * @return 判定結果
         * @since v0.1.0-alpha
         */
        static bool _isOptionName(const std::string &option_arg_);

        /**
         * @brief 引数がエイリアスを表しているかを判定します。
         * @param alias_arg_ 判定したい引数
         * @return 判定結果
         * @since v0.1.0-alpha
         */
        static bool _isAliasName(const std::string &alias_arg_);

        /**
         * @brief オプションを登録します。
         * @param option_name_ 登録したいオプションの名称
         * @param value_ オプションの値
         * @since v0.1.0-alpha
         */
        void _addOption(const std::string &option_name_, OptionValue value_);

        /**
         * @brief 引数を登録します。
         * @param value_ 引数の値
         * @since v0.1.0-alpha
         */
        void _addArgument(const std::string &value_);

        /**
         * @brief 無効なオプションを登録します。
         * @param option_name_ 無効であったオプション名
         * @param value_ オプションの値
         * @since v0.1.0-alpha
         */
        void _addInvalidOption(const std::string &option_name_, const std::string &value_);

        /**
         * @brief 型が無効であったオプション名を登録します。
         * @param option_name_ 型が無効であったオプション名
         * @param value_ 型に変換できなかった文字列
         * @param type_ 期待されている型
         * @since v0.1.0-alpha
         */
        void _addInvalidOptionType(const std::string &option_name_, const std::string &value_, OptionType type_);

        /**
         * @brief オプション名と紐づけがされていなかったエイリアスを登録します。
         * @param alias_name_ 紐づけがされていなかったエイリアス名
         * @param value_ エイリアスに指定されていた値
         * @since v0.1.0-alpha
         */
        void _addInvalidAlias(const std::string &alias_name_, const std::string &value_);

        /**
         * @brief 文字列がOptionTypeに適合しているかを判定します。
         * @param value_ 判定したい文字列
         * @param type_ 判定したい対象の型
         * @return 判定結果
         * @since v0.1.0-alpha
         */
        static bool _isValidType(const std::string &value_, OptionType type_);

        /**
         * @brief 文字列をtype_に変換し、OptionValueにラップします。
         * @param value_ 対象文字列
         * @param type_ 変換対象の型
         * @return value_をOptionValueにラップした値。
         * @since v0.1.0-alpha
         */
        static OptionValue _convertOptionValue(const std::string &value_, OptionType type_);

        /// オプション以外の引数を保持します。順番を変更せずに格納されます。
        std::vector<std::string> _args;
        /// オプションのデータ
        std::unordered_map<std::string, OptionValue> _options;
        /// 無効なオプションのデータ
        std::unordered_map<std::string, std::vector<std::string> > _invalid_options;
        /// 型が無効なオプションのデータ
        std::unordered_map<std::string, std::vector<std::pair<std::string /* 対象のオプション値の文字列 */, OptionType
            /* オプションが期待している型 */> > > _invalid_option_types;
        /// 無効なエイリアスのデータ
        std::unordered_map<std::string, std::vector<std::string> > _invalid_alias;
        /// 有効なオプション名とその型
        OptionNames _valid_option_names;
        /// 有効なオプションエイリアス
        OptionAlias _valid_alias;
        /// オプションマッパーを使用せず、従来の方法で登録を行います。
        bool _option_mapper_mode;

        static const std::regex _option_name_pattern;
        static const std::regex _alias_name_pattern;
        static const std::regex _extract_option_name_pattern;
        static const std::regex _extract_alias_name_pattern;
    };
} // namespace com_sakushira::cpp_lib

#endif//DEVELOPER_TOOL_COMMANDPARSER_H
