#include <cxlog/writer/func.h>
#include <cxlog/writer/null.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

namespace {
const std::shared_ptr<Writer> g_nullWriter = std::make_shared<WriterFunc>();
}

const std::shared_ptr<Writer> &NullWriter() noexcept {
    return g_nullWriter;
}

const std::shared_ptr<Writer> &FillWriter(const std::shared_ptr<Writer> &writer) noexcept {
    if (writer == nullptr) {
        return g_nullWriter;
    } else {
        return writer;
    }
}

NAMESPACE_CXLOG_END
