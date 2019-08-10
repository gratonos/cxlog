#pragma once

#include <cxlog/macro.h>

#include <algorithm>
#include <string>

#include <ctype.h>
#include <string.h>

NAMESPACE_CXLOG_BEGIN

template <typename Rep, typename Period>
constexpr std::int64_t Seconds(const std::chrono::duration<Rep, Period> &duration) {
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

template <typename Rep, typename Period>
constexpr std::int64_t MilliSeconds(const std::chrono::duration<Rep, Period> &duration) {
    using namespace std::chrono_literals;
    constexpr std::int64_t MSecsPerSec =
        std::chrono::duration_cast<std::chrono::milliseconds>(1s).count();

    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() %
        MSecsPerSec;
}

template <typename Rep, typename Period>
constexpr std::int64_t MicroSeconds(const std::chrono::duration<Rep, Period> &duration) {
    using namespace std::chrono_literals;
    constexpr std::int64_t USecsPerSec =
        std::chrono::duration_cast<std::chrono::microseconds>(1s).count();

    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() %
        USecsPerSec;
}

template <typename Rep, typename Period>
constexpr std::int64_t NanoSeconds(const std::chrono::duration<Rep, Period> &duration) {
    using namespace std::chrono_literals;
    constexpr std::int64_t NSecsPerSec =
        std::chrono::duration_cast<std::chrono::nanoseconds>(1s).count();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % NSecsPerSec;
}

inline std::string ToLower(const std::string &str) noexcept {
    std::string lower;
    lower.reserve(str.size());
    std::transform(std::cbegin(str), std::cend(str), std::back_inserter(lower), ::tolower);
    return std::move(lower);
}

inline void TrimLeft(std::string &str) {
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), ::isspace));
}

inline std::string TrimLeft(std::string &&str) {
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), ::isspace));
    return std::move(str);
}

inline void TrimRight(std::string &str) {
    str.erase(std::find_if_not(str.rbegin(), str.rend(), ::isspace).base(), str.end());
}

inline std::string TrimRight(std::string &&str) {
    str.erase(std::find_if_not(str.rbegin(), str.rend(), ::isspace).base(), str.end());
    return std::move(str);
}

inline void Trim(std::string &str) {
    TrimLeft(str);
    TrimRight(str);
}

inline std::string Trim(std::string &&str) {
    return TrimRight(TrimLeft(std::move(str)));
}

// support for utf-8 only
inline const char *LastSegments(const char *s, std::size_t n, char sep) noexcept {
    if (n == 0) {
        return s;
    }
    for (std::size_t i = std::strlen(s); i > 0; --i) {
        std::size_t index = i - 1;
        if (s[index] == sep) {
            --n;
            if (n == 0) {
                return s + i;
            }
        }
    }
    return s;
}

NAMESPACE_CXLOG_END
