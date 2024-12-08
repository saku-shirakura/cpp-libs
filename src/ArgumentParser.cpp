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

#include <com_sakushira_cpp_lib/ArgumentParser.h>
#include <regex>

void com_sakushira::cpp_lib::ArgumentParser::parse(const std::vector<std::string> &args_) {
    // 引数の種別が名前付き引数かどうかのフラグ。
    bool named_arg_flag = false;
    for (size_t i = 1; i < args_.size(); i++) {
        const auto &current_arg = args_.at(i);
        size_t equal_pos = current_arg.find('=');
        // イコールが存在する場合は、名前付き引数として扱う。
        if (equal_pos != std::string::npos) {
            named_arg_flag = true;
        }
        // 名前付き引数をパースする。
        if (named_arg_flag) {
            std::string key, value;
            // 一回目に出現したイコールを基準にkeyとvalueにパースする。
            // valueにのみイコールを含むことが可能です。
            key = current_arg.substr(0, equal_pos);
            value = current_arg.substr(equal_pos + 1);
            // keyが空なら、登録しない。
            if (key.empty()) {
                break;
            }
            // valueが空なら、登録しない。
            if (value.empty()) {
                break;
            }
            // mapに格納する。
            // 名前付き引数(任意)
            _named_arg.try_emplace(key, value);
        } else {
            // フラグ引数を格納する。
            if (std::regex_match(current_arg, std::regex("(^-[^-]$)|(^--[^-].+$)"))) {
                _named_arg.try_emplace(current_arg, "true");
            } else {
                // イコールを含まない引数を格納する。
                // 引数(必須)
                _args.emplace_back(current_arg);
            }
        }
    }
}

com_sakushira::cpp_lib::ArgumentParser::ArgumentParser() : _args(), _named_arg() {}

const std::vector<std::string> &com_sakushira::cpp_lib::ArgumentParser::get_args() const {
    return _args;
}

std::string com_sakushira::cpp_lib::ArgumentParser::get_arg(size_t i) const {
    if (i < _args.size()) {
        return _args.at(i);
    }
    return "";
}

std::string com_sakushira::cpp_lib::ArgumentParser::get_named_arg(const std::string &key) const {
    if (_named_arg.contains(key)) {
        return _named_arg.at(key);
    }
    return "";
}

bool com_sakushira::cpp_lib::ArgumentParser::is_exist_named_arg(const std::string &key) const {
    return _named_arg.contains(key);
}

int64_t com_sakushira::cpp_lib::ArgumentParser::get_named_arg_int64(const std::string &key) const {
    auto val = this->get_named_arg(key);
    if (!std::regex_match(val, std::regex(R"(-?(0|[1-9][0-9]*))"))) {
        throw std::runtime_error("Argument value is not number.");
    }
    return std::stoll(val);
}

int64_t
com_sakushira::cpp_lib::ArgumentParser::get_named_arg_int64(const std::string &key,
                                                            int64_t default_value) const noexcept {
    try {
        return get_named_arg_int64(key);
    } catch (std::exception &e) {
        return default_value;
    }
}

void com_sakushira::cpp_lib::ArgumentParser::parse(int argc, char **argv) {
    std::vector<std::string> args(argv, argv + argc);
    this->parse(args);
}
