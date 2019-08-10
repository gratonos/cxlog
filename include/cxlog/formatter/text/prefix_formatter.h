#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/formatter/text/fmt.h>
#include <cxlog/iface/iface.h>

#include <string>

NAMESPACE_CXLOG_BEGIN

class PrefixFormatter final : public ElementFormatter {
public:
    PrefixFormatter(const std::string &fmtstr) {
        this->fmtstr = (fmtstr.empty() ? "%s" : fmtstr);
    }

public:
    void FormatElement(fmt::memory_buffer &buf, const Record &record) override {
        fmt::bprintf(buf, this->fmtstr, record.prefix);
    }

private:
    std::string fmtstr;
};

NAMESPACE_CXLOG_END
