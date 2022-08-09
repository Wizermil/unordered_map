#pragma once

#include <cstddef>

#include "helper/hash.hpp"
#include <unordered_map/hash.hpp>

extern std::size_t ctr;
extern std::size_t cpy;
extern std::size_t cpy_assign;
extern std::size_t mv;
extern std::size_t mv_assign;
extern std::size_t dtr;

struct nat {
    signed long long cnt;
    nat();
    explicit nat(signed long long c0);
    explicit nat(signed long long c0, signed long long c1);
    nat(nat const& other) noexcept;
    nat& operator=(nat const& other) noexcept;
    nat(nat&& other) noexcept;
    nat& operator=(nat&& other) noexcept;
    ~nat();
};

namespace wiz {
    template <>
    struct hash<nat> {
        constexpr std::size_t operator()(nat const& value) const noexcept { return static_cast<std::size_t>(wiz::hash<signed long long>{}(value.cnt)); }
    };

    template <>
    struct nohash<nat> {
        constexpr std::size_t operator()(nat const& value) const noexcept { return static_cast<std::size_t>(value.cnt); }
    };
}


constexpr bool operator==(nat const& lhs, nat const& rhs) noexcept {
    return lhs.cnt == rhs.cnt;
}

struct nat_no_move {
    std::size_t cnt;
    nat_no_move();
    nat_no_move(nat_no_move const& other) noexcept;
    nat_no_move& operator=(nat_no_move const& other) noexcept;
    ~nat_no_move();
};

struct nat_no_default_ctr {
    std::size_t cnt;
    nat_no_default_ctr() = delete;
    nat_no_default_ctr(nat_no_default_ctr const& other) noexcept;
    nat_no_default_ctr& operator=(nat_no_default_ctr const& other) noexcept;
    ~nat_no_default_ctr();
};

void reset_static_nat_counter() noexcept;
