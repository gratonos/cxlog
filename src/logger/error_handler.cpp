#include "error_handler_inl.h"

#include <iostream>
#include <sstream>

NAMESPACE_CXLOG_BEGIN

const ErrorHandler g_nullErrorHandler = [](const Slice &, const Record &, const Error &) {};

const ErrorHandler &NullErrorHandler() noexcept {
    return g_nullErrorHandler;
}

const ErrorHandler &FillErrorHandler(const ErrorHandler &handler) noexcept {
    if (handler == nullptr) {
        return g_nullErrorHandler;
    } else {
        return handler;
    }
}

inline void OutputError(std::ostringstream &stream, const Error &error) {
    stream << "cxlog error: errno: " << error.error << ", desc: " << error.desc;
}

inline void OutputBasic(std::ostringstream &stream, const Record &record) {
    stream << ", file: " << record.file << ", line: " << record.line
           << ", func: " << record.func;
}

inline void OutputDetails(std::ostringstream &stream, const Slice &slice) {
    stream << ", log: ";
    stream.write(slice.ptr, slice.len);
}

void Report(const Slice &, const Record &record, const Error &error) {
    std::ostringstream stream;
    OutputError(stream, error);
    OutputBasic(stream, record);
    const std::string &str = stream.str();
    std::cerr.write(str.c_str(), str.length());
}

void ReportDetails(const Slice &slice, const Record &, const Error &error) {
    std::ostringstream stream;
    OutputError(stream, error);
    OutputDetails(stream, slice);
    const std::string &str = stream.str();
    std::cerr.write(str.c_str(), str.length());
}

NAMESPACE_CXLOG_END
