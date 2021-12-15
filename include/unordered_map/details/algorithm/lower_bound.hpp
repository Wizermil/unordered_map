#pragma once

#include "unordered_map/details/type.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/functional/less.hpp"

namespace wiz {
    namespace details {
        template <typename Compare, typename ForwardIterator, typename T>
        WIZ_HIDE_FROM_ABI constexpr ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, T const& value, Compare&& comp) {
            sptr len = last - first;
            while (len != 0) {
                sptr const l2 = len >> 1;
                auto m = first + l2;
                if (comp(*m, value)) {
                    first = ++m;
                    len -= l2 + 1;
                } else {
                    len = l2;
                }
            }
            return first;
        }
    }

    template <typename ForwardIterator, typename T, typename Compare>
    [[nodiscard]] constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, T const& value, Compare&& comp) {
        return details::_lower_bound<Compare>(first, last, value, forward<Compare>(comp));
    }

    template <typename ForwardIterator, typename T>
    [[nodiscard]] constexpr ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, T const& value) {
        return lower_bound(first, last, value, less{});
    }
}
