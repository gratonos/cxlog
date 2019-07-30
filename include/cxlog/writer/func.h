#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

class WriterFunc final : public Writer {
public:
    using Func = std::function<Error(const std::string &log, const Record &record)>;

public:
    WriterFunc(const Func &func = nullptr) {
        if (func == nullptr) {
            this->func = [](const std::string &, const Record &) -> Error { return Error{}; };
        } else {
            this->func = func;
        }
    }

public:
    Error Write(const std::string &log, const Record &record) override {
        return this->func(log, record);
    }

private:
    Func func;
};

NAMESPACE_CXLOG_END
