#pragma once

#include <cxlog/formatter/text/formatter.h>
#include <cxlog/logger/logger.h>
#include <cxlog/writer/wrapper.h>

NAMESPACE_CXLOG_BEGIN

inline std::shared_ptr<TextFormatter> CreateDefaultFormatter() {
    auto formatter = std::make_shared<TextFormatter>(TextConfig());
    return std::move(formatter);
}

inline const std::shared_ptr<TextFormatter> &DefaultFormatter() noexcept {
    static const auto formatter = CreateDefaultFormatter();
    return formatter;
}

inline Logger CreateDefaultLogger() {
    Logger logger(LoggerConfig{});
    Slot slot(DefaultFormatter(), std::make_shared<WriterWrapper>(std::cerr), Level::Trace);
    logger.SetSlot(SlotIndex::Slot0, std::move(slot));
    return std::move(logger);
}

inline Logger &DefaultLogger() noexcept {
    static Logger logger = CreateDefaultLogger();
    return logger;
}

NAMESPACE_CXLOG_END
