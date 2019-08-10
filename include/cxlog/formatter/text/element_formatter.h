#pragma once

#include <cxlog/iface/iface.h>

#include <fmt/format.h>

NAMESPACE_CXLOG_BEGIN

class ElementFormatter {
public:
    virtual void FormatElement(fmt::memory_buffer &buf, const Record &record) = 0;
    virtual ~ElementFormatter() = default;
};

NAMESPACE_CXLOG_END
