#pragma once

#include <cxlog/formatter/text/color_mgr.h>
#include <cxlog/formatter/text/config.h>
#include <cxlog/iface/iface.h>

#include <mutex>

NAMESPACE_CXLOG_BEGIN

class TextFormatter final : public Formatter {
public:
    static std::map<Level, Color> DefaultColorMapping() {
        return std::map<Level, Color>{
            {Level::Trace, Color::Green},
            {Level::Debug, Color::Green},
            {Level::Info, Color::Green},
            {Level::Warn, Color::Yellow},
            {Level::Error, Color::Red},
            {Level::Fatal, Color::Red},
        };
    }

private:
    using LockGuard = std::lock_guard<std::recursive_mutex>;

public:
    TextFormatter(const TextConfig &config) : color_mgr(config.color_map, config.mark_color) {
        this->header = config.header;
        this->coloring = config.coloring;
    }

public:
    std::string Format(const Record &record) override {
        return record.msg;
    }

public:
    std::string GetHeader() const {
        LockGuard lock(this->lock);
        return this->header;
    }
    void SetHeader(const std::string &header);
    bool GetColoring() const {
        LockGuard lock(this->lock);
        return this->coloring;
    }
    void SetColoring(bool coloring) {
        LockGuard lock(this->lock);
        this->coloring = coloring;
    }
    Color GetColor(Level level) const {
        LockGuard lock(this->lock);
        return this->color_mgr.GetColor(level);
    }
    void SetColor(Level level, Color color) {
        LockGuard lock(this->lock);
        this->color_mgr.SetColor(level, color);
    }
    void MapColors(const std::map<Level, Color> &color_map) {
        LockGuard lock(this->lock);
        this->color_mgr.MapColors(color_map);
    }
    Color GetMarkColor() const {
        LockGuard lock(this->lock);
        return this->color_mgr.GetMarkColor();
    }
    void SetMarkColor(Color color) {
        LockGuard lock(this->lock);
        this->color_mgr.SetMarkColor(color);
    }

private:
    std::string header;
    bool coloring;

    ColorMgr color_mgr;

    mutable std::recursive_mutex lock;
};

NAMESPACE_CXLOG_END
