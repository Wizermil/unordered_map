#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type_traits/is_lvalue_reference.hpp"
#include "unordered_map/details/type_traits/remove_reference.hpp"

namespace wiz {
    template <typename T>
    WIZ_HIDE_FROM_ABI constexpr T&& forward(remove_reference_t<T>& t) noexcept {
        return static_cast<T&&>(t);
    }

    template <typename T>
    WIZ_HIDE_FROM_ABI constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
        static_assert(!is_lvalue_reference_v<T>, "can not forward an rvalue as an lvalue");
        return static_cast<T&&>(t);
    }
} // namespace wiz
