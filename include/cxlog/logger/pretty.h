#pragma once

#include <cxlog/macro.h>

#include <fmt/printf.h>

#include <chrono>
#include <string>

NAMESPACE_CXLOG_BEGIN

template <typename Rep, typename Period>
constexpr int64_t Microseconds(const std::chrono::duration<Rep, Period> &duration) {
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}

template <typename Rep, typename Period>
constexpr int64_t Seconds(const std::chrono::duration<Rep, Period> &duration) {
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

template <typename Rep, typename Period>
inline std::string Pretty(const std::chrono::duration<Rep, Period> &duration) {
    using namespace std::chrono_literals;

    static constexpr std::size_t USecsPerSec = Microseconds(1s);
    std::int64_t s = Seconds(duration);
    std::int64_t us = Microseconds(duration) % USecsPerSec;

    return fmt::sprintf("%lld.%06lld s", s, us);
}

NAMESPACE_CXLOG_END
