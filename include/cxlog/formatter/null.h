#pragma once

#include <cxlog/iface/iface.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

const std::shared_ptr<Formatter> &NullFormatter() noexcept;
const std::shared_ptr<Formatter> &FillFormatter(
    const std::shared_ptr<Formatter> &formatter) noexcept;

NAMESPACE_CXLOG_END
