#pragma once

#include <limits>

#include "unordered_map/details/bit/bitop_unsigned_integer.hpp"
#include "unordered_map/details/bit/clz.hpp"
#include "unordered_map/details/bit/rotr.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"
#include "unordered_map/details/type_traits/enable_if.hpp"

namespace wiz::details::bit {

    template <typename T, enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    WIZ_HIDE_FROM_ABI constexpr s32 countl_zero(T t) noexcept {
        static_assert(bitop_unsigned_integer<T>::value, "countl_zero requires unsigned");
        if (t == 0) {
            return std::numeric_limits<T>::digits;
        }

        if constexpr (sizeof(T) <= sizeof(u32)) {
            return clz(static_cast<u32>(t)) - (std::numeric_limits<u32>::digits - std::numeric_limits<T>::digits);
        } else if constexpr (sizeof(T) <= sizeof(usize)) {
            return clz(static_cast<usize>(t)) - (std::numeric_limits<usize>::digits - std::numeric_limits<T>::digits);
        } else if constexpr (sizeof(T) <= sizeof(u64)) {
            return clz(static_cast<u64>(t)) - (std::numeric_limits<u64>::digits - std::numeric_limits<T>::digits);
        } else {
            s32 ret = 0;
            s32 iter = 0;
            u32 const ulldigits = std::numeric_limits<u64>::digits;
            while (true) {
                t = rotr(t, ulldigits);
                if ((iter = countl_zero(static_cast<u64>(t))) != ulldigits) {
                    break;
                }
                ret += iter;
            }
            return ret + iter;
        }
    }
} // namespace wiz::details::bit
