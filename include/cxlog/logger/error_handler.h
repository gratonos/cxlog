#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

using ErrorHandler =
    std::function<void(const std::string &log, const Record &record, const Error &error)>;

const ErrorHandler &NullErrorHandler() noexcept;
const ErrorHandler &FillErrorHandler(const ErrorHandler &handler) noexcept;

void Report(const std::string &, const Record &record, const Error &error);
void ReportDetails(const std::string &log, const Record &, const Error &error);

NAMESPACE_CXLOG_END
