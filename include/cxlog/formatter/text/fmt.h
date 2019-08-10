#pragma once

#include <fmt/printf.h>

namespace fmt {

template <typename S, typename Char = char_t<S>>
inline void vbprintf(basic_memory_buffer<Char> &buffer, const S &format,
    basic_format_args<basic_printf_context_t<Char>> args) {

    printf(buffer, to_string_view(format), args);
}

template <typename S, typename... Args,
    typename Char = enable_if_t<internal::is_string<S>::value, char_t<S>>>
inline void bprintf(basic_memory_buffer<Char> &buffer, const S &format, const Args &... args) {
    using context = basic_printf_context_t<Char>;
    vbprintf(buffer, to_string_view(format), {make_format_args<context>(args...)});
}

} // namespace fmt
