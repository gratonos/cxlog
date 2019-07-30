#include <cxlog/logger/logger.h>

NAMESPACE_CXLOG_BEGIN

void Logger::Log(Level level, const char *file, std::size_t line, const char *func,
    std::string &&msg) const {

    const Additional &additional = this->additional;
    std::vector<Context> contexts;
    contexts.reserve(additional.statics->size() + additional.dynamics->size());

    for (StaticContext &context : *additional.statics) {
        contexts.emplace_back(Context{context.GetKey(), context.GetValue()});
    }
    for (DynamicContext &context : *additional.dynamics) {
        contexts.emplace_back(Context{context.GetKey(), context.GetValue()});
    }

    LockGuard lock(this->intrinsic->lock);

    Record record;
    record.time = std::chrono::system_clock::now();
    record.level = level;
    record.file = file;
    record.line = line;
    record.func = func;
    record.msg = std::move(msg);
    record.prefix = *additional.prefix;
    record.contexts = std::move(contexts);
    record.mark = additional.mark;

    if (additional.filter(record) && this->intrinsic->config.DoFilter(record)) {
        this->FormatAndWrite(level, record);
    }
}

void Logger::FormatAndWrite(Level level, const Record &record) const {
    const Intrinsic &intrinsic = *this->intrinsic;
    std::array<Slice, SLOT_COUNT> slices;
    for (std::size_t i = 0; i < SLOT_COUNT; i++) {
        const Slot &slot = intrinsic.slots[i];
        if (slot.NeedToLog(level, record)) {
            Slice &slice = slices[i];
            if (slice == Slice{}) {
                slice = slot.Format(record);
                for (size_t n : intrinsic.equivalents[i]) {
                    slices[n] = slice;
                }
            }
            slot.Write(slice, record);
        }
    }
}

void Logger::UpdateEquivalents() {
    Intrinsic &intrinsic = *this->intrinsic;
    for (std::size_t i = 0; i < SLOT_COUNT; i++) {
        intrinsic.equivalents[i].clear();
        for (std::size_t j = i + 1; j < SLOT_COUNT; j++) {
            if (intrinsic.slots[i].GetFormatter() == intrinsic.slots[j].GetFormatter()) {
                intrinsic.equivalents[i].push_back(j);
            }
        }
    }
}

NAMESPACE_CXLOG_END
