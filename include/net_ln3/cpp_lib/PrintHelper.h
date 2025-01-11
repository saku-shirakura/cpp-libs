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
 * @file PrintHelper.h
 * @date 25/01/07
 * @author saku shirakura (saku@sakushira.com)
 * @since v0.1.2-alpha
 */


#ifndef PRINTHELPER_H
#define PRINTHELPER_H
#include <string>

namespace net_ln3::cpp_lib {
    /**
     * @brief CUI表示補助用の雑多な関数群
     * @details [詳細なコード例](docs/examples/PrintHelper.md)
     * @since v0.1.2-alpha
     * */
    class PrintHelper {
    public:
        /**
         * @brief コンソール出力に色を付ける。
         * @details 16進数6桁のRGBコードを用いて、色を指定し、それについての文字列を取得します。
         * @details 例: 白の場合 -> 0xFFFFFF
         * @param input_ 色を付けたい対象の文字列
         * @param red_ 赤(最大は255で、それを超える場合、255に補正されます。)
         * @param green_ 緑(最大は255で、それを超える場合、255に補正されます。)
         * @param blue_ 青(最大は255で、それを超える場合、255に補正されます。)
         * @return 色をANSI Escape Codeを付与し、Truecolorでの色を指定した文字列
         * @note Truecolorが使用できない場合は、正しく表示されない場合があります。
         * @note また、コンソールがANSIエスケープシーケンスに対応していない場合も正しく表示されません。(色は付けられません)
         * @note Windowsの場合は、`net_ln3::cpp_libs::multi_platform::EnableAnsiEscapeSequence::enable()`によって初期化することで有効化できます。
         * @note ただし、この方法についても"windows 10"未満のバージョンでは対応していないため色は付与されません。
         * @since v0.1.2-alpha
         */
        static std::string Color(const std::string& input_, unsigned short red_, unsigned short green_, unsigned short blue_);

        /**
         * @brief コンソール出力に色を付ける。
         * @details 16進数6桁のRGBコードを用いて、色を指定し、それについての文字列を取得します。
         * @details 例: 白の場合 -> 0xFFFFFF
         * @param input_ 色を付けたい対象の文字列
         * @param rgb_color_ 色コード
         * @returns 色をANSI Escape Codeを付与し、Truecolorでの色を指定した文字列
         * @returns 本質的には、rgb_color_をr, g, bに分割し、Color(input_, r, g, b)を返す関数です。
         * @note Truecolorが使用できない場合は、正しく表示されない場合があります。
         * @note また、コンソールがANSIエスケープシーケンスに対応していない場合も正しく表示されません。(色は付けられません)
         * @note Windowsの場合は、`net_ln3::cpp_libs::multi_platform::EnableAnsiEscapeSequence::enable()`によって初期化することで有効化できます。
         * @note ただし、この方法についても"windows 10"未満のバージョンでは対応していないため色は付与されません。
         * @since v0.1.2-alpha
         */
        static std::string Color(const std::string& input_, unsigned rgb_color_);

        /**
         * @brief コンソール出力に色を付ける。
         * @details 16進数6桁のRGBコードを用いて、色を指定し、それについての文字列を取得します。
         * @details 例: 白の場合 -> "#FFFFFF"
         * @details 例: 色名の場合 -> "white"
         * @param input_ 色を付けたい対象の文字列
         * @param color_ 色コード(`#rrggbb`)、または、表の色名のいずれかを指定します。
         * |色名|
         * |--|
         * |black|
         * |red|
         * |green|
         * |yellow|
         * |blue|
         * |magenta|
         * |cyan|
         * |white|
         * |gray|
         * |b-red|
         * |b-green|
         * |b-yellow|
         * |b-blue|
         * |b-magenta|
         * |b-cyan|
         * |b-white|
         * @returns `color_`を`unsigned`に変換したものをconvertedとしたとき、戻り値は以下のようになります。
         * @returns `return Color(input_, converted);`
         * @returns 正しく指定されていない場合は、"[PrintHelper::Color] error: 色コードが不正です。({input_}, {color_})"と返します。
         * @note Truecolorが使用できない場合は、正しく表示されない場合があります。
         * @note また、コンソールがANSIエスケープシーケンスに対応していない場合も正しく表示されません。(色は付けられません)
         * @note Windowsの場合は、`net_ln3::cpp_libs::multi_platform::EnableAnsiEscapeSequence::enable()`によって初期化することで有効化できます。
         * @note ただし、この方法についても"windows 10"未満のバージョンでは対応していないため色は付与されません。
         * @since v0.1.2-alpha
         */
        static std::string Color(const std::string& input_, std::string color_);
    };
}

#endif //PRINTHELPER_H
