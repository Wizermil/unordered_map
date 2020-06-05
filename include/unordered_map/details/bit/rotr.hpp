#pragma once

#include <limits>

#include "unordered_map/details/bit/bitop_unsigned_integer.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"
#include "unordered_map/details/type_traits/enable_if.hpp"

namespace wiz::details::bit {

    template <typename T, enable_if_t<bitop_unsigned_integer<T>::value>* = nullptr>
    WIZ_HIDE_FROM_ABI constexpr T rotr(T t, u32 cnt) noexcept {
        static_assert(bitop_unsigned_integer<T>::value, "rotr requires unsigned");
        u32 const dig = std::numeric_limits<T>::digits;
        if ((cnt % dig) == 0) {
            return t;
        }
        return (t >> (cnt % dig)) | (t << (dig - (cnt % dig)));
    }

} // namespace wiz::details::bit
