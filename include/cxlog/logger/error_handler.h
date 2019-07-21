#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

using ErrorHandler =
    std::function<void(const Slice &slice, const Record &record, const Error &error)>;

const ErrorHandler &NullErrorHandler() noexcept;
void Report(const Slice &, const Record &record, const Error &error);
void ReportDetails(const Slice &slice, const Record &, const Error &error);

NAMESPACE_CXLOG_END
