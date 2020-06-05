#pragma once

#include "unordered_map/details/config.hpp"

namespace wiz {

    template <typename T1, typename T2 = T1>
    struct equal_to {
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T1 const& x, T1 const& y) const { return x == y; }
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T1 const& x, T2 const& y) const { return x == y; }
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T2 const& x, T1 const& y) const { return x == y; }
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T2 const& x, T2 const& y) const { return x == y; }
    };

    template <typename T1>
    struct equal_to<T1, T1> {
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T1 const& x, T1 const& y) const { return x == y; }
    };

    template <typename T1>
    struct equal_to<T1 const, T1> {
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T1 const& x, T1 const& y) const { return x == y; }
    };

    template <typename T1>
    struct equal_to<T1, T1 const> {
        WIZ_HIDE_FROM_ABI constexpr bool operator()(T1 const& x, T1 const& y) const { return x == y; }
    };

} // namespace wiz
