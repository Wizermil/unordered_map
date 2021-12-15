#pragma once

#include "unordered_map/details/utility/forward.hpp"

namespace wiz {
    struct less {
        template <typename T>
        constexpr auto operator()(T&& t, T&& u) const noexcept(noexcept(forward<T>(t) < forward<T>(u))) -> decltype(forward<T>(t) < forward<T>(u)) {
            return forward<T>(t) < forward<T>(u);
        }

        template <typename T>
        constexpr auto operator()(T& t, T& u) const noexcept(noexcept(t < u)) -> decltype(t < u) {
            return t < u;
        }

        template <typename T>
        constexpr auto operator()(const T& t, T& u) const noexcept(noexcept(t < u)) -> decltype(t < u) {
            return t < u;
        }

        template <typename T>
        constexpr auto operator()(T& t, const T& u) const noexcept(noexcept(t < u)) -> decltype(t < u) {
            return t < u;
        }

        template <typename T>
        constexpr auto operator()(const T& t, const T& u) const noexcept(noexcept(t < u)) -> decltype(t < u) {
            return t < u;
        }
    };
}
