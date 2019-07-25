#pragma once

#include <cxlog/formatter/null.h>
#include <cxlog/iface/iface.h>
#include <cxlog/logger/error_handler.h>
#include <cxlog/logger/filter.h>
#include <cxlog/writer/null.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

class Slot final {
public:
    Slot(const std::shared_ptr<Formatter> &formatter = nullptr,
        const std::shared_ptr<Writer> &writer = nullptr, Level level = Level::Off,
        const Filter &filter = nullptr, const ErrorHandler &handler = nullptr) {

        this->formatter = FillFormatter(formatter);
        this->writer = FillWriter(writer);
        this->level = level;
        this->filter = FillFilter(filter);
        this->handler = FillErrorHandler(handler);
    }

public:
    const std::shared_ptr<Formatter> &GetFormatter() const noexcept {
        return this->formatter;
    }
    void SetFormatter(const std::shared_ptr<Formatter> &formatter) {
        this->formatter = FillFormatter(formatter);
    }
    const std::shared_ptr<Writer> &GetWriter() const noexcept {
        return this->writer;
    }
    void SetWriter(const std::shared_ptr<Writer> &writer) {
        this->writer = FillWriter(writer);
    }
    Level GetLevel() const noexcept {
        return this->level;
    }
    void SetLevel(Level level) noexcept {
        this->level = level;
    }
    const Filter &GetFilter() const noexcept {
        return this->filter;
    }
    void SetFilter(const Filter &filter) {
        this->filter = FillFilter(filter);
    }
    const ErrorHandler &GetErrorHandler() const noexcept {
        return this->handler;
    }
    void SetErrorHandler(const ErrorHandler &handler) {
        this->handler = FillErrorHandler(handler);
    }

private:
    std::shared_ptr<Formatter> formatter;
    std::shared_ptr<Writer> writer;
    Level level;
    Filter filter;
    ErrorHandler handler;
};

NAMESPACE_CXLOG_END
