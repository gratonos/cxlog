#pragma once

#include <cxlog/iface/iface.h>

#include <ostream>

NAMESPACE_CXLOG_BEGIN

class WriterWrapper final : public Writer {
public:
    WriterWrapper(std::ostream &stream) : stream(stream) {}

public:
    Error Write(const Slice &slice, const Record &) override {
        this->stream.write(slice.ptr, slice.len);
        return Error{}; // TODO: handle error
    }

private:
    std::ostream &stream;
};

NAMESPACE_CXLOG_END
