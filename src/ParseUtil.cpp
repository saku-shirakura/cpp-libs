#include "ParseUtil.h"

namespace com_sakushira::cpp_lib {

    template<class T>
    [[maybe_unused]] std::vector<T> ParseUtil::slice(std::vector<T> array, size_t beg, size_t end) {
        if (end < array.size() && beg < array.size()) {
            if (beg > end) {
                return std::vector<T>(array.begin() + beg, array.end()); // beg以降の文字列
            } else if (beg < end) {
                return std::vector<T>(array.begin() + beg, array.begin() + end + 1); // begからend-beg+1個分の文字列
            } else {
                return std::vector<T>(array.begin(), array.begin() + end + 1); // 最初からendまでの文字列
            }
        }
        throw ParseUtilOutOfRange("[ParseUtil::slice()] Error: invalid argument. beg or end is out of range.");
    }

    std::vector<char> ParseUtil::toArray(std::string str) {
        return {str.begin(), str.end()};
    }

    std::string ParseUtil::toString(std::vector<char> array) {
        return {array.begin(), array.end()};
    }

    std::string ParseUtil::slice(std::string str, size_t beg, size_t end) {
        try {
            return toString(slice(toArray(std::move(str)), beg, end));
        } catch (std::exception &e) {
            throw;
        }
    }

    std::vector<std::string> ParseUtil::split(const std::string &str, const std::string &delim) {
        try {
            if (delim.empty()) {
                auto tmp_result = toArray(str);
                std::vector<std::string> result{};
                for (const auto &i: tmp_result) {
                    std::string w;
                    w.push_back(i);
                    result.emplace_back(w);
                }
                return result;
            }
            std::string work = str;
            std::vector<std::string> result;
            size_t pos = work.find(delim);
            while (pos != std::string::npos) {
                result.emplace_back(slice(work, 0, pos + delim.length() - 1));
                work.erase(0, result.back().length());
                result.back() = std::regex_replace(result.back(), std::regex(delim), "");
                pos = work.find(delim);
            }
            result.emplace_back(work);
            return result;
        } catch (std::exception &e) {
            throw;
        }
    }

    std::string ParseUtil::appendAll(const std::vector<std::string> &input, const std::string &glue) {
        if (input.empty()) {
            return "";
        }
        std::string str(input.front());
        for (size_t i = 1; i < input.size(); i++) {
            str += glue + input.at(i);
        }
        return str;
    }
}
