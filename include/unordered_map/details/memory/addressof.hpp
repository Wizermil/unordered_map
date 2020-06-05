#pragma once

#if !__has_builtin(__builtin_addressof)
#    error compiler not supported
#endif

namespace wiz {

    template <typename T>
    constexpr T* addressof(T& arg) noexcept {
        return __builtin_addressof(arg);
    }

    template <typename T>
    T const* addressof(T const&&) = delete;

} // namespace wiz
