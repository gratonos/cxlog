#include "filter_inl.h"

#include <cxlog/logger/logic.h>

NAMESPACE_CXLOG_BEGIN

Filter And(std::initializer_list<Filter> filter_list) {
    std::vector<Filter> filters = FillFilters(filter_list);
    return [filters = std::move(filters)](const Record &record) -> bool {
        for (auto &filter : filters) {
            if (!filter(record)) {
                return false;
            }
        }
        return true;
    };
}

Filter Or(std::initializer_list<Filter> filter_list) {
    std::vector<Filter> filters = FillFilters(filter_list);
    return [filters = std::move(filters)](const Record &record) -> bool {
        for (auto &filter : filters) {
            if (filter(record)) {
                return true;
            }
        }
        return false;
    };
}

Filter Not(Filter filter) {
    filter = FillFilter(filter);
    return [filter](const Record &record) -> bool { return !filter(record); };
}

NAMESPACE_CXLOG_END
