#pragma once

#ifdef NAMESPACE_CXLOG_BEGIN
#undef NAMESPACE_CXLOG_BEGIN
#endif
#define NAMESPACE_CXLOG_BEGIN namespace cxlog {

#ifdef NAMESPACE_CXLOG_END
#undef NAMESPACE_CXLOG_END
#endif
#define NAMESPACE_CXLOG_END }

#ifdef CXLOG_LOGGER_TRACE
#undef CXLOG_LOGGER_TRACE
#endif
#define CXLOG_LOGGER_TRACE(logger, format, ...)                                                \
    do {                                                                                       \
        (logger).Logf(                                                                         \
            ::cxlog::Level::Trace, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
    } while (false)

#ifdef CXLOG_LOGGER_DEBUG
#undef CXLOG_LOGGER_DEBUG
#endif
#define CXLOG_LOGGER_DEBUG(logger, format, ...)                                                \
    do {                                                                                       \
        (logger).Logf(                                                                         \
            ::cxlog::Level::Debug, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
    } while (false)

#ifdef CXLOG_LOGGER_INFO
#undef CXLOG_LOGGER_INFO
#endif
#define CXLOG_LOGGER_INFO(logger, format, ...)                                                \
    do {                                                                                      \
        (logger).Logf(                                                                        \
            ::cxlog::Level::Info, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
    } while (false)

#ifdef CXLOG_LOGGER_WARN
#undef CXLOG_LOGGER_WARN
#endif
#define CXLOG_LOGGER_WARN(logger, format, ...)                                                \
    do {                                                                                      \
        (logger).Logf(                                                                        \
            ::cxlog::Level::Warn, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
    } while (false)

#ifdef CXLOG_LOGGER_ERROR
#undef CXLOG_LOGGER_ERROR
#endif
#define CXLOG_LOGGER_ERROR(logger, format, ...)                                                \
    do {                                                                                       \
        (logger).Logf(                                                                         \
            ::cxlog::Level::Error, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
    } while (false)

#ifdef CXLOG_LOGGER_FATAL
#undef CXLOG_LOGGER_FATAL
#endif
#define CXLOG_LOGGER_FATAL(logger, format, ...)                                                \
    do {                                                                                       \
        (logger).Logf(                                                                         \
            ::cxlog::Level::Fatal, __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__); \
        ::std::exit(EXIT_FAILURE);                                                             \
    } while (false)

#ifdef CXLOG_LOGGER_TIMING
#undef CXLOG_LOGGER_TIMING
#endif
#define CXLOG_LOGGER_TIMING(logger, level, format, ...) \
    (logger).Timingf((level), __FILE__, __LINE__, __FUNCTION__, (format), ##__VA_ARGS__)
