#include <cxlog/formatter/func.h>
#include <cxlog/formatter/null.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

namespace {
const std::shared_ptr<Formatter> g_nullFormatter = std::make_shared<FormatterFunc>();
}

const std::shared_ptr<Formatter> &NullFormatter() noexcept {
    return g_nullFormatter;
}

const std::shared_ptr<Formatter> &FillFormatter(
    const std::shared_ptr<Formatter> &formatter) noexcept {
    if (formatter == nullptr) {
        return g_nullFormatter;
    } else {
        return formatter;
    }
}

NAMESPACE_CXLOG_END
