#pragma once

#include <cxlog/macro.h>

#include <string>

NAMESPACE_CXLOG_BEGIN

template <typename T>
std::string ToString(T &&value) {
    return std::to_string(std::forward(value));
}

const std::string &ToString(const std::string &str) {
    return str;
}

std::string ToString(std::string &&str) {
    return std::move(str);
}

std::string ToString(bool b) {
    return b ? std::string("true") : std::string("false");
}
std::string ToString(char c) {
    return std::string() + c;
}

NAMESPACE_CXLOG_END
