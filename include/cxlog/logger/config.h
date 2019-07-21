#pragma once

#include <cxlog/iface/iface.h>
#include <cxlog/logger/filter.h>
#include <cxlog/macro.h>

NAMESPACE_CXLOG_BEGIN

struct LoggerConfig final {
    void SetDefaults();

    Level level   = Level::Trace;
    Filter filter = nullptr;
};

NAMESPACE_CXLOG_END
