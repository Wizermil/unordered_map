#pragma once

#include "unordered_map/details/config.hpp"

namespace wiz::details {

    template <typename ForwardIt, typename T>
    WIZ_HIDE_FROM_ABI constexpr void fill(ForwardIt first, ForwardIt last, T const& value) noexcept {
        for (; first != last; ++first) {
            *first = value;
        }
    }

} // namespace wiz::details
