#include <gtest/gtest.h>
#include <ParseUtil.h>

using namespace com_sakushira::cpp_lib;

// toStringのテスト

TEST(ParseUtilToString, eq) {
    ASSERT_EQ(ParseUtil::toString({'a', 'b', 'c', 'd', 'e', 'f', 'g'}), "abcdefg");
}

TEST(ParseUtilToString, boundary) {
    ASSERT_EQ(ParseUtil::toString({}), "");
}

// toArrayのテスト

TEST(ParseUtilToArray, eq) {
    std::vector<char> valid{'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    ASSERT_EQ(ParseUtil::toArray("abcdefg"), valid);
}

TEST(ParseUtilToArray, boundary) {
    std::vector<char> valid{};
    ASSERT_EQ(ParseUtil::toArray(""), valid);
}

// toString - toArrayの相互変換テスト

TEST(ParseUtilMutalToArrayToString, ArrayTo) {
    std::vector<char> test{'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    ASSERT_EQ(test, ParseUtil::toArray(ParseUtil::toString(test)));
}

TEST(ParseUtilMutalToArrayToString, StringTo) {
    std::string test = "abcdefg";
    ASSERT_EQ(test, ParseUtil::toString(ParseUtil::toArray(test)));
}

// sliceのテスト
// beg < end: 1とします。
// beg > end: 2とします。
// beg = end: 3とします。
// 同値テスト
TEST(ParseUtilSlice, eq1) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 2, 3));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 2, 3), "cd");
}

TEST(ParseUtilSlice, eq2) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 3, 0));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 3, 0), "defg");
}

TEST(ParseUtilSlice, eq3) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 4, 4));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 4, 4), "abcde");
}

// 境界値テスト
TEST(ParseUtilSlice, boundary1Max) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 4, 6));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 4, 6), "efg");
}

TEST(ParseUtilSlice, boundary1Min) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 0, 4));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 0, 4), "abcde");
}

TEST(ParseUtilSlice, boundary1MinMax) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 0, 6));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 0, 6), "abcdefg");
}

TEST(ParseUtilSlice, boundary1OverMax) {
    ASSERT_THROW(ParseUtil::slice("abcdefg", 0, 7), ParseUtilOutOfRange);
}

TEST(ParseUtilSlice, boundary2Max) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 6, 0));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 6, 0), "g");
}

TEST(ParseUtilSlice, boundary2Min) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 1, 0));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 1, 0), "bcdefg");
}

TEST(ParseUtilSlice, boundary2OverMax) {
    ASSERT_THROW(ParseUtil::slice("abcdefg", 7, 0), ParseUtilOutOfRange);
}

TEST(ParseUtilSlice, boundary3Max) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 6, 6));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 6, 6), "abcdefg");
}

TEST(ParseUtilSlice, boundary3Min) {
    ASSERT_NO_THROW(ParseUtil::slice("abcdefg", 0, 0));
    ASSERT_EQ(ParseUtil::slice("abcdefg", 0, 0), "a");
}

TEST(ParseUtilSlice, boundary3OverMax) {
    ASSERT_THROW(ParseUtil::slice("abcdefg", 7, 7), ParseUtilOutOfRange);
}

// splitのテスト

TEST(ParseUtilSplit, eq1) {
    std::vector<std::string> test{"alpha", "beta", "gamma"};
    ASSERT_NO_THROW(ParseUtil::split("alpha beta gamma"));
    ASSERT_EQ(ParseUtil::split("alpha beta gamma"), test);
}

TEST(ParseUtilSplit, eq2) {
    std::vector<std::string> test{"", "hello", "my", "name", "is", "test", "case!", ""};
    ASSERT_NO_THROW(ParseUtil::split("eqhelloeqmyeqnameeqiseqtesteqcase!eq", "eq"));
    ASSERT_EQ(ParseUtil::split("eqhelloeqmyeqnameeqiseqtesteqcase!eq", "eq"), test);
}

TEST(ParseUtilSplit, eq3) {
    std::vector<std::string> test{"", "hello", "my", "name", "is", "test", "case!", ""};
    ASSERT_NO_THROW(ParseUtil::split("whalehellowhalemywhalenamewhaleiswhaletestwhalecase!whale", "whale"));
    ASSERT_EQ(ParseUtil::split("whalehellowhalemywhalenamewhaleiswhaletestwhalecase!whale", "whale"), test);
}

TEST(ParseUtilSplit, boundary1) {
    std::vector<std::string> test{"a", "b", "c", "d", "e", "f", "g"};
    ASSERT_NO_THROW(ParseUtil::split("abcdefg", ""));
    ASSERT_EQ(ParseUtil::split("abcdefg", ""), test);
}

TEST(ParseUtilSplit, boundary2) {
    std::vector<std::string> test{""};
    ASSERT_NO_THROW(ParseUtil::split("", " "));
    ASSERT_EQ(ParseUtil::split("", " "), test);
}

TEST(ParseUtilSplit, boundary3) {
    std::vector<std::string> test{"test"};
    ASSERT_NO_THROW(ParseUtil::split("test", ","));
    ASSERT_EQ(ParseUtil::split("test", ","), test);
}

TEST(ParseUtilAppendAll, eq1) {
    std::vector<std::string> test{"test", "abc", "hello", "world", "say"};
    ASSERT_EQ(ParseUtil::appendAll(test, ", at ,"), "test, at ,abc, at ,hello, at ,world, at ,say");
}

TEST(ParseUtilAppendAll, eq2) {
    std::vector<std::string> test{"hello,", "world!"};
    ASSERT_EQ(ParseUtil::appendAll(test), "hello,world!");
}

TEST(ParseUtilAppendAll, eq3) {
    std::vector<std::string> test{"a5a", "","", "a5a"};
    ASSERT_EQ(ParseUtil::appendAll(test, "f"), "a5afffa5a");
}

TEST(ParseUtilAppendAll, boundary1) {
    std::vector<std::string> test{"hello,"};
    ASSERT_EQ(ParseUtil::appendAll(test, "abc"), "hello,");
}

TEST(ParseUtilAppendAll, boundary2) {
    std::vector<std::string> test{};
    ASSERT_EQ(ParseUtil::appendAll(test, "abc"), "");
}
