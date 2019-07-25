#include <cxlog/logger/filter.h>

NAMESPACE_CXLOG_BEGIN

namespace {
const Filter g_nullFilter = [](const Record &) -> bool { return true; };
}

const Filter &NullFilter() noexcept {
    return g_nullFilter;
}

const Filter &FillFilter(const Filter &filter) noexcept {
    if (filter == nullptr) {
        return g_nullFilter;
    } else {
        return filter;
    }
}

std::vector<Filter> FillFilters(std::initializer_list<Filter> filter_list) {
    std::vector<Filter> filters(filter_list.begin(), filter_list.end());
    for (auto &filter : filters) {
        if (filter == nullptr) {
            filter = g_nullFilter;
        }
    }
    return std::move(filters);
}

NAMESPACE_CXLOG_END
