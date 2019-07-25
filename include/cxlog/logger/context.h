#pragma once

#include <fmt/format.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

NAMESPACE_CXLOG_BEGIN

// ATTENTION: You SHOULD be careful about concurrency safety or deadlocks with dynamic contexts.
using Dynamic = std::function<std::string() noexcept>;

class StaticContext final {
public:
    template <typename K, typename V>
    StaticContext(K &&key, V &&value) {
        this->key = fmt::to_string(std::forward<K>(key));
        this->value = fmt::to_string(std::forward<V>(value));
    }

public:
    const std::string &GetKey() const noexcept {
        return this->key;
    }
    const std::string &GetValue() const noexcept {
        return this->value;
    }

private:
    std::string key;
    std::string value;
};

class DynamicContext final {
public:
    template <typename K>
    DynamicContext(K &&key, const Dynamic &value) {
        if (value == nullptr) {
            throw std::invalid_argument("cxlog: null dynamic value");
        }
        this->key = fmt::to_string(std::forward<K>(key));
        this->value = value;
    }

public:
    const std::string &GetKey() const noexcept {
        return this->key;
    }
    std::string GetValue() const noexcept {
        return this->value();
    }

private:
    std::string key;
    Dynamic value;
};

NAMESPACE_CXLOG_END
