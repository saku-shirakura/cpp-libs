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

#include <iostream>
#include <net_ln3/cpp_lib/multi_platform_util.h>
#ifdef _WIN32
#include <windows.h>

net_ln3::cpp_lib::multi_platform::CodePageGuard::CodePageGuard(): CodePageGuard(CP_UTF8) {
}

net_ln3::cpp_lib::multi_platform::CodePageGuard::CodePageGuard(const unsigned target_): _target_code_page(target_) {
    _setCodePage();
}

net_ln3::cpp_lib::multi_platform::CodePageGuard::~CodePageGuard() { _restoreCodePage(); }

void net_ln3::cpp_lib::multi_platform::CodePageGuard::_setCodePage() {
    _before_code_page = GetConsoleOutputCP();
    if (_before_code_page == 0) {
        // 警告: コードページを正常に取得できませんでした。
        std::cerr << "Warning: Failed to get codepage successfully. Error code: "
            << GetLastError();
        return;
    }
    if (_before_code_page == _target_code_page) { return; }
    if (SetConsoleOutputCP(_target_code_page) == 0) {
        // 警告: コードページを正常に変更できませんでした。
        std::cerr << "Warning: The codepage could not be changed successfully. Error code: "
            << GetLastError();
    }
}

void net_ln3::cpp_lib::multi_platform::CodePageGuard::_restoreCodePage() const {
    if (_before_code_page == 0 || _target_code_page == _before_code_page) { return; }
    if (SetConsoleOutputCP(_before_code_page) == 0) {
        // 警告: コードページを正常に変更できませんでした。
        std::cerr << "Warning: The codepage could not be changed successfully. Error code: "
            << GetLastError();
    }
}

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::enable(const bool rerun) {
    // すでに実行済みであればスキップする。また、rerunフラグが有効であれば再度実行する。
    if (_is_executed && !rerun) { return isEnabled(); }
    _is_executed = true;
    if (isEnabled())
        return true;
    const DWORD mode = _getConsoleMode();
    if (mode == 0) { return false; }
    // エスケープシーケンスがすでに有効であるかチェックする。
    if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) { _enabled = true; }
    else {
        //エスケープシーケンスを有効にする。
        _enabled = SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    return _enabled;
}

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::isEnabled() {
    if (_is_executed) { return _enabled; }
    if (_enabled) { return true; }
    // エスケープシーケンスが有効であるかチェックする。
    if (_getConsoleMode() & ENABLE_VIRTUAL_TERMINAL_PROCESSING) { _enabled = true; }
    return _enabled;
}

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::isExecuted() { return _is_executed; }

unsigned long net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::_getConsoleMode() {
    // ReSharper disable once CppLocalVariableMayBeConst
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    // 現在のモードを取得
    if (!GetConsoleMode(handle, &mode)) { return 0; }
    return mode;
}

#else
// DISABLE ALL FUNCTIONS

net_ln3::cpp_lib::multi_platform::CodePageGuard::CodePageGuard() {}

net_ln3::cpp_lib::multi_platform::CodePageGuard::CodePageGuard(const unsigned target_) {}

net_ln3::cpp_lib::multi_platform::CodePageGuard::~CodePageGuard() {}

void net_ln3::cpp_lib::multi_platform::CodePageGuard::_setCodePage() {}

void net_ln3::cpp_lib::multi_platform::CodePageGuard::_restoreCodePage() const {}

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::enable(const bool rerun) { return true; }

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::isEnabled() { return true; }

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::isExecuted() { return true; }

unsigned long net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::_getConsoleMode() { return 0; }

#endif

bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::_is_executed = false;
bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::_enabled = false;
bool net_ln3::cpp_lib::multi_platform::EnableAnsiEscapeSequence::_is_mode_obtained = false;
