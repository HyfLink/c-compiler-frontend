#pragma once

#include "type_traits.hpp"

namespace eztl
{
    template <typename T>
    constexpr T &&forward(remove_reference_t<T> &val) { return static_cast<T &&>(val); }

    template <typename T>
    constexpr T &&forward(remove_reference_t<T> &&val) { return static_cast<T &&>(val); }

    template <typename T>
    constexpr remove_reference_t<T> &&move(T &&val) { return static_cast<remove_reference_t<T> &&>(val); }

} // namespace eztl
