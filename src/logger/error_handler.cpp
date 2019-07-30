#include <cxlog/logger/error_handler.h>

#include <iostream>
#include <sstream>

NAMESPACE_CXLOG_BEGIN

namespace {
const ErrorHandler g_nullHandler = [](const std::string &, const Record &, const Error &) {};
}

const ErrorHandler &NullErrorHandler() noexcept {
    return g_nullHandler;
}

const ErrorHandler &FillErrorHandler(const ErrorHandler &handler) noexcept {
    if (handler == nullptr) {
        return g_nullHandler;
    } else {
        return handler;
    }
}

inline void OutputError(std::ostringstream &stream, const Error &error) {
    stream << "cxlog error: errno: " << error.error << ", desc: " << error.desc;
}

inline void OutputBasic(std::ostringstream &stream, const Record &record) {
    stream << ", file: " << record.file << ", line: " << record.line
           << ", func: " << record.func << std::endl;
}

inline void OutputDetails(std::ostringstream &stream, const std::string &log) {
    stream << ", log: ";
    stream << log << std::endl;
}

void Report(const std::string &, const Record &record, const Error &error) {
    std::ostringstream stream;
    OutputError(stream, error);
    OutputBasic(stream, record);
    std::cerr << stream.str();
}

void ReportDetails(const std::string &log, const Record &, const Error &error) {
    std::ostringstream stream;
    OutputError(stream, error);
    OutputDetails(stream, log);
    std::cerr << stream.str();
}

NAMESPACE_CXLOG_END
