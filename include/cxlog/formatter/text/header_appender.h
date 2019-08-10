#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/iface/iface.h>

#include <fmt/format.h>

#include <memory>
#include <string>

NAMESPACE_CXLOG_BEGIN

class HeaderAppender final {
private:
    static std::unique_ptr<ElementFormatter> CreateElementFormatter(
        const std::string &element, const std::string &property, const std::string &fmtstr);

public:
    HeaderAppender(const std::string &element, const std::string &property,
        const std::string &fmtstr, const std::string &static_text) {

        this->formatter = CreateElementFormatter(element, property, fmtstr);
        this->static_text = static_text;
    }

public:
    void AppendHeader(fmt::memory_buffer &buf, const Record &record) const {
        fmt::format_to(buf, "{}", this->static_text);
        this->formatter->FormatElement(buf, record);
    }

private:
    std::unique_ptr<ElementFormatter> formatter;
    std::string static_text;
};

NAMESPACE_CXLOG_END
