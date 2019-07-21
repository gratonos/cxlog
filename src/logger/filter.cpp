#include "filter_inl.h"

NAMESPACE_CXLOG_BEGIN

const Filter NullFilter = [](const Record &) -> bool { return true; };

const Filter &FillFilter(const Filter &filter) {
    if (filter == nullptr) {
        return NullFilter;
    } else {
        return filter;
    }
}

std::vector<Filter> FillFilters(std::initializer_list<Filter> filter_list) {
    std::vector<Filter> filters(filter_list.begin(), filter_list.end());
    for (auto &filter : filters) {
        if (filter == nullptr) {
            filter = NullFilter;
        }
    }
    return std::move(filters);
}

NAMESPACE_CXLOG_END
