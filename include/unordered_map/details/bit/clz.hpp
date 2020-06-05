#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details::bit {

    WIZ_HIDE_FROM_ABI constexpr s32 clz(u32 x) noexcept { return __builtin_clz(x); }

    WIZ_HIDE_FROM_ABI constexpr s32 clz(usize x) noexcept { return __builtin_clzl(x); }

    WIZ_HIDE_FROM_ABI constexpr s32 clz(u64 x) noexcept { return __builtin_clzll(x); }

} // namespace wiz::details::bit
