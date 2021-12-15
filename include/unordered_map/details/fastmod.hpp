#pragma once

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details {

    WIZ_HIDE_FROM_ABI constexpr u128 computeM(u64 d) {
        // what follows is just ((__uint128_t)0 - 1) / d) + 1 spelled out
        u128 M = 0xFFFFFFFFFFFFFFFFull;
        M <<= 64;
        M |= 0xFFFFFFFFFFFFFFFFull;
        M /= d;
        M += 1;
        return M;
    }

    WIZ_HIDE_FROM_ABI constexpr u64 mul128_u64(u128 lowbits, u64 d) {
        u128 bottom_half = (lowbits & 0xFFFFFFFFFFFFFFFFull) * d; // Won't overflow
        bottom_half >>= 64;  // Only need the top 64 bits, as we'll shift the lower half away;
        u128 top_half = (lowbits >> 64) * d;
        u128 both_halves = bottom_half + top_half; // Both halves are already shifted down by 64
        both_halves >>= 64; // Get top half of both_halves
        return static_cast<u64>(both_halves);
    }

    WIZ_HIDE_FROM_ABI constexpr u64 fastmod_u64(u64 a, u128 M, u64 d) {
        u128 lowbits = M * a;
        return mul128_u64(lowbits, d);
    }

}
