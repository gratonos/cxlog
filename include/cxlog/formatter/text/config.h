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

const std::string FullHeader = "{{time}} {{level}} {{file}}:{{line}} {{pkg}}.{{func}} "
                               "{{prefix}}[{{context}}] {{msg}}\n";
const std::string CompactHeader = "{{time:time.us}} {{level}} {{file:1}}:{{line}} "
                                  "{{pkg}}.{{func}} {{prefix}}[{{context}}] {{msg}}\n";
const std::string SyslogHeader = "{{file:1}}:{{line}} {{pkg}}.{{func}} "
                                 "{{prefix}}[{{context}}] {{msg}}\n";

struct TextConfig final {
    std::string header = CompactHeader;
    std::map<Level, Color> color_map;
    Color mark_color = Color::Magenta;
    bool coloring = true;
};

NAMESPACE_CXLOG_END
