#pragma once

#include <cxlog/formatter/null.h>
#include <cxlog/iface/iface.h>
#include <cxlog/logger/error_handler.h>
#include <cxlog/logger/filter.h>
#include <cxlog/writer/null.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

enum class SlotIndex : std::size_t {
    Slot0,
    Slot1,
    Slot2,
    Slot3,
    Slot4,
    Slot5,
    Slot6,
    Slot7,
};

constexpr size_t SlotToSize(SlotIndex slot) noexcept {
    return static_cast<std::size_t>(slot);
}

constexpr std::size_t SLOT_COUNT = 8;

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
    std::shared_ptr<Formatter> GetFormatter() const noexcept {
        return this->formatter;
    }
    void SetFormatter(const std::shared_ptr<Formatter> &formatter) noexcept {
        this->formatter = FillFormatter(formatter);
    }
    std::shared_ptr<Writer> GetWriter() const noexcept {
        return this->writer;
    }
    void SetWriter(const std::shared_ptr<Writer> &writer) noexcept {
        this->writer = FillWriter(writer);
    }
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
    ErrorHandler GetErrorHandler() const {
        return this->handler;
    }
    void SetErrorHandler(const ErrorHandler &handler) {
        this->handler = FillErrorHandler(handler);
    }

public:
    bool NeedToLog(Level level, const Record &record) const {
        return this->level <= level && this->filter(record);
    }
    std::string Format(const Record &record) const {
        return this->formatter->Format(record);
    }
    void Write(const std::string &log, const Record &record) const {
        const Error &error = this->writer->Write(log, record);
        if (error.error != 0) {
            this->handler(log, record, error);
        }
    }

private:
    std::shared_ptr<Formatter> formatter;
    std::shared_ptr<Writer> writer;
    Level level;
    Filter filter;
    ErrorHandler handler;
};

NAMESPACE_CXLOG_END
