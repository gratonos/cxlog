#pragma once

#include <cxlog/iface/iface.h>

#include <memory>

NAMESPACE_CXLOG_BEGIN

const std::shared_ptr<Writer> &NullWriter() noexcept;
const std::shared_ptr<Writer> &FillWriter(const std::shared_ptr<Writer> &writer) noexcept;

NAMESPACE_CXLOG_END
