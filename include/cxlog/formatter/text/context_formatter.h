#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/formatter/text/fmt.h>
#include <cxlog/iface/iface.h>
#include <cxlog/util.h>

#include <functional>
#include <string>

NAMESPACE_CXLOG_BEGIN

class ContextFormatter final : public ElementFormatter {
private:
    using FormatFunc =
        std::function<void(fmt::memory_buffer &buf, const std::vector<Context> &contexts)>;

private:
    static void FormatPair(fmt::memory_buffer &buf, const std::vector<Context> &contexts) {
        const char *left = "(";
        for (const Context &context : contexts) {
            fmt::bprintf(buf, "%s%s: %s)", left, context.key, context.value);
            left = " (";
        }
    }
    static void FormatList(fmt::memory_buffer &buf, const std::vector<Context> &contexts) {
        const char *begin = "";
        for (const Context &context : contexts) {
            fmt::bprintf(buf, "%s%s: %s", begin, context.key, context.value);
            begin = ", ";
        }
    }

public:
    ContextFormatter(const std::string &property, const std::string &fmtstr) {
        this->formatter = (ToLower(property) == "list" ? FormatList : FormatPair);
        this->fmtstr = (fmtstr.empty() ? "%s" : fmtstr);
    }

public:
    void FormatElement(fmt::memory_buffer &buf, const Record &record) override {
        this->formatter(buf, record.contexts);
    }

private:
    FormatFunc formatter;
    std::string fmtstr;
};

NAMESPACE_CXLOG_END
