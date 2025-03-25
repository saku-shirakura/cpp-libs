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
#include <gtest/gtest.h>
#include <net_ln3/cpp_lib/multi_platform_util.h>

using namespace net_ln3::cpp_lib;

// toStringのテスト

TEST(PrintHelperColor, eq) {
    using ph = PrintHelper;
#ifdef WIN32
    // Windowsの場合、AnsiEscapeが無効であるケースを確認する。
    if (!multi_platform::EnableAnsiEscapeSequence::isEnabled()) {
        ASSERT_EQ(ph::Color("abc", "#9a1Fac"), "abc");
        ASSERT_EQ(ph::Color("Jabc", 0x9a1Fac), "Jabc");
        ASSERT_EQ(ph::Color("abc", 0xFFFFFFFF), "abc");
        ASSERT_EQ(ph::Color("abc", 0xA2, 0x3F, 0x9D), "abc");
        multi_platform::EnableAnsiEscapeSequence::enable(false, true); // 強制フラグを有効にし、強制的に許可する。
    }
#endif
    ASSERT_EQ(ph::Color("abc", "#9a1Fac"), "\033[38;2;154;31;172mabc\033[39m");
    ASSERT_EQ(ph::Color("Jabc", 0x9a1Fac), "\033[38;2;154;31;172mJabc\033[39m");
    ASSERT_EQ(ph::Color("abc", 0xFFFFFFFF), "\033[38;2;255;255;255mabc\033[39m");
    ASSERT_EQ(ph::Color("abc", 0xA2, 0x3F, 0x9D), "\033[38;2;162;63;157mabc\033[39m");
}
