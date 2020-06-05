#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details {

    WIZ_HIDE_FROM_ABI constexpr usize next_aligned(usize sz, usize a_minus_one) noexcept { return (sz + a_minus_one) & ~a_minus_one; }

} // namespace wiz::details
