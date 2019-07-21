#include <cxlog/logger/logger.h>

NAMESPACE_CXLOG_BEGIN

Logger::Logger(const LoggerConfig &config) {
    *m_config = config;
    m_config->SetDefaults();
}

Level Logger::GetLevel() const noexcept {
    return m_config->level;
}

void Logger::SetLevel(Level level) noexcept {
    m_config->level = level;
}

Filter Logger::GetFilter() const {
    std::lock_guard<decltype(*m_lock)> lock(*m_lock);
    return m_config->filter;
}

void Logger::SetFilter(Filter filter) {
    std::lock_guard<decltype(*m_lock)> lock(*m_lock);
    m_config->filter = filter;
}

template <typename... Args>
void Logger::Tracef(const char *file, std::size_t line, const char *func, Args &&... args) {
    Logf(Level::Trace, file, line, func, args...);
}

template <typename... Args>
void Logger::Logf(
    Level level, const char *file, std::size_t line, const char *func, Args &&... args) {}

NAMESPACE_CXLOG_END
