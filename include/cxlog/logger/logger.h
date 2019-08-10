#pragma once

#include <cxlog/logger/config.h>
#include <cxlog/logger/context.h>
#include <cxlog/logger/pretty.h>
#include <cxlog/logger/slot.h>
#include <cxlog/logger/timing.h>

#include <fmt/printf.h>

#include <algorithm>
#include <array>
#include <cstdlib>
#include <memory>
#include <mutex>

NAMESPACE_CXLOG_BEGIN

// A Logger is a logging framework that contains EIGHT slots. Each Slot contains
// a Formatter and a Writer. A Logger has its own level and filter while each
// Slot has its independent level and filter. Logger calls the Formatter and
// Writer of each Slot in the order from Slot0 to Slot7 when a log is emitted.
class Logger final {
private:
    struct Additional final {
        Level level = Level::Trace;
        Filter filter = NullFilter();
        std::shared_ptr<std::string> prefix = std::make_shared<std::string>();
        std::shared_ptr<std::vector<StaticContext>> statics =
            std::make_shared<std::vector<StaticContext>>();
        std::shared_ptr<std::vector<DynamicContext>> dynamics =
            std::make_shared<std::vector<DynamicContext>>();
        bool mark = false;
    };
    struct Intrinsic final {
        LoggerConfig config;
        std::array<Slot, SlotCount> slots;
        // indexes of equivalent formatters, used to avoid duplicated formatting
        std::array<std::vector<std::size_t>, SlotCount> equivalents;
        std::recursive_mutex lock;
    };
    using LockGuard = std::lock_guard<std::recursive_mutex>;

public:
    Logger(const LoggerConfig &config) {
        this->intrinsic->config = config;
    }
    Logger(const Logger &logger) = default;
    Logger &operator=(const Logger &logger) = delete;
    Logger(Logger &&logger) noexcept = default;
    Logger &operator=(Logger &&logger) noexcept = delete;

public:
    Logger WithLevel(Level level) const & {
        Logger logger(*this);
        logger.additional.level = level;
        return std::move(logger);
    }
    Logger WithLevel(Level level) && noexcept {
        this->additional.level = level;
        return std::move(*this);
    }
    Logger WithFilter(const Filter &filter) const & {
        Logger logger(*this);
        logger.additional.filter = FillFilter(filter);
        return std::move(logger);
    }
    Logger WithFilter(const Filter &filter) && {
        this->additional.filter = FillFilter(filter);
        return std::move(*this);
    }
    template <typename T>
    Logger WithPrefix(T &&prefix) const & {
        Logger logger(*this);
        logger.additional.prefix =
            std::make_shared<std::string>(fmt::to_string(std::forward<T>(prefix)));
        return std::move(logger);
    }
    template <typename T>
    Logger WithPrefix(T &&prefix) && {
        this->additional.prefix =
            std::make_shared<std::string>(fmt::to_string(std::forward<T>(prefix)));
        return std::move(*this);
    }
    Logger WithStatics(std::initializer_list<StaticContext> static_list) const & {
        Logger logger(*this);
        logger.additional.statics = CopyAppend(logger.additional.statics, static_list);
        return std::move(logger);
    }
    Logger WithStatics(std::initializer_list<StaticContext> static_list) && {
        this->additional.statics = CopyAppend(this->additional.statics, static_list);
        return std::move(*this);
    }
    Logger WithDynamics(std::initializer_list<DynamicContext> dynamic_list) const & {
        Logger logger(*this);
        logger.additional.dynamics = CopyAppend(logger.additional.dynamics, dynamic_list);
        return std::move(logger);
    }
    Logger WithDynamics(std::initializer_list<DynamicContext> dynamic_list) && {
        this->additional.dynamics = CopyAppend(this->additional.dynamics, dynamic_list);
        return std::move(*this);
    }
    Logger WithMark(bool mark) const & {
        Logger logger(*this);
        logger.additional.mark = mark;
        return std::move(logger);
    }
    Logger WithMark(bool mark) && noexcept {
        this->additional.mark = mark;
        return std::move(*this);
    }

public:
    Level GetLevel() const noexcept {
        return this->intrinsic->config.GetLevel();
    }
    void SetLevel(Level level) noexcept {
        this->intrinsic->config.SetLevel(level);
    }
    Filter GetFilter() const {
        LockGuard lock(this->intrinsic->lock);
        return this->intrinsic->config.GetFilter();
    }
    void SetFilter(const Filter &filter) {
        LockGuard lock(this->intrinsic->lock);
        this->intrinsic->config.SetFilter(filter);
    }

public:
    template <typename S, typename... Args>
    void Logf(Level level, const char *file, std::size_t line, const char *func, S &&format,
        Args &&... args) const {

        if (NeedToLog(level)) {
            Log(level, file, line, func,
                fmt::sprintf(std::forward<S>(format), std::forward<Args>(args)...));
        }
    }
    template <typename S, typename... Args>
    TimingDone Timingf(Level level, const char *file, std::size_t line, const char *func,
        S &&format, Args &&... args) const {

        if (NeedToLog(level)) {
            std::string msg =
                fmt::sprintf(std::forward<S>(format), std::forward<Args>(args)...);
            auto start = Clock::now();
            return [logger = Logger(*this), level, file, line, func, msg = std::move(msg),
                       start = std::move(start)] {
                auto cost = Clock::now() - start;
                logger.Log(level, file, line, func,
                    fmt::sprintf("%s (cost: %s)", std::move(msg), Pretty(cost)));
            };
        } else {
            return [] {};
        }
    }

public:
    Slot GetSlot(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index);
    }
    void SetSlot(SlotIndex index, const Slot &slot) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index) = slot;
        this->UpdateEquivalents();
    }
    void UpdateSlot(SlotIndex index, const std::function<Slot(Slot)> &func) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index) = func(this->SlotRef(index));
        this->UpdateEquivalents();
    }
    void ResetSlot(SlotIndex index) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index) = Slot{};
        this->UpdateEquivalents();
    }
    void ResetAllSlots() {
        LockGuard lock(this->intrinsic->lock);
        for (Slot &slot : this->intrinsic->slots) {
            slot = Slot{};
        }
        this->UpdateEquivalents();
    }
    void CopySlot(SlotIndex dst, SlotIndex src) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(dst) = this->SlotRef(src);
        this->UpdateEquivalents();
    }
    void MoveSlot(SlotIndex to, SlotIndex from) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(to) = std::move(this->SlotRef(from));
        this->SlotRef(from) = Slot{};
        this->UpdateEquivalents();
    }
    void SwapSlot(SlotIndex left, SlotIndex right) {
        LockGuard lock(this->intrinsic->lock);
        Slot slot = std::move(this->SlotRef(left));
        this->SlotRef(left) = std::move(this->SlotRef(right));
        this->SlotRef(right) = std::move(slot);
        this->UpdateEquivalents();
    }
    std::shared_ptr<Formatter> GetSlotFormatter(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index).GetFormatter();
    }
    void SetSlotFormatter(SlotIndex index, const std::shared_ptr<Formatter> &formatter) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index).SetFormatter(formatter);
        this->UpdateEquivalents();
    }
    std::shared_ptr<Writer> GetSlotWriter(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index).GetWriter();
    }
    void SetSlotWriter(SlotIndex index, const std::shared_ptr<Writer> &writer) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index).SetWriter(writer);
    }
    Level GetSlotLevel(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index).GetLevel();
    }
    void SetSlotLevel(SlotIndex index, Level level) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index).SetLevel(level);
    }
    Filter GetSlotFilter(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index).GetFilter();
    }
    void SetSlotFilter(SlotIndex index, const Filter &filter) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index).SetFilter(filter);
    }
    ErrorHandler GetSlotErrorHandler(SlotIndex index) const {
        LockGuard lock(this->intrinsic->lock);
        return this->SlotRef(index).GetErrorHandler();
    }
    void SetSlotErrorHandler(SlotIndex index, const ErrorHandler &handler) {
        LockGuard lock(this->intrinsic->lock);
        this->SlotRef(index).SetErrorHandler(handler);
    }

