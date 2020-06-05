#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details {

    enum flag : s8 { EMPTY = -128, DELETED = -2, END = -1 };

    template <usize Size, usize Align>
    class WIZ_HIDDEN meta {
    public:
        using value_type = s8;
        using pointer = value_type*;
        using const_pointer = value_type const*;
        using iterator = value_type*;
        using const_iterator = value_type const*;
        using reference = value_type&;
        using const_reference = value_type const&;

    private:
        alignas(Align) s8 _dibs[Size];

    public:
        WIZ_HIDE_FROM_ABI constexpr explicit meta(flag f) noexcept { fill(_dibs, _dibs + Size, f); }
        WIZ_HIDE_FROM_ABI constexpr pointer data() noexcept { return _dibs; }
        WIZ_HIDE_FROM_ABI constexpr const_pointer data() const noexcept { return _dibs; }
    };

    WIZ_HIDE_FROM_ABI constexpr bool is_empty_or_deleted(s8 d) noexcept { return d < END; }
    WIZ_HIDE_FROM_ABI constexpr bool is_empty_or_end(s8 d) noexcept { return d == EMPTY || d == END; }
    WIZ_HIDE_FROM_ABI constexpr bool is_full(s8 d) noexcept { return d >= 0; }
    WIZ_HIDE_FROM_ABI constexpr bool is_end(s8 d) noexcept { return d == END; }
    WIZ_HIDE_FROM_ABI constexpr bool is_at_desired_position(s8 d) noexcept { return d <= 0; }

    template <usize Size, usize Align>
    inline s8* last_meta() noexcept {
        static meta<Size, Align> const last{flag::END};
        return const_cast<s8*>(last.data());
    }

}
