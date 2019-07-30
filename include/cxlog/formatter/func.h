#pragma once

#include <cxlog/iface/iface.h>

#include <functional>

NAMESPACE_CXLOG_BEGIN

class FormatterFunc final : public Formatter {
public:
    using Func = std::function<std::string(const Record &record)>;

public:
    FormatterFunc(const Func &func = nullptr) {
        if (func == nullptr) {
            this->func = [](const Record &) -> std::string { return std::string(); };
        } else {
            this->func = func;
        }
    }

public:
    std::string Format(const Record &record) override {
        return this->func(record);
    }

private:
    Func func;
};

NAMESPACE_CXLOG_END
