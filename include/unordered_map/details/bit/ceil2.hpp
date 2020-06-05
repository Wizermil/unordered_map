#pragma once

#include <assert.h>
#include <limits>

#include "unordered_map/details/bit/bitop_unsigned_integer.hpp"
#include "unordered_map/details/bit/countl_zero.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"
#include "unordered_map/details/type_traits/enable_if.hpp"

namespace wiz::details::bit {

    template <typename T, enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    WIZ_HIDE_FROM_ABI constexpr T ceil2(T t) noexcept {
        if (t < 2) {
            return 1;
        }
        u32 const n = static_cast<u32>(std::numeric_limits<T>::digits - countl_zero(static_cast<T>(t - 1u)));
        assert(n != std::numeric_limits<T>::digits); // "Bad input to ceil2"

        if constexpr (sizeof(T) >= sizeof(u32)) {
            return T{1} << n;
        } else {
            u32 const extra = static_cast<u32>(std::numeric_limits<u32>::digits - std::numeric_limits<T>::digits);
            u32 const retVal = 1u << (n + extra);
            return static_cast<T>(retVal >> extra);
        }
    }

} // namespace wiz::details::bit
