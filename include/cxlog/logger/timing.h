#pragma once

#include <cxlog/macro.h>

#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <string>

NAMESPACE_CXLOG_BEGIN

using TimingDone = std::function<void()>;

class Timing final {
public:
    Timing(const TimingDone &done_func) : done_func(done_func) {}
    Timing(TimingDone &&done_func) noexcept : done_func(std::move(done_func)) {}
    ~Timing() {
        this->Done();
    }

public:
    void Done() noexcept {
        bool expected = false;
        if (this->is_done.compare_exchange_strong(expected, true)) {
            try {
                this->done_func();
            } catch (const std::exception &e) {
                std::cerr << std::string("cxlog: Timing exception: ") + e.what() << std::endl;
            } catch (...) {
                std::cerr << "cxlog: Timing unknown exception" << std::endl;
            }
        }
    }
    void Cancel() noexcept {
        this->is_done = true;
    }

private:
    std::atomic_bool is_done{false};
    TimingDone done_func;
};

NAMESPACE_CXLOG_END
