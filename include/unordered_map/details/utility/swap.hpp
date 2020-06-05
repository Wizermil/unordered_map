#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/utility/move.hpp"

namespace wiz {

    template <typename T>
    WIZ_HIDE_FROM_ABI inline void swap(T& x, T& y) {
        T t(wiz::move(x));
        x = wiz::move(y);
        y = wiz::move(t);
    }

} // namespace wiz
