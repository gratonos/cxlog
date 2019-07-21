#pragma once

#include <cxlog/logger/filter.h>

#include <initializer_list>
#include <vector>

NAMESPACE_CXLOG_BEGIN

const Filter &FillFilter(const Filter &filter) noexcept;
std::vector<Filter> FillFilters(std::initializer_list<Filter> filters);

NAMESPACE_CXLOG_END
