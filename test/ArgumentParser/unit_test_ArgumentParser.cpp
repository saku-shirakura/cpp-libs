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
#include <net_ln3/cpp_lib/ParseUtil.h>

using namespace net_ln3::cpp_lib;

TEST(ArgumentParserParse, validNoMap) {
        ArgumentParser parser;
        const std::string command_line = "help this --value 4321 -v just-fit -v test --as--s test as--d";
        parser.parse(ParseUtil::split(command_line));
        ASSERT_EQ(parser.getOption("value").getString(), "4321");
        ASSERT_EQ(parser.getOption("as--s").getString(), "test");
        ASSERT_EQ(parser.getArgs(), std::vector<std::string>({"help", "this", "as--d"}));
        const std::unordered_map<std::string, std::vector<std::string>> correctInvalidArgs(
                {{"v", {"just-fit", "test"}}}
        );
        ASSERT_EQ(parser.getInvalidAlias(), correctInvalidArgs);
}

TEST(ArgumentParserParse, validMap) {
        ArgumentParser parser(ArgumentParser::OptionNames(
                                      {
                                              {"value", ArgumentParser::OptionType::UNSIGNED},
                                              {"invalid", ArgumentParser::OptionType::UNSIGNED},
                                              {"help", ArgumentParser::OptionType::BOOLEAN},
                                              {"name", ArgumentParser::OptionType::STRING},
                                              {"type", ArgumentParser::OptionType::SIGNED},
                                              {"decimal", ArgumentParser::OptionType::LONG_DOUBLE}
                                      }), {});
        const std::string command_line =
                "help this --value 4321 --help --name test --invalid 0.03 --type -500 decimal --decimal 0.25 --name faster --post poster list -n faster";
        parser.parse(ParseUtil::split(command_line));
        ASSERT_EQ(parser.getOption("value").getUnsigned(), 4321);
        ASSERT_TRUE(parser.getOption("help").getBoolean());
        ASSERT_EQ(parser.getOption("name").getString(), "test");
        ASSERT_EQ(parser.getOption("type").getSigned(), -500);
        ASSERT_EQ(parser.getOption("decimal").getLongDouble(), 0.25);
        ASSERT_EQ(parser.getArgs(), std::vector<std::string>({"help", "this", "decimal", "list"}));
        const std::unordered_map<std::string, std::vector<std::string>> correctInvalidArgs(
                {
                        {"name", {"faster"}},
                        {"post", {"poster"}}
                }
        );
        ASSERT_EQ(parser.getInvalidOptions(), correctInvalidArgs);
        const std::unordered_map<std::string, std::vector<std::pair<std::string, ArgumentParser::OptionType>>>
                correctInvalidTypeArgs(
                        {
                                {"invalid", {{"0.03", ArgumentParser::OptionType::UNSIGNED}}}
                        }
                );
        ASSERT_EQ(parser.getInvalidOptionTypes(), correctInvalidTypeArgs);
        const std::unordered_map<std::string, std::vector<std::string>> correctInvalidAliasArgs(
                {
                        {"n", {"faster"}}
                }
        );
        ASSERT_EQ(parser.getInvalidAlias(), correctInvalidAliasArgs);
}

TEST(ArgumentParserParse, validMapAlias) {
        ArgumentParser parser(ArgumentParser::OptionNames(
                                      {
                                              {"value", ArgumentParser::OptionType::UNSIGNED},
                                              {"invalid", ArgumentParser::OptionType::UNSIGNED},
                                              {"help", ArgumentParser::OptionType::BOOLEAN},
                                              {"name", ArgumentParser::OptionType::STRING},
                                              {"type", ArgumentParser::OptionType::SIGNED},
                                              {"decimal", ArgumentParser::OptionType::LONG_DOUBLE}
                                      }),
                              ArgumentParser::OptionAlias({{"?", "help"}, {"t", "type"}}));
        const std::string command_line =
                "help this --value 4321 -? --name test --invalid 0.03 -t -500 decimal --decimal 0.25 --name faster --post poster list -n faster";
        parser.parse(ParseUtil::split(command_line));
        ASSERT_EQ(parser.getOption("value").getUnsigned(), 4321);
        ASSERT_TRUE(parser.getOption("help").getBoolean());
        ASSERT_EQ(parser.getOption("name").getString(), "test");
        ASSERT_EQ(parser.getOption("type").getSigned(), -500);
        ASSERT_EQ(parser.getOption("decimal").getLongDouble(), 0.25);
        ASSERT_EQ(parser.getArgs(), std::vector<std::string>({"help", "this", "decimal", "list"}));
        const std::unordered_map<std::string, std::vector<std::string>> correctInvalidArgs(
                {
                        {"name", {"faster"}},
                        {"post", {"poster"}}
                }
        );
        ASSERT_EQ(parser.getInvalidOptions(), correctInvalidArgs);
        const std::unordered_map<std::string, std::vector<std::pair<std::string, ArgumentParser::OptionType>>>
                correctInvalidTypeArgs(
                        {
                                {"invalid", {{"0.03", ArgumentParser::OptionType::UNSIGNED}}}
                        }
                );
        ASSERT_EQ(parser.getInvalidOptionTypes(), correctInvalidTypeArgs);
        const std::unordered_map<std::string, std::vector<std::string>> correctInvalidAliasArgs(
                {
                        {"n", {"faster"}}
                }
        );
        ASSERT_EQ(parser.getInvalidAlias(), correctInvalidAliasArgs);
}
