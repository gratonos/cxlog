#pragma once

#include <cxlog/logger/config.h>
#include <cxlog/logger/util.h>

#include <memory>
#include <mutex>

NAMESPACE_CXLOG_BEGIN

using Dynamic = std::function<std::string(const std::string &key)>;

class Logger final {
private:
    struct StaticContext final {
        template <typename K, typename V>
        StaticContext(K &&key, V &&value) :
            key(ToString(std::forward(key))), value(ToString(std::forward(value))) {}

        std::string key;
        std::string value;
    };

    struct DynamicContext final {
        template <typename K>
        DynamicContext(K &&key, const Dynamic &value) :
            key(ToString(std::forward(key))), value(value) {}

        std::string key;
        Dynamic value;
    };

    struct Additional final {
        Level level = Level::Trace;
        Filter filter;
        std::shared_ptr<std::string> prefix = std::make_shared<std::string>();
        std::shared_ptr<std::vector<StaticContext>> statics =
            std::make_shared<std::vector<StaticContext>>();
        std::shared_ptr<std::vector<DynamicContext>> dynamics =
            std::make_shared<std::vector<DynamicContext>>();
        bool mark = false;
    };

public:
    Logger(const LoggerConfig &config);

public:
    Logger WithLevel(Level level) const;
    Logger WithFilter(Filter filter) const;
    template <typename T>
    Logger WithPrefix(T &&prefix) const;
    Logger WithStatics(std::initializer_list<StaticContext> static_list) const;
    template <typename K>
    Logger WithDynamics(std::initializer_list<DynamicContext> dynamic_list) const;
    Logger WithMark(bool mark) const;

public:
    Level GetLevel() const noexcept;
    void SetLevel(Level level) noexcept;
    Filter GetFilter() const;
    void SetFilter(Filter filter);

public:
    template <typename... Args>
    void Tracef(const char *file, std::size_t line, const char *func, Args &&... args);
    template <typename... Args>
    void Logf(
        Level level, const char *file, std::size_t line, const char *func, Args &&... args);

private:
    Additional m_additional; // copy on write, concurrency safe
    std::shared_ptr<LoggerConfig> m_config = std::make_shared<LoggerConfig>();
    // slots       []Slot
    // equivalents [][]int // indexes of equivalent formatters
    std::shared_ptr<std::recursive_mutex> m_lock = std::make_shared<std::recursive_mutex>();
};

inline Logger Logger::WithLevel(Level level) const {
    Logger logger(*this);
    logger.m_additional.level = level;
    return std::move(logger);
}

inline Logger Logger::WithFilter(Filter filter) const {
    Logger logger(*this);
    logger.m_additional.filter = filter;
    return std::move(logger);
}

template <typename T>
inline Logger Logger::WithPrefix(T &&prefix) const {
    Logger logger(*this);
    *logger.m_additional.prefix = ToString(std::forward(prefix));
    return std::move(logger);
}

inline Logger Logger::WithStatics(std::initializer_list<StaticContext> static_list) const {
    Logger logger(*this);

    auto &statics = logger.m_additional.statics;
    statics = std::make_shared<std::vector<StaticContext>>(*statics);
    statics->reserve(statics->size() + static_list.size());

    for (auto &context : static_list) {
        statics->emplace_back(context);
    }

    return std::move(logger);
}

template <typename K>
inline Logger Logger::WithDynamics(std::initializer_list<DynamicContext> dynamic_list) const {
    Logger logger(*this);

    auto &dynamics = logger.m_additional.dynamics;
    dynamics = std::make_shared<std::vector<DynamicContext>>(*dynamics);
    dynamics->reserve(dynamics->size() + dynamic_list.size());

    for (auto &context : dynamic_list) {
        dynamics->emplace_back(context);
    }

    return std::move(logger);
}

inline Logger Logger::WithMark(bool mark) const {
    Logger logger(*this);
    logger.m_additional.mark = mark;
    return std::move(logger);
}

NAMESPACE_CXLOG_END
