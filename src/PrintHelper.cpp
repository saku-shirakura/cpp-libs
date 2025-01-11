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

#include <net_ln3/cpp_lib/PrintHelper.h>

#include <algorithm>
#include <format>
#include <sstream>
#include <net_ln3/cpp_lib/multi_platform_util.h>

constexpr unsigned BLACK = 0x000000;
constexpr unsigned RED = 0x800000;
constexpr unsigned GREEN = 0x008000;
constexpr unsigned YELLOW = 0x808000;
constexpr unsigned BLUE = 0x000080;
constexpr unsigned MAGENTA = 0x800080;
constexpr unsigned CYAN = 0x008080;
constexpr unsigned WHITE = 0xC0C0C0;
constexpr unsigned GRAY = 0x808080;
constexpr unsigned B_RED = 0xFF00000;
constexpr unsigned B_GREEN = 0x00FF00;
constexpr unsigned B_YELLOW = 0xFFFF00;
constexpr unsigned B_BLUE = 0x0000FF;
constexpr unsigned B_MAGENTA = 0xFF00FF;
constexpr unsigned B_CYAN = 0x00FFFF;
constexpr unsigned B_WHITE = 0xFFFFFF;

std::string net_ln3::cpp_lib::PrintHelper::Color(
    const std::string& input_,
    unsigned short red_,
    unsigned short green_,
    unsigned short blue_) {
    // ANSIエスケープシーケンスに対応していない場合はそのまま出力する。
    if (!cpp_libs::multi_platform::EnableAnsiEscapeSequence::isEnabled()) {
        return input_;
    }
    if (red_ > 255) red_ = 255;
    if (green_ > 255) green_ = 255;
    if (blue_ > 255) blue_ = 255;
    return std::format("\033[38;2;{};{};{}m{}\033[39m", red_, green_, blue_, input_);
}

std::string net_ln3::cpp_lib::PrintHelper::Color(const std::string& input_, unsigned rgb_color_)
{
    if (rgb_color_ > 0xFFFFFF) { rgb_color_ = 0xFFFFFF; }
    const auto r = rgb_color_ >> 16;
    const auto g = (rgb_color_ & 0x00FF00) >> 8;
    const auto b = rgb_color_ & 0x0000FF;
    return Color(input_, r, g, b);
}

std::string net_ln3::cpp_lib::PrintHelper::Color(const std::string& input_, std::string color_)
{
    unsigned result_color = 0;
    // 小文字を大文字に置き換える
    std::ranges::transform(color_, color_.begin(), toupper);
    if (color_ == "BLACK") { result_color = BLACK; }
    else if (color_ == "RED") { result_color = RED; }
    else if (color_ == "GREEN") { result_color = GREEN; }
    else if (color_ == "YELLOW") { result_color = YELLOW; }
    else if (color_ == "BLUE") { result_color = BLUE; }
    else if (color_ == "MAGENTA") { result_color = MAGENTA; }
    else if (color_ == "CYAN") { result_color = CYAN; }
    else if (color_ == "WHITE") { result_color = WHITE; }
    else if (color_ == "GRAY") { result_color = GRAY; }
    else if (color_ == "B-RED") { result_color = B_RED; }
    else if (color_ == "B-GREEN") { result_color = B_GREEN; }
    else if (color_ == "B-YELLOW") { result_color = B_YELLOW; }
    else if (color_ == "B-BLUE") { result_color = B_BLUE; }
    else if (color_ == "B-MAGENTA") { result_color = B_MAGENTA; }
    else if (color_ == "B-CYAN") { result_color = B_CYAN; }
    else if (color_ == "B-WHITE") { result_color = B_WHITE; }
    else {
        if (color_.at(0) != '#') {
            return std::format(
                "[PrintHelper::Color] {}: 色コードが不正です。(\"{}\", {})",
                Color("error", RED), input_, color_
            );
        }
        color_.erase(0, 1);
        try {
            result_color = std::stoi(color_, nullptr, 16);
        }catch (std::exception&) {
            return std::format(
                "[PrintHelper::Color] {}: 色コードが不正です。(\"{}\", {})",
                Color("error", RED), input_, color_
            );
        }
    }
    return Color(input_, result_color);
}

