#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <type_traits>

namespace wiz {

    struct RNG {
        std::uint64_t state;
        std::uint64_t inc;
    };

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T> && std::is_integral_v<T>>* = nullptr>
    static inline T get_random(T a, T b) noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dis(a, b);
        return dis(gen);
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T> && std::is_floating_point_v<T>>* = nullptr>
    static inline T get_random(T a, T b) noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dis(a, b);
        return dis(gen);
    }

    std::uint32_t getUnsignedInt32();
    std::uint32_t getUnsignedInt32InRange(std::uint32_t min, std::uint32_t max);
    std::uint32_t getBoundedUnsignedInt32(std::uint32_t bound);
    bool getBool();

} // namespace wiz