private:
    template <typename T>
    static std::shared_ptr<std::vector<T>> CopyAppend(
        const std::shared_ptr<std::vector<T>> &origin, std::initializer_list<T> list) {

        auto clone = std::make_shared<std::vector<T>>();
        clone->reserve(origin->size() + list.size());
        std::copy(std::begin(*origin), std::end(*origin), std::back_inserter(*clone));
        std::copy(std::begin(list), std::end(list), std::back_inserter(*clone));
        return std::move(clone);
    }

private:
    bool NeedToLog(Level level) const {
        if (level == Level::Off) {
            throw std::invalid_argument("cxlog: invalid log level");
        }
        return this->additional.level <= level && this->GetLevel() <= level;
    }
    void Log(Level level, const char *file, std::size_t line, const char *func,
        std::string &&msg) const;
    void FormatAndWrite(Level level, const Record &record) const;
    const Slot &SlotRef(SlotIndex index) const {
        return this->intrinsic->slots[SlotToSizeT(index)];
    }
    Slot &SlotRef(SlotIndex index) {
        return this->intrinsic->slots[SlotToSizeT(index)];
    }
    void UpdateEquivalents();

private:
    Additional additional; // copy on write, concurrency safe
    std::shared_ptr<Intrinsic> intrinsic = std::make_shared<Intrinsic>();
};

NAMESPACE_CXLOG_END
