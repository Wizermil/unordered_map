#pragma once

namespace wiz {

    namespace details {

        template <bool>
        struct _enable_if;

        template <>
        struct _enable_if<true> {
            template <typename T = void>
            using type = T;
        };

        template <>
        struct _enable_if<false> {};

    } // namespace details

    template <bool B, typename T = void>
    using enable_if_t = typename details::_enable_if<B>::template type<T>;

} // namespace wiz
