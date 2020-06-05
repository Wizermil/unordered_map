#pragma once

namespace wiz {

    template <typename T>
    struct is_lvalue_reference {
        static constexpr bool const value = false;
    };

    template <typename T>
    struct is_lvalue_reference<T&> {
        static constexpr bool const value = true;
    };

    template <typename T>
    static constexpr bool const is_lvalue_reference_v = is_lvalue_reference<T>::value;

} // namespace wiz
