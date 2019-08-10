#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/formatter/text/fmt.h>
#include <cxlog/iface/iface.h>
#include <cxlog/util.h>

#include <array>
#include <string>

NAMESPACE_CXLOG_BEGIN

class LevelFormatter final : public ElementFormatter {
public:
    LevelFormatter(const std::string &property, const std::string &fmtstr) {
        if (ToLower(property) == "char") {
            this->level_names = {"T", "D", "I", "W", "E", "F"};
        } else {
            this->level_names = {"TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};
        }
        this->fmtstr = (fmtstr.empty() ? "%s" : fmtstr);
    }

public:
    void FormatElement(fmt::memory_buffer &buf, const Record &record) override {
        fmt::bprintf(buf, this->fmtstr, this->level_names[LevelToSizeT(record.level)]);
    }

private:
    std::string fmtstr;
    std::array<const char *, LogLevelCount> level_names;
};

NAMESPACE_CXLOG_END
