#pragma once

#include <cxlog/formatter/text/color_mgr.h>
#include <cxlog/formatter/text/config.h>
#include <cxlog/formatter/text/header_appender.h>
#include <cxlog/iface/iface.h>

#include <fmt/core.h>

#include <mutex>
#include <string>
#include <vector>

NAMESPACE_CXLOG_BEGIN

class TextFormatter final : public Formatter {
private:
    using LockGuard = std::lock_guard<std::recursive_mutex>;

public:
    TextFormatter(const TextConfig &config) : color_mgr(config.color_map, config.mark_color) {
        this->coloring = config.coloring;
        this->SetHeader(config.header);
    }

public:
    std::string Format(const Record &record) override;

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
    bool AddAppender(const std::string &element, const std::string &property,
        const std::string &fmtstr, const std::string &static_text) {
        try {
            this->appenders.emplace_back(element, property, fmtstr, static_text);
        } catch (const std::invalid_argument &) {
            return false;
        }
        return true;
    }

private:
    std::string header;
    bool coloring;

    ColorMgr color_mgr;
    std::vector<HeaderAppender> appenders;
    std::string suffix;
    fmt::memory_buffer buf;

    mutable std::recursive_mutex lock;
};

NAMESPACE_CXLOG_END
