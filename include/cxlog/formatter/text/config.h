#pragma once

#include <cxlog/iface/iface.h>

#include <map>
#include <string>

NAMESPACE_CXLOG_BEGIN

enum class Color : std::size_t {
    Black = 30,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
};

constexpr std::size_t ColorToSizeT(Color color) noexcept {
    return static_cast<std::size_t>(color);
}

const std::string FullHeader = "{{time}} {{level}} {{file}}:{{line}} {{func}} "
                               "{{prefix}}[{{context}}] {{msg}}\n";
const std::string StdHeader = "{{time}} {{level}} {{file:2}}:{{line}} {{func}} "
                              "{{prefix}}[{{context}}] {{msg}}\n";
const std::string CompactHeader = "{{time:time.us}} {{level}} {{file:2}}:{{line}} "
                                  "{{func}} {{prefix}}[{{context}}] {{msg}}\n";
const std::string SyslogHeader = "{{file:2}}:{{line}} {{func}} "
                                 "{{prefix}}[{{context}}] {{msg}}\n";

struct TextConfig final {
    TextConfig &Header(const std::string &header) & {
        this->header = header;
        return *this;
    }
    TextConfig Header(const std::string &header) && {
        this->header = header;
        return std::move(*this);
    }
    TextConfig &ColorMap(const std::map<Level, Color> &color_map) & {
        this->color_map = color_map;
        return *this;
    }
    TextConfig ColorMap(const std::map<Level, Color> &color_map) && {
        this->color_map = color_map;
        return std::move(*this);
    }
    TextConfig &MarkColor(Color mark_color) & {
        this->mark_color = mark_color;
        return *this;
    }
    TextConfig MarkColor(Color mark_color) && {
        this->mark_color = mark_color;
        return std::move(*this);
    }
    TextConfig &Coloring(bool coloring) & {
        this->coloring = coloring;
        return *this;
    }
    TextConfig Coloring(bool coloring) && {
        this->coloring = coloring;
        return std::move(*this);
    }
    // The header is the format specifier of a text formatter.
    // It is used to specify which and how the fields of a Record to be formatted.
    // The pattern of a field specifier is {{<name>[:property][%fmtstr]}}.
    // e.g. {{level:char}}, {{line%05zd}}, {{file:1}}, {{context:list%40s}}
    // All fields have support for the fmtstr. If the fmtstr is NOT the default one
    // of a field, it will be passed to fmt.Sprintf to format the field and this
    // affects the performance a little.
    // The supported properties vary with fields.
    // All supported fields are as the follows:
    //    name    | supported property       | defaults     | property examples
    //  ----------+--------------------------+--------------+------------------------
    //    time    | <date|time>[.ms|.us|.ns] | "date.us" %s | "date.ns", "time"
    //            | format that is supported |              | "%Y-%m-%d %H:%M:%S"
    //            |   by std::strftime       |              |
    //    level   | <full|char>              | "full"    %s | "full", "char"
    //    file    | <lastSegs>               | 0         %s | 0, 1, 2, ...
    //    line    |                          |          %zd |
    //    func    |                          |           %s |
    //    prefix  |                          |           %s |
    //    context | <pair|list>              | "pair"    %s | "pair", "list"
    //    msg     |                          |           %s |
    std::string header = StdHeader;
    std::map<Level, Color> color_map;
    Color mark_color = Color::Magenta;
    bool coloring = true;
};

NAMESPACE_CXLOG_END
