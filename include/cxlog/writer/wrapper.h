#pragma once

#include <cxlog/iface/iface.h>

#include <ostream>

NAMESPACE_CXLOG_BEGIN

class WriterWrapper final : public Writer {
public:
    WriterWrapper(std::ostream &stream) : stream(stream) {}

public:
    Error Write(const std::string &log, const Record &) override {
        this->stream << log;
        return Error{}; // TODO: handle error
    }

private:
    std::ostream &stream;
};

NAMESPACE_CXLOG_END
