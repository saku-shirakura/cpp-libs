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

#include <net_ln3/cpp_lib/ArgumentParser.h>
#include <net_ln3/cpp_lib/shorthand.h>
#include <regex>
#include <utility>
#include <net_ln3/cpp_lib/ParseUtil.h>

net_ln3::cpp_lib::ArgumentParser::OptionValue::OptionValue() : OptionValue(createInstance(nullptr)) {
}

std::string net_ln3::cpp_lib::ArgumentParser::OptionValue::getString(const std::string& default_) const {
    if (isNull()) { return default_; }
    if (isString()) { return _container.getTypeValue<std::string>(); }
    if (isBoolean()) { return getBoolean() ? "true" : "false"; }
    if (isSigned()) { return std::to_string(getSigned()); }
    if (isLongDouble()) { return std::to_string(getLongDouble()); }
    if (isUnsigned()) { return std::to_string(getUnsigned()); }
    return default_;
}

sll net_ln3::cpp_lib::ArgumentParser::OptionValue::getSigned(const sll default_) const {
    if (isSigned()) { return _container.getTypeValue<sll>(); }
    return default_;
}

ull net_ln3::cpp_lib::ArgumentParser::OptionValue::getUnsigned(const ull default_) const {
    if (isUnsigned()) return _container.getTypeValue<ull>();
    return default_;
}

long double net_ln3::cpp_lib::ArgumentParser::OptionValue::getLongDouble(const long double default_) const {
    if (isLongDouble()) return _container.getTypeValue<long double>();
    return default_;
}

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::getBoolean(const bool default_) const {
    if (isBoolean()) return _container.getTypeValue<bool>();
    return default_;
}

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isNull() const { return _container.checkType<std::nullptr_t>(); }

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isString() const { return _container.checkType<std::string>(); }

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isSigned() const { return _container.checkType<sll>(); }

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isUnsigned() const { return _container.checkType<ull>(); }

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isLongDouble() const { return _container.checkType<long double>(); }

