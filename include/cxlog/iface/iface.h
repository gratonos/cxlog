#pragma once

#include <cxlog/macro.h>

#include <chrono>
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

constexpr size_t LevelToSize(Level level) noexcept {
    return static_cast<std::size_t>(level);
}

constexpr std::size_t LEVEL_COUNT = LevelToSize(Level::Off) - LevelToSize(Level::Trace);

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

struct Error final {
    std::int64_t error = 0;
    std::string desc;
};

class Formatter {
public:
    virtual std::string Format(const Record &record) = 0;
    virtual ~Formatter() = default;
};

class Writer {
public:
    virtual Error Write(const std::string &log, const Record &record) = 0;
    virtual ~Writer() = default;
};

NAMESPACE_CXLOG_END
