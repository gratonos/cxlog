#pragma once

#include <cxlog/macro.h>
#include <cxlog/util.h>

#include <fmt/printf.h>

#include <chrono>
#include <string>

NAMESPACE_CXLOG_BEGIN

template <typename Rep, typename Period>
inline std::string Pretty(const std::chrono::duration<Rep, Period> &duration) {
    std::int64_t s = Seconds(duration);
    std::int64_t us = MicroSeconds(duration);

    return fmt::sprintf("%lld.%06lld s", s, us);
}

NAMESPACE_CXLOG_END
