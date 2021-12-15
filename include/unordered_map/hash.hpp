#pragma once

#include <stddef.h>

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz {

    namespace details {
        WIZ_HIDE_FROM_ABI constexpr inline u64 _moremur(u64 x) noexcept WIZ_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK {
            x ^= x >> 27u;
            x *= 0x3C79AC492BA7B653ull;
            x ^= x >> 33;
            x *= 0x1C69B3F74AC4AE35ull;
            x ^= x >> 27;
            return x;
        }
    }

    template <typename T>
    struct hash;

#define WIZ_HASH_SPECIALIZE_BY_VALUE(type)                                                            \
    template <>                                                                                       \
    struct hash<type> {                                                                               \
        constexpr size_t operator()(type value) const noexcept { return static_cast<size_t>(value); } \
    };

    WIZ_HASH_SPECIALIZE_BY_VALUE(bool)
    WIZ_HASH_SPECIALIZE_BY_VALUE(char)
    WIZ_HASH_SPECIALIZE_BY_VALUE(signed char)
    WIZ_HASH_SPECIALIZE_BY_VALUE(signed short)
    WIZ_HASH_SPECIALIZE_BY_VALUE(signed int)
    WIZ_HASH_SPECIALIZE_BY_VALUE(signed long)
    WIZ_HASH_SPECIALIZE_BY_VALUE(signed long long)
    WIZ_HASH_SPECIALIZE_BY_VALUE(unsigned char)
    WIZ_HASH_SPECIALIZE_BY_VALUE(unsigned short)
    WIZ_HASH_SPECIALIZE_BY_VALUE(unsigned int)
    WIZ_HASH_SPECIALIZE_BY_VALUE(unsigned long)
    WIZ_HASH_SPECIALIZE_BY_VALUE(unsigned long long)

#undef WIZ_HASH_SPECIALIZE_BY_VALUE

    template <typename T>
    struct hash<T*> {
        WIZ_HIDE_FROM_ABI size_t operator()(T* value) const noexcept {
            size_t const x = reinterpret_cast<size_t>(value);
            return x + (x >> 3);
        }
    };
} // namespace wiz
