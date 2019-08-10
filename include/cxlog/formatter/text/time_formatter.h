#pragma once

#include <cxlog/formatter/text/element_formatter.h>
#include <cxlog/formatter/text/fmt.h>
#include <cxlog/iface/iface.h>
#include <cxlog/util.h>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <ctime>
#include <stdexcept>
#include <string>
#include <tuple>

NAMESPACE_CXLOG_BEGIN

class TimeFormatter final : public ElementFormatter {
private:
    using FractionFormatFunc =
        std::function<void(fmt::memory_buffer &buf, const Clock::time_point &time)>;

public:
    TimeFormatter(const std::string &property, const std::string &fmtstr) {
        std::tie(this->fmtspec, this->fmtfunc) = ParseProperty(property);
        this->fmtstr = (fmtstr.empty() ? "%s" : fmtstr);
    }

public:
    void FormatElement(fmt::memory_buffer &buf, const Record &record) override {
        this->buf.clear();
        this->FormatTime(record.time);
        fmt::bprintf(buf, this->fmtstr, this->buf.data());
    }

private:
    static std::tuple<std::string, FractionFormatFunc> ParseProperty(
        const std::string &property);
    static std::tuple<std::string, FractionFormatFunc> ParseTimeOptions(
        const std::string &property);
    static void DoNothing(fmt::memory_buffer &, const Clock::time_point &) {}
    static void FormatMilli(fmt::memory_buffer &buf, const Clock::time_point &time) {
        fmt::format_to(buf, ".{}", MilliSeconds(time.time_since_epoch()));
    }
    static void FormatMicro(fmt::memory_buffer &buf, const Clock::time_point &time) {
        fmt::format_to(buf, ".{}", MicroSeconds(time.time_since_epoch()));
    }
    static void FormatNano(fmt::memory_buffer &buf, const Clock::time_point &time) {
        fmt::format_to(buf, ".{}", NanoSeconds(time.time_since_epoch()));
    }

private:
    void FormatTime(const Clock::time_point &time) {
        static constexpr std::size_t BufSize = 128;
        char time_desc[BufSize];
        std::tm tm = fmt::localtime(Clock::to_time_t(time));
        if (std::strftime(time_desc, sizeof(time_desc), this->fmtspec.c_str(), &tm) == 0) {
            throw std::out_of_range("cxlog: time desc buffer out of range");
        }
        fmt::format_to(this->buf, "{}", time_desc);
        this->fmtfunc(this->buf, time);
    }

private:
    std::string fmtspec;
    std::string fmtstr;
    FractionFormatFunc fmtfunc;
    fmt::memory_buffer buf;
};

NAMESPACE_CXLOG_END
