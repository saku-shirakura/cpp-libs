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

#include <gtest/gtest.h>
#include <net_ln3/cpp_lib/ArgumentParser.h>
#include <net_ln3/cpp_lib/shorthand.h>

using namespace net_ln3::cpp_lib;

TEST(OptionValueCreateInstance, valid) {
    // 文字列の受け入れ検査
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance("This is String."));
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::string("This is String.")));
    // 整数の受け入れ検査
    // 正と負の境界
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(1));
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(0));
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(-1));
    // 最大値・最小値
    // int64_tの最小値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<ull>().min()));
    // int32_tの最小値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<sll>().min()));
    // int64_tの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<ull>().max()));
    // int32_tの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<sll>().max()));
    // uint64_tの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<ull>().max()));
    // uint32_tの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<sll>().max()));
    // floatの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<float>().max()));
    // doubleの最大値
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(std::numeric_limits<double>().max()));
    // booleanの検査
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(true));
    // nullptrの検査
    ASSERT_NO_THROW(ArgumentParser::OptionValue::createInstance(nullptr));
}

TEST(OptionValueGet, validCase) {
    using ov = ArgumentParser::OptionValue;
    ASSERT_EQ(ov::createInstance("test").getString(), "test");
    ASSERT_EQ(ov::createInstance(true).getString(), "true");
    ASSERT_EQ(ov::createInstance(nullptr).getString("nullptr"), "nullptr");
    ASSERT_EQ(ov::createInstance(std::numeric_limits<sll>().max()).getString(), "9223372036854775807");
    ASSERT_EQ(ov::createInstance(std::numeric_limits<ull>().max()).getString(), "18446744073709551615");
}
