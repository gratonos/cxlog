#pragma once

#include <cxlog/iface/iface.h>
#include <cxlog/logger/filter.h>

#include <atomic>

NAMESPACE_CXLOG_BEGIN

class LoggerConfig final {
public:
    LoggerConfig(Level level = Level::Trace, const Filter &filter = nullptr) {
        this->level = level;
        this->filter = FillFilter(filter);
    }
    LoggerConfig(const LoggerConfig &config) {
        this->Copy(config);
    }
    LoggerConfig &operator=(const LoggerConfig &config) {
        if (this != &config) {
            this->Copy(config);
        }
        return *this;
    }
    LoggerConfig(LoggerConfig &&config) noexcept {
        this->Move(std::move(config));
    }
    LoggerConfig &operator=(LoggerConfig &&config) noexcept {
        if (this != &config) {
            this->Move(std::move(config));
        }
        return *this;
    }

public:
    Level GetLevel() const noexcept {
        return this->level;
    }
    void SetLevel(Level level) noexcept {
        this->level = level;
    }

    Filter GetFilter() const {
        return this->filter;
    }
    void SetFilter(const Filter &filter) {
        this->filter = FillFilter(filter);
    }

public:
    bool DoFilter(const Record &record) const {
        return this->filter(record);
    }

private:
    void Copy(const LoggerConfig &config) {
        this->level = config.level.load();
        this->filter = config.filter;
    }
    void Move(LoggerConfig &&config) noexcept {
        this->level = config.level.load();
        this->filter = std::move(config.filter);
    }

private:
    std::atomic<Level> level;
    Filter filter;
};

NAMESPACE_CXLOG_END
