#pragma once

#include <stddef.h>

#include <unordered_map/details/config.hpp>
#include <unordered_map/details/type.hpp>

namespace wiz {

    namespace details {
        WIZ_HIDE_FROM_ABI constexpr inline u64 _nohash(u64 x) noexcept WIZ_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK {
            return x;
        }
    }

    template <typename T>
    struct nohash;

#define WIZ_HASH_SPECIALIZE_BY_VALUE(type)                                                            \
    template <>                                                                                       \
    struct nohash<type> {                                                                             \
        constexpr size_t operator()(type value) const noexcept { return static_cast<size_t>(details::_nohash(static_cast<u64>(value))); } \
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
    struct nohash<T*> {
        WIZ_HIDE_FROM_ABI inline size_t operator()(T* value) const noexcept {
            return static_cast<size_t>(details::_nohash(reinterpret_cast<u64>(value)));
        }
    };
} // namespace wiz

