#include <cxlog/formatter/text/time_formatter.h>
#include <cxlog/util.h>

NAMESPACE_CXLOG_BEGIN

std::tuple<std::string, TimeFormatter::FractionFormatFunc> TimeFormatter::ParseProperty(
    const std::string &property) {

    if (property.find('%') != std::string::npos) {
        return std::make_tuple(property, DoNothing);
    } else {
        return ParseTimeOptions(ToLower(property));
    }
}

std::tuple<std::string, TimeFormatter::FractionFormatFunc> TimeFormatter::ParseTimeOptions(
    const std::string &property) {

    static constexpr const char *DateFmtSpec = "%F %T"; // %Y-%m-%d %H:%M:%S
    static constexpr const char *TimeFmtSpec = "%T"; // %H:%M:%S

    std::string time_type;
    std::string fraction_type;
    const auto pos = property.find('.');
    if (pos == std::string::npos) {
        time_type = property;
    } else {
        time_type = property.substr(0, pos);
        fraction_type = property.substr(pos + 1);
    }

    std::string fmtspec;
    if (time_type == "date") {
        fmtspec = DateFmtSpec;
    } else if (time_type == "time") {
        fmtspec = TimeFmtSpec;
    } else {
        return std::make_tuple(DateFmtSpec, FormatMicro);
    }

    FractionFormatFunc fmtfunc;
    if (fraction_type == "ms") {
        fmtfunc = FormatMilli;
    } else if (fraction_type == "us") {
        fmtfunc = FormatMicro;
    } else if (fraction_type == "ns") {
        fmtfunc = FormatNano;
    } else {
        fmtfunc = DoNothing;
    }

    return std::make_tuple(std::move(fmtspec), std::move(fmtfunc));
}

NAMESPACE_CXLOG_END
