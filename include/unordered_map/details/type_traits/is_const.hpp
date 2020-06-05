#pragma once

namespace wiz {

    template <typename T>
    struct is_const {
        static constexpr bool const value = false;
    };

    template <typename T>
    struct is_const<T const> {
        static constexpr bool const value = true;
    };

    template <typename T>
    static constexpr bool const is_const_v = is_const<T>::value;

} // namespace wiz
