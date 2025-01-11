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
 * @file multi_platform_util.h
 * @date 25/01/11
 * @brief マルチプラットフォーム対応用ユーティリティ
 * @author saku shirakura (saku@sakushira.com)
 */


#ifndef MULTI_PLATFORM_UTIL_H
#define MULTI_PLATFORM_UTIL_H

/**
 * @brief マルチプラットフォーム対応用のクラスや関数の名前空間
 * @since v0.1.3-alpha
 */
namespace net_ln3::cpp_lib::multi_platform {
    /**
     * @brief このクラスのインスタンスをmain関数で作成することで、自動的にコードページを変更できます。
     * @details [詳細なコード例](docs/examples/CodePageGuard.md)
     * @note 現状このクラスは、Windowsのコンソールへの対応用です。
     * @since v0.1.3-alpha
     */
    class CodePageGuard {
    public:
        /**
         * @brief コンソールの出力文字列をUTF-8として設定する。
         * @details `CodePageGuard(65001)`と等価です。
         * @note Windows環境以外では何も行いません。
         * @since v0.1.3-alpha
         */
        CodePageGuard();

        /**
         * @brief コンソールの出力文字列を特定の文字コードに設定する。
         * @param target_ 変更対象のコードページ番号
         * @note Windows環境以外では何も行いません。
         * @since v0.1.3-alpha
         */
        explicit CodePageGuard(unsigned target_);

        /**
         * @brief restoreCodePage関数を呼び出すことで文字コードを実行前の状態に戻す処理が行われます。
         * @note Windows環境以外では何も行いません。
         * @since v0.1.3-alpha
         */
        ~CodePageGuard();

    private:
        // 実行前のコードページを保持しておく。
        unsigned _before_code_page = 0;
        unsigned _target_code_page = 0;

        /**
         * @brief コンソールの出力文字コードを対象の文字コード（`this->_target_code_page`）に設定します。
         * @details この関数では、「変更前の文字コードが取得できなかった場合」及び「文字コードが変更できなかった場合」に標準エラー出力に警告を印字します。
         * @note Windows環境以外では何も行いません。
         * @since v0.1.3-alpha
         */
        void _setCodePage();

        /**
         * @brief 出力文字コードを変更前の状態に設定します。
         * @details この関数では、文字コードが変更できなかった場合に標準エラー出力に警告を印字します。
         * @note Windows環境以外では何も行いません。
         * @since v0.1.3-alpha
         */
        void _restoreCodePage() const;
    };

    /**
     * @brief ANSIエスケープシーケンスを有効にするためのクラス。
     * @details [詳細なコード例](docs/examples/EnableAnsiEscapeSequence.md)
     * @note 現状このクラスはWindows環境において、コンソールでANSIエスケープを有効にするために使用されます。
     * @since v0.1.3-alpha
     */
    class EnableAnsiEscapeSequence {
    public:
        /**
         * @brief ANSIエスケープをコンソールで有効にする。
         * @param rerun 実行済みフラグに問わず、強制的に実行します。デフォルトは`false`です。
         * @return 変更後の有効・無効の状態。成功した場合は、`true`が帰ります。
         * @note Windows環境以外では常に`true`を返します。
         * @since v0.1.3-alpha
         */
        static bool enable(bool rerun = false);

        /**
         * @brief ANSIエスケープが有効か否かを返します。
         * @details この関数は、::enable()が呼ばれておらず、`_enabled`が`false`の場合には、コンソールモードを取得し、有効かどうかを確認します。
         * @return ANSIエスケープモードが有効か否か
         * @note Windows環境以外では常に`true`を返します。
         * @since v0.1.3-alpha
         */
        static bool isEnabled();

        /**
         * @brief ::enable()関数が一度でも実行されているかを確認します。
         * @return ::enable()関数が一度実行されているかどうか。
         * @note Windows環境以外では常に`true`を返します。
         * @since v0.1.3-alpha
         */
        static bool isExecuted();

    private:
        static bool _is_executed;
        static bool _enabled;
        static bool _is_mode_obtained;

        /**
         * @brief コンソールモードを取得します。
         * @details エラーが発生し、正常に取得できなかった場合は0が返ります。
         * @return 現在のコンソールモード
         * @note Windows環境以外では常に`0`を返します。
         * @since v0.1.3-alpha
         */
        static unsigned long _getConsoleMode();
    };
}

#endif //MULTI_PLATFORM_UTIL_H
