#pragma once

namespace wiz {

    template <bool>
    struct conditional;

    template <>
    struct conditional<true> {
        template <typename X, typename Y>
        using type = X;
    };

    template <>
    struct conditional<false> {
        template <typename X, typename Y>
        using type = Y;
    };

    template <bool B, class X, class Y>
    using conditional_t = typename conditional<B>::template type<X, Y>;

} // namespace wiz
