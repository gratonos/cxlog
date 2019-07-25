#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

using Filter = std::function<bool(const Record &record)>;

const Filter &NullFilter() noexcept;
const Filter &FillFilter(const Filter &filter) noexcept;
std::vector<Filter> FillFilters(std::initializer_list<Filter> filters);

NAMESPACE_CXLOG_END
