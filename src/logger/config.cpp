#include "filter_inl.h"

#include <cxlog/logger/config.h>

NAMESPACE_CXLOG_BEGIN

void LoggerConfig::SetDefaults() {
    this->filter = FillFilter(this->filter);
}

NAMESPACE_CXLOG_END
