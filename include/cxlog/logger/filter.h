#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

using Filter = std::function<bool(const Record &record)>;

const Filter &NullFilter() noexcept;

NAMESPACE_CXLOG_END
