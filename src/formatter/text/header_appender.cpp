#include <cxlog/formatter/text/context_formatter.h>
#include <cxlog/formatter/text/file_formatter.h>
#include <cxlog/formatter/text/func_formatter.h>
#include <cxlog/formatter/text/header_appender.h>
#include <cxlog/formatter/text/level_formatter.h>
#include <cxlog/formatter/text/line_formatter.h>
#include <cxlog/formatter/text/msg_formatter.h>
#include <cxlog/formatter/text/prefix_formatter.h>
#include <cxlog/formatter/text/time_formatter.h>

#include <stdexcept>

NAMESPACE_CXLOG_BEGIN

std::unique_ptr<ElementFormatter> HeaderAppender::CreateElementFormatter(
    const std::string &element, const std::string &property, const std::string &fmtstr) {

    if (element == "time") {
        return std::make_unique<TimeFormatter>(property, fmtstr);
    } else if (element == "level") {
        return std::make_unique<LevelFormatter>(property, fmtstr);
    } else if (element == "file") {
        return std::make_unique<FileFormatter>(property, fmtstr);
    } else if (element == "line") {
        return std::make_unique<LineFormatter>(fmtstr);
    } else if (element == "func") {
        return std::make_unique<FuncFormatter>(fmtstr);
    } else if (element == "msg") {
        return std::make_unique<MsgFormatter>(fmtstr);
    } else if (element == "prefix") {
        return std::make_unique<PrefixFormatter>(fmtstr);
    } else if (element == "context") {
        return std::make_unique<ContextFormatter>(property, fmtstr);
    } else {
        throw std::invalid_argument("unknown element: " + element);
    }
}

NAMESPACE_CXLOG_END
