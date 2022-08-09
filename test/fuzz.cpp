#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include "nat.hpp"
#include "helper/linear_probing.hpp"
#include "helper/robin_hood.hpp"

#include <cstddef>
#include <random>
#include <type_traits>
#include <unordered_map>

template <typename T, std::enable_if_t<std::is_arithmetic_v<T> && std::is_integral_v<T>>* = nullptr>
static inline T get_random(T a, T b) noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<T> dis(a, b);
    return dis(gen);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
TEMPLATE_PRODUCT_TEST_CASE("flat_hash_map modifiers", "[flat_hash_map]",
                           (wiz::linear_probing::test::node_hash_map, wiz::linear_probing::test::flat_hash_map, wiz::robin_hood::test::node_hash_map, wiz::robin_hood::test::flat_hash_map),
                           ((signed long long, nat))) {

    // test mix
    reset_static_nat_counter();
    for (std::size_t i = 0ul; i < 1000ul; ++i) {
        TestType a;
        std::unordered_map<signed long long, nat, wiz::hash<signed long long>, wiz::equal_to<signed long long>> b;

        signed long long unique = 0;

        for (std::size_t j = 0, jmax = get_random(1ul, 100ul); j < jmax; ++j) {
            if (a.empty() || get_random(0ul, 100ul) > 50ul) { // add
                for (std::size_t k = 0ul, add_max = get_random(1ul, 10ul); k < add_max; ++k) {
                    a.insert({unique, nat{unique}});
                    b.insert({unique, nat{unique}});

                    ++unique;
                }
            } else { // remove
                std::size_t const rnd = get_random(0ul, 100ul);

                if (rnd > 66ul) { // erase front
                    for (std::size_t k = 0ul, remove_max = get_random(1ul, std::min(5ul, a.size())); k < remove_max; ++k) {
                        auto ait = a.cbegin();
                        signed long long const key_to_remove = ait->first;
                        a.erase(ait);
                        b.erase(key_to_remove);
                    }
                } else { // erase(const_iterator)
                    for (std::size_t k = 0ul, remove_max = get_random(1ul, std::min(5ul, a.size())); k < remove_max; ++k) {
                        auto ait = a.cbegin();
                        for (std::size_t l = 0, index_to_remove = get_random(0ul, a.size() - 1ul); l < index_to_remove; ++l) {
                            ++ait;
                        }
                        signed long long const key_to_remove = ait->first;
                        a.erase(ait);
                        b.erase(key_to_remove);
                    }
                }

                REQUIRE((a.size() == b.size()));

                bool equal = true;
                for (auto const& [ind, data] : a) {
                    auto bit = b.find(ind);
                    if (bit == b.end()) {
                        equal = false;
                        break;
                    } else if (bit->second.cnt != data.cnt) {
                        equal = false;
                        break;
                    }
                }
                REQUIRE(equal);
            }
        }
    }
    REQUIRE(ctr + cpy + mv == dtr);

}

TEMPLATE_PRODUCT_TEST_CASE("flat_hash_map distrib", "[flat_hash_map]",
                           (wiz::robin_hood::test::flat_hash_map),
                           ((signed long long, nat))) {

    

}
