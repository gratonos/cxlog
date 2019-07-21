#include "filter_inl.h"

#include <cxlog/logger/config.h>

NAMESPACE_CXLOG_BEGIN

inline void copyConfig(LoggerConfig &dst, const LoggerConfig &src) {
    dst.level = src.level.load();
    dst.filter = src.filter;
}

inline void moveConfig(LoggerConfig &dst, LoggerConfig &&src) noexcept {
    dst.level = src.level.load();
    src.level = Level::Trace;
    dst.filter = std::move(src.filter);
}

void LoggerConfig::SetDefaults() {
    this->filter = FillFilter(this->filter);
}

LoggerConfig::LoggerConfig(const LoggerConfig &config) {
    copyConfig(*this, config);
}

LoggerConfig &LoggerConfig::operator=(const LoggerConfig &config) {
    if (&config == this) {
        return *this;
    }

    copyConfig(*this, config);
    return *this;
}

LoggerConfig::LoggerConfig(LoggerConfig &&config) noexcept {
    moveConfig(*this, std::move(config));
}

LoggerConfig &LoggerConfig::operator=(LoggerConfig &&config) noexcept {
    if (&config == this) {
        return *this;
    }

    moveConfig(*this, std::move(config));
    return *this;
}

NAMESPACE_CXLOG_END
