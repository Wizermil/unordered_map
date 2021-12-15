#pragma once

#include "unordered_map/details/config.hpp"

namespace wiz {

    template <typename T>
    WIZ_HIDE_FROM_ABI constexpr const T& max(T const& l, T const& r) noexcept(noexcept(l > r)) {
        return l > r ? l : r;
    }

} // namespace wiz
