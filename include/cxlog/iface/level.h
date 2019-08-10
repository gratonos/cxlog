#pragma once

#include <cxlog/macro.h>

#include <cstddef>

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

constexpr std::size_t LevelToSizeT(Level level) noexcept {
    return static_cast<std::size_t>(level);
}

constexpr bool operator<(Level lhs, Level rhs) noexcept {
    return LevelToSizeT(lhs) < LevelToSizeT(rhs);
}

constexpr bool operator<=(Level lhs, Level rhs) noexcept {
    return lhs < rhs || lhs == rhs;
}

constexpr bool operator>(Level lhs, Level rhs) noexcept {
    return !(lhs <= rhs);
}

constexpr bool operator>=(Level lhs, Level rhs) noexcept {
    return !(lhs < rhs);
}

constexpr std::size_t LEVEL_COUNT = LevelToSizeT(Level::Off) - LevelToSizeT(Level::Trace);

NAMESPACE_CXLOG_END