bool net_ln3::cpp_lib::ArgumentParser::OptionValue::isBoolean() const { return _container.checkType<bool>(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::operator std::string() const { return getString(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::operator long long() const { return getSigned(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::operator unsigned long() const { return getUnsigned(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::operator long double() const { return getLongDouble(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::operator bool() const { return getBoolean(); }

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const std::nullptr_t v_): _value(v_) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const char* v_)
    : VContainer(std::string(v_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(std::string v_): _value(std::move(v_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const long double v_): _value(v_) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const double v_): _value(
    static_cast<long double>(v_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const bool v_): _value(v_) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const int v_)
    : _value(static_cast<sll>(v_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const unsigned v_)
    : _value(static_cast<ull>(v_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const long long v_): _value(v_) {
}

net_ln3::cpp_lib::ArgumentParser::OptionValue::VContainer::VContainer(const unsigned long long v_): _value(v_) {
}

net_ln3::cpp_lib::ArgumentParser::OptionNames::OptionNames(
    std::unordered_map<std::string, OptionType> table_) : _name_type_table(std::move(table_)) {
}

net_ln3::cpp_lib::ArgumentParser::OptionType net_ln3::cpp_lib::ArgumentParser::OptionNames::getOptionType(
    const std::string& option_name_) const {
    if (_name_type_table.contains(option_name_)) {
        if (const auto t = _name_type_table.at(option_name_); t != OptionType::NULLITY) { return t; }
    }
    return OptionType();
}

bool net_ln3::cpp_lib::ArgumentParser::OptionNames::isExistOption(const std::string& option_name_) const {
    return _name_type_table.contains(option_name_);
}

bool net_ln3::cpp_lib::ArgumentParser::OptionNames::addOption(const std::string& option_name_,
                                                              OptionType type_) {
    if (_name_type_table.contains(option_name_)) { return false; }
    _name_type_table.try_emplace(option_name_, type_);
    return true;
}

bool net_ln3::cpp_lib::ArgumentParser::OptionNames::removeOption(const std::string& option_name_) {
    if (_name_type_table.contains(option_name_)) {
        _name_type_table.erase(option_name_);
        return true;
    }
    return false;
}

net_ln3::cpp_lib::ArgumentParser::OptionAlias::OptionAlias(
    std::unordered_map<std::string, std::string> table_) : _name_alias_table(std::move(table_)) {
}

std::string net_ln3::cpp_lib::ArgumentParser::OptionAlias::getOptionName(const std::string& alias_name_) const {
    if (_name_alias_table.contains(alias_name_)) { return _name_alias_table.at(alias_name_); }
    return "";
}

bool net_ln3::cpp_lib::ArgumentParser::OptionAlias::isExistAlias(const std::string& alias_name_) const {
    return _name_alias_table.contains(alias_name_);
}

bool net_ln3::cpp_lib::ArgumentParser::OptionAlias::addAlias(const std::string& alias_name_,
                                                             const std::string& option_name_) {
    if (_name_alias_table.contains(alias_name_)) { return false; }
    _name_alias_table.try_emplace(alias_name_, option_name_);
    return true;
}

bool net_ln3::cpp_lib::ArgumentParser::OptionAlias::removeAlias(const std::string& alias_name_) {
    if (_name_alias_table.contains(alias_name_)) {
        _name_alias_table.erase(alias_name_);
        return true;
    }
    return false;
}

net_ln3::cpp_lib::ArgumentParser::ArgumentParser() : _option_mapper_mode(false) {
}

net_ln3::cpp_lib::ArgumentParser::ArgumentParser(OptionNames type_) : ArgumentParser(std::move(type_), {}) {
}

net_ln3::cpp_lib::ArgumentParser::ArgumentParser(OptionNames type_,
                                                 OptionAlias
                                                 alias_) : _valid_option_names(std::move(type_)),
                                                           _valid_alias(std::move(alias_)),
                                                           _option_mapper_mode(true) {
}

void net_ln3::cpp_lib::ArgumentParser::parse(const int argc_, char** argv_) {
    const std::vector<std::string> args(argv_, argv_ + argc_);
    this->parse(args);
}

void net_ln3::cpp_lib::ArgumentParser::parse(const std::vector<std::string>& args_) {
    // 次のコマンドライン引数がオプションの値である
    bool option_value_flag = false;
    std::string option_name;
    std::string alias_name;
    for (auto& arg : args_) {
        // オプションを登録する。
        if (option_value_flag) {
            // マッピングモードでない場合。(従来の動作)
            if (!_option_mapper_mode) {
                if (option_name.empty()) { _addInvalidAlias(alias_name, arg); }
                else { _addOption(option_name, OptionValue::createInstance(arg)); }
                // エイリアス名が存在しない場合
            }
            else if (option_name.empty()) {
                _addInvalidAlias(alias_name, arg);
                // オプション名が存在する場合
            }
            else if (_valid_option_names.isExistOption(option_name)) {
                if (const auto t = _valid_option_names.getOptionType(option_name); _isValidType(arg, t)) {
                    _addOption(option_name, _convertOptionValue(arg, t));
                }
                else {
                    // 型検証に失敗したオプションを登録する。
                    _addInvalidOptionType(option_name, arg, t);
                }
                // オプション名が存在しない場合
            }
            else { _addInvalidOption(option_name, arg); }
            option_name = "";
            alias_name = "";
            option_value_flag = false;
            // オプション名を取得する。
        }
        else {
            if (_isOptionName(arg)) { option_name = _getOptionName(arg); }
            else if (_isAliasName(arg)) { alias_name = _getAliasName(arg); }
            else {
                _addArgument(arg);
                continue;
            }
            // エイリアス名でオプションが指定されている場合、それをオプション名に変換する。
            if (!alias_name.empty()) { option_name = _valid_alias.getOptionName(alias_name); }
            // オプションが真偽型の場合、フラグとして扱う。
            if (_valid_option_names.getOptionType(option_name) == OptionType::BOOLEAN) {
                _addOption(option_name, OptionValue::createInstance(true));
                option_name = "";
                alias_name = "";
            }
            else {
                // オプション<フラグ>以外では次の入力をオプションの値とする
                option_value_flag = true;
            }
        }
    }
}

const std::vector<std::string>& net_ln3::cpp_lib::ArgumentParser::getArgs() const { return _args; }

std::string net_ln3::cpp_lib::ArgumentParser::getArg(const size_t i_) const {
    if (i_ < _args.size()) { return _args.at(i_); }
    return "";
}

net_ln3::cpp_lib::ArgumentParser::OptionValue net_ln3::cpp_lib::ArgumentParser::getOption(
    const std::string& option_name_, const OptionValue& default_) const {
    if (_options.contains(option_name_)) { return _options.at(option_name_); }
    return default_;
}

bool net_ln3::cpp_lib::ArgumentParser::isExistOption(const std::string& key_) const { return _options.contains(key_); }

const std::unordered_map<std::string, std::vector<std::string>>&
net_ln3::cpp_lib::ArgumentParser::getInvalidOptions() const { return _invalid_options; }

const std::unordered_map<std::string, std::vector<std::pair<std::string,
                                                            net_ln3::cpp_lib::ArgumentParser::OptionType>>>&
net_ln3::cpp_lib::ArgumentParser::getInvalidOptionTypes() const { return _invalid_option_types; }

const std::unordered_map<std::string, std::vector<std::string>>&
net_ln3::cpp_lib::ArgumentParser::getInvalidAlias() const { return _invalid_alias; }

std::string net_ln3::cpp_lib::ArgumentParser::_getOptionName(const std::string& option_arg_) {
    return std::regex_replace(option_arg_, _extract_option_name_pattern, "");
}

std::string net_ln3::cpp_lib::ArgumentParser::_getAliasName(const std::string& alias_arg_) {
    return std::regex_replace(alias_arg_, _extract_alias_name_pattern, "");
}

bool net_ln3::cpp_lib::ArgumentParser::_isOptionName(const std::string& option_arg_) {
    return std::regex_match(option_arg_, _option_name_pattern);
}

bool net_ln3::cpp_lib::ArgumentParser::_isAliasName(const std::string& alias_arg_) {
    return std::regex_match(alias_arg_, _alias_name_pattern);
}

void net_ln3::cpp_lib::ArgumentParser::_addOption(const std::string& option_name_, OptionValue value_) {
    if (_options.contains(option_name_)) {
        _addInvalidOption(option_name_, value_.getString());
        return;
    }
    _options.try_emplace(option_name_, value_);
}

void net_ln3::cpp_lib::ArgumentParser::_addArgument(const std::string& value_) { _args.push_back(value_); }

void net_ln3::cpp_lib::ArgumentParser::_addInvalidOption(const std::string& option_name_,
                                                         const std::string& value_) {
    if (_invalid_options.contains(option_name_)) {
        _invalid_options.at(option_name_).emplace_back(value_);
        return;
    }
    _invalid_options.try_emplace(option_name_, std::vector({value_}));
}

void net_ln3::cpp_lib::ArgumentParser::_addInvalidOptionType(const std::string& option_name_,
                                                             const std::string& value_,
                                                             OptionType type_) {
    if (_invalid_option_types.contains(option_name_)) {
        _invalid_option_types.at(option_name_).emplace_back(value_, type_);
        return;
    }
    _invalid_option_types.try_emplace(option_name_,
                                      std::vector<std::pair<std::string, OptionType>>({{value_, type_}}));
}

void net_ln3::cpp_lib::ArgumentParser::_addInvalidAlias(const std::string& alias_name_,
                                                        const std::string& value_) {
    if (_invalid_alias.contains(alias_name_)) {
        _invalid_alias.at(alias_name_).emplace_back(value_);
        return;
    }
    _invalid_alias.try_emplace(alias_name_, std::vector({value_}));
}

bool net_ln3::cpp_lib::ArgumentParser::_isValidType(const std::string& value_,
                                                    const OptionType type_) {
    using sv = ParseUtil::StringValidator;
    switch (type_) {
    case OptionType::STRING:
        return true;
    case OptionType::SIGNED:
        return sv::isValidSigned(value_);
    case OptionType::UNSIGNED:
        return sv::isValidUnsigned(value_);
    case OptionType::LONG_DOUBLE:
        return sv::isValidLongDouble(value_);
    case OptionType::BOOLEAN:
        return sv::isValidBoolean(value_);
    case OptionType::NULLITY:
        return false;
    default:
        break;
    }
    return false;
}

net_ln3::cpp_lib::ArgumentParser::OptionValue net_ln3::cpp_lib::ArgumentParser::_convertOptionValue(
    const std::string& value_, const OptionType type_) {
    OptionValue ov;
    switch (type_) {
    case OptionType::STRING:
        ov = OptionValue::createInstance(value_);
        break;
    case OptionType::SIGNED:
        ov = OptionValue::createInstance(std::stoll(value_));
        break;
    case OptionType::UNSIGNED:
        ov = OptionValue::createInstance(std::stoull(value_));
        break;
    case OptionType::LONG_DOUBLE:
        ov = OptionValue::createInstance(std::stod(value_));
        break;
    case OptionType::BOOLEAN:
        ov = OptionValue::createInstance(value_ == "true");
        break;
    case OptionType::NULLITY:
        ov = OptionValue();
        break;
    default:
        break;
    }
    return ov;
}

const std::regex net_ln3::cpp_lib::ArgumentParser::_option_name_pattern{"--.+"};
const std::regex net_ln3::cpp_lib::ArgumentParser::_alias_name_pattern{"-.+"};
const std::regex net_ln3::cpp_lib::ArgumentParser::_extract_option_name_pattern{"^--"};
const std::regex net_ln3::cpp_lib::ArgumentParser::_extract_alias_name_pattern{"^-"};
