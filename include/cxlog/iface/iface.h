#pragma once

#include <cxlog/macro.h>

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

NAMESPACE_CXLOG_BEGIN

enum class Level : std::size_t {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Off,
};

constexpr std::size_t LEVEL_COUNT =
    static_cast<std::size_t>(Level::Off) - static_cast<std::size_t>(Level::Trace);

struct Context final {
    std::string key;
    std::string value;
};

struct Record final {
    std::chrono::system_clock::time_point time;
    Level level = Level::Trace;
    const char *file = nullptr; // __FILE__
    std::size_t line = 0; // __LINE__
    const char *func = nullptr; // __FUNCTION__
    std::string msg;

    std::string prefix;
    std::vector<Context> contexts;
    bool mark = false;
};

struct Slice final {
    char *ptr = nullptr;
    std::size_t len = 0;
};

struct Error final {
    std::int64_t error = 0;
    std::string desc;
};

class Formatter {
    virtual Slice Format(const Record &record) = 0;
    virtual ~Formatter() = default;
};

class Writer {
    virtual Error Write(const Slice &slice, const Record &record) = 0;
    virtual ~Writer() = default;
};

NAMESPACE_CXLOG_END
