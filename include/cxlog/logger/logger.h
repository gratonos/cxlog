#pragma once

#include <cxlog/logger/config.h>
#include <cxlog/logger/context.h>
#include <cxlog/logger/slot.h>

#include <fmt/printf.h>

#include <memory>
#include <mutex>

NAMESPACE_CXLOG_BEGIN

// A Logger is a logging framework that contains EIGHT slots. Each Slot contains
// a Formatter and a Writer. A Logger has its own level and filter while each
// Slot has its independent level and filter. Logger calls the Formatter and
// Writer of each Slot in the order from Slot0 to Slot7 when a log is emitted.
class Logger final {
private:
    struct Additional final {
        Level level = Level::Trace;
        Filter filter = NullFilter();
        std::shared_ptr<std::string> prefix = std::make_shared<std::string>();
        std::shared_ptr<std::vector<StaticContext>> statics =
            std::make_shared<std::vector<StaticContext>>();
        std::shared_ptr<std::vector<DynamicContext>> dynamics =
            std::make_shared<std::vector<DynamicContext>>();
        bool mark = false;
    };
    struct Intrinsic final {
        LoggerConfig config;
        // slots       []Slot
        // equivalents [][]int // indexes of equivalent formatters
        std::recursive_mutex lock;
    };

public:
    Logger(const LoggerConfig &config) {
        this->intrinsic->config = config;
    }
    Logger(const Logger &logger) = default;
    Logger &operator=(const Logger &logger) = delete;
    Logger(Logger &&logger) noexcept = default;
    Logger &operator=(Logger &&logger) noexcept = delete;

public:
    Logger WithLevel(Level level) const & {
        Logger logger(*this);
        logger.additional.level = level;
        return std::move(logger);
    }
    Logger WithLevel(Level level) && noexcept {
        this->additional.level = level;
        return std::move(*this);
    }
    Logger WithFilter(const Filter &filter) const & {
        Logger logger(*this);
        logger.additional.filter = FillFilter(filter);
        return std::move(logger);
    }
    Logger WithFilter(const Filter &filter) && {
        this->additional.filter = FillFilter(filter);
        return std::move(*this);
    }
    template <typename T>
    Logger WithPrefix(T &&prefix) const & {
        Logger logger(*this);
        logger.additional.prefix =
            std::make_shared<std::string>(fmt::to_string(std::forward<T>(prefix)));
        return std::move(logger);
    }
    template <typename T>
    Logger WithPrefix(T &&prefix) && {
        this->additional.prefix =
            std::make_shared<std::string>(fmt::to_string(std::forward<T>(prefix)));
        return std::move(*this);
    }
    Logger WithStatics(std::initializer_list<StaticContext> static_list) const & {
        Logger logger(*this);
        logger.additional.statics = CloneAndAppend(logger.additional.statics, static_list);
        return std::move(logger);
    }
    Logger WithStatics(std::initializer_list<StaticContext> static_list) && {
        this->additional.statics = CloneAndAppend(this->additional.statics, static_list);
        return std::move(*this);
    }
    Logger WithDynamics(std::initializer_list<DynamicContext> dynamic_list) const & {
        Logger logger(*this);
        logger.additional.dynamics = CloneAndAppend(logger.additional.dynamics, dynamic_list);
        return std::move(logger);
    }
    Logger WithDynamics(std::initializer_list<DynamicContext> dynamic_list) && {
        this->additional.dynamics = CloneAndAppend(this->additional.dynamics, dynamic_list);
        return std::move(*this);
    }
    Logger WithMark(bool mark) const & {
        Logger logger(*this);
        logger.additional.mark = mark;
        return std::move(logger);
    }
    Logger WithMark(bool mark) && noexcept {
        this->additional.mark = mark;
        return std::move(*this);
    }

public:
    Level GetLevel() const noexcept {
        return this->intrinsic->config.GetLevel();
    }
    void SetLevel(Level level) noexcept {
        this->intrinsic->config.SetLevel(level);
    }
    Filter GetFilter() const {
        std::lock_guard<decltype(this->intrinsic->lock)> lock(this->intrinsic->lock);
        return this->intrinsic->config.GetFilter();
    }
    void SetFilter(const Filter &filter) {
        std::lock_guard<decltype(this->intrinsic->lock)> lock(this->intrinsic->lock);
        this->intrinsic->config.SetFilter(filter);
    }

public:
    template <typename S, typename... Args>
    void Tracef(const char *file, std::size_t line, const char *func, S &&format,
        Args &&... args) const;

    template <typename S, typename... Args>
    void Logf(Level level, const char *file, std::size_t line, const char *func, S &&format,
        Args &&... args) const;

private:
    template <typename T>
    static inline std::shared_ptr<std::vector<T>> CloneAndAppend(
        const std::shared_ptr<std::vector<T>> &origin, std::initializer_list<T> list) {

        auto clone = std::make_shared<std::vector<T>>();
        clone->reserve(origin->size() + list.size());
        std::copy(std::begin(*origin), std::end(*origin), std::back_inserter(*clone));
        std::copy(std::begin(list), std::end(list), std::back_inserter(*clone));
        return std::move(clone);
    }

private:
    bool NeedToLog(Level level) const;
    void Log(Level level, const char *file, std::size_t line, const char *func,
        std::string &&msg) const;
    void FormatAndWrite(Level level, const Record &record) const;

private:
    Additional additional; // copy on write, concurrency safe
    std::shared_ptr<Intrinsic> intrinsic = std::make_shared<Intrinsic>();
};

template <typename S, typename... Args>
inline void Logger::Tracef(
    const char *file, std::size_t line, const char *func, S &&format, Args &&... args) const {
    Logf(Level::Trace, file, line, func, format, args...);
}

template <typename S, typename... Args>
inline void Logger::Logf(Level level, const char *file, std::size_t line, const char *func,
    S &&format, Args &&... args) const {

    if (NeedToLog(level)) {
        Log(level, file, line, func, fmt::sprintf(format, args...));
    }
}

inline bool Logger::NeedToLog(Level level) const {
    if (level == Level::Off) {
        throw std::invalid_argument("cxlog: invalid log level");
    }
    return static_cast<std::size_t>(additional.level) <= static_cast<std::size_t>(level) &&
        static_cast<std::size_t>(GetLevel()) <= static_cast<std::size_t>(level);
}

NAMESPACE_CXLOG_END
