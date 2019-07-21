#pragma once

#include <cxlog/logger/filter.h>

#include <initializer_list>

NAMESPACE_CXLOG_BEGIN

Filter And(std::initializer_list<Filter> filter_list);
Filter Or(std::initializer_list<Filter> filter_list);
Filter Not(Filter filter);

NAMESPACE_CXLOG_END
