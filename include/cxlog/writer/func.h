#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

class WriterFunc final : public Writer {
public:
    using Func = std::function<Error(const Slice &slice, const Record &record)>;

public:
    WriterFunc(const Func &func = nullptr) {
        if (func == nullptr) {
            this->func = [](const Slice &, const Record &) -> Error { return Error{}; };
        } else {
            this->func = func;
        }
    }

public:
    Error Write(const Slice &slice, const Record &record) override {
        return this->func(slice, record);
    }

private:
    Func func;
};

NAMESPACE_CXLOG_END
