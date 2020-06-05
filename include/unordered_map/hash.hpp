#pragma once

#include <stddef.h>

#include "unordered_map/details/config.hpp"

namespace wiz {
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
