#pragma once

#include <cxlog/formatter/text/config.h>
#include <cxlog/iface/iface.h>

#include <fmt/printf.h>

#include <array>
#include <map>
#include <string>

NAMESPACE_CXLOG_BEGIN

class ColorMgr final {
public:
    ColorMgr(const std::map<Level, Color> &color_map, Color mark_color) {
        this->MapColors(color_map);
        this->SetMarkColor(mark_color);
    }

public:
    Color GetColor(Level level) const {
        return this->colors[LevelToSizeT(level)];
    }
    void SetColor(Level level, Color color) {
        const std::size_t index = LevelToSizeT(level);
        this->colors[index] = color;
        this->color_seqs[index] = MakeColorSeq(color);
    }
    void MapColors(const std::map<Level, Color> &color_map) {
        for (const auto &pair : color_map) {
            this->SetColor(pair.first, pair.second);
        }
    }
    Color GetMarkColor() const {
        return this->mark_color;
    }
    void SetMarkColor(Color color) {
        this->mark_color = color;
        this->mark_color_seq = MakeColorSeq(color);
    }

private:
    static std::string MakeColorSeq(Color color) {
        return fmt::sprintf("\033[%zm", ColorToSizeT(color));
    }

private:
    const std::string ResetSeq = "\033[0m";

private:
    std::array<Color, LogLevelCount> colors;
    Color mark_color;

    std::array<std::string, LogLevelCount> color_seqs;
    std::string mark_color_seq;
};

NAMESPACE_CXLOG_END
