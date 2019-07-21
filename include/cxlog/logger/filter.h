#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

// Do NOT call any method of the Logger within a Filter, or it may deadlock.
using Filter = std::function<bool(const Record &record)>;

extern const Filter NullFilter;

NAMESPACE_CXLOG_END
