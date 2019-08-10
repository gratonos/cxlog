#include <cxlog/formatter/text/formatter.h>
#include <cxlog/util.h>

#include <fmt/format.h>

#include <regex>
#include <stdexcept>

NAMESPACE_CXLOG_BEGIN

inline void ExtractElement(std::string &element, std::string &property, std::string &fmtspec,
    const std::smatch &match) {
    if (match.size() != 4) {
        throw std::logic_error("cxlog: unmatched groups");
    }
    // {{name:property%fmtstr}}
    // 0<-------------------->0
    //   1<>1
    //        2<---->2
    //                3<--->3
    element = ToLower(Trim(match[1].str()));
    property = Trim(match[2].str());
    fmtspec = Trim(match[3].str());
    if (fmtspec == "%") {
        fmtspec = "";
    }
}

std::string TextFormatter::Format(const Record &record) {
    LockGuard lock(this->lock);

    fmt::string_view left, right;
    if (this->coloring) {
        if (record.mark) {
            std::tie(left, right) = this->color_mgr.MarkColorEars();
        } else {
            std::tie(left, right) = this->color_mgr.ColorEars(record.level);
        }
    }

    this->buf.clear();

    fmt::format_to(this->buf, "{}", left);
    for (const HeaderAppender &appender : this->appenders) {
        appender.AppendHeader(this->buf, record);
    }
    fmt::format_to(this->buf, "{}", this->suffix);
    fmt::format_to(this->buf, "{}", right);

    return std::string(this->buf.data(), this->buf.size());
}

void TextFormatter::SetHeader(const std::string &header) {
    // {{<name>[:property][%fmtstr]}}
    static const std::regex Regex("\\{\\{([^:%]*?)(?::([^%]*?))?(%.*?)?\\}\\}");

    LockGuard lock(this->lock);

    this->header = header;
    this->appenders.clear();

    std::string head = header;
    std::string static_text;
    std::smatch match;
    while (!head.empty() && std::regex_search(head, match, Regex)) {
        static_text += match.prefix().str();
        std::string element, property, fmtspec;
        ExtractElement(element, property, fmtspec, match);
        if (this->AddAppender(element, property, fmtspec, static_text)) {
            static_text.clear();
        }
        head = match.suffix().str();
    }
    this->suffix = static_text + head;
}

NAMESPACE_CXLOG_END
