#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type_traits/remove_reference.hpp"

namespace wiz {
    template <typename T>
    WIZ_HIDE_FROM_ABI constexpr remove_reference_t<T>&& move(T&& t) noexcept {
        using U = remove_reference_t<T>;
        return static_cast<U&&>(t);
    }
} // namespace wiz
