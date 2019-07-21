#pragma once

#include <cxlog/iface/iface.h>
#include <cxlog/logger/filter.h>

#include <atomic>

NAMESPACE_CXLOG_BEGIN

struct LoggerConfig final {
    LoggerConfig() = default;
    LoggerConfig(const LoggerConfig &config);
    LoggerConfig &operator=(const LoggerConfig &config);
    LoggerConfig(LoggerConfig &&config) noexcept;
    LoggerConfig &operator=(LoggerConfig &&config) noexcept;

    void SetDefaults();

    std::atomic<Level> level{Level::Trace};
    Filter filter;
};

NAMESPACE_CXLOG_END
