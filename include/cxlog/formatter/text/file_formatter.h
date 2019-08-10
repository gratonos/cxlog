#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/formatter/text/fmt.h>
#include <cxlog/iface/iface.h>
#include <cxlog/util.h>

#include <cstdlib>
#include <string>

NAMESPACE_CXLOG_BEGIN

class FileFormatter final : public ElementFormatter {
public:
    FileFormatter(const std::string &property, const std::string &fmtstr) {
        int segments = std::atoi(property.c_str());
        if (segments < 0) {
            segments = 0;
        }
        this->segments = static_cast<std::size_t>(segments);
        this->fmtstr = (fmtstr.empty() ? "%s" : fmtstr);
    }

public:
    void FormatElement(fmt::memory_buffer &buf, const Record &record) override {
        // support for unix-like only
        const char *file = LastSegments(record.file, this->segments, '/');
        // for absolute path, avoid to omit the root path '/'
        if (record.file + 1 == file && record.file[0] == '/') {
            file = record.file;
        }
        fmt::bprintf(buf, this->fmtstr, file);
    }

private:
    std::string fmtstr;
    std::size_t segments;
};

NAMESPACE_CXLOG_END
