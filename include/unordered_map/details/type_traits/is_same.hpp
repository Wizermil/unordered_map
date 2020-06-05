#pragma once

namespace wiz {

    template <typename T, typename U>
    struct is_same {
        static constexpr bool const value = false;
    };

    template <typename T>
    struct is_same<T, T> {
        static constexpr bool const value = true;
    };

    template <typename T, typename U>
    static constexpr bool const is_same_v = is_same<T, U>::value;

} // namespace wiz
