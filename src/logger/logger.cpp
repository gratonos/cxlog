#include <cxlog/logger/logger.h>

#include <time.h>

#include <algorithm>
#include <iomanip> // todo: delete

NAMESPACE_CXLOG_BEGIN

void Logger::Log(Level level, const char *file, std::size_t line, const char *func,
    std::string &&msg) const {

    std::vector<Context> contexts;
    contexts.reserve(this->additional.statics->size() + this->additional.dynamics->size());

    for (auto &context : *this->additional.statics) {
        contexts.emplace_back(Context{context.GetKey(), context.GetValue()});
    }
    for (auto &context : *this->additional.dynamics) {
        contexts.emplace_back(Context{context.GetKey(), context.GetValue()});
    }

    std::lock_guard<decltype(this->intrinsic->lock)> lock(this->intrinsic->lock);

    Record record;
    record.time = std::chrono::system_clock::now();
    record.level = level;
    record.file = file;
    record.line = line;
    record.func = func;
    record.msg = std::move(msg);
    record.prefix = *this->additional.prefix;
    record.contexts = std::move(contexts);
    record.mark = this->additional.mark;

    if (this->additional.filter(record) && this->intrinsic->config.GetFilter()(record)) {
        FormatAndWrite(level, record);
    }
}

void Logger::FormatAndWrite(Level /*level*/, const Record &record) const {
    auto time = std::chrono::system_clock::to_time_t(record.time);
    std::tm tm;
    localtime_r(&time, &tm);
    fmt::printf(
        "time: %s, level: %d, file: %s, line: %d, func: %s, msg: %s, prefix: %s, mark: %d\n",
        std::put_time(&tm, "%F %T"), LevelToSize(record.level), record.file,
        record.line, record.func, record.msg, record.prefix, record.mark);

    for (auto &context : record.contexts) {
        fmt::printf("key: %s, value: %s\n", context.key, context.value);
    }
}

NAMESPACE_CXLOG_END
