#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include "helper/linear_probing.hpp"
#include "helper/robin_hood.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
TEMPLATE_PRODUCT_TEST_CASE("flat_hash_map iterators", "[flat_hash_map]",
                           (wiz::linear_probing::test::node_hash_map, wiz::linear_probing::test::flat_hash_map, wiz::robin_hood::test::node_hash_map, wiz::robin_hood::test::flat_hash_map),
                           ((int, int))) {
    SECTION("iterator begin()/end() noexcept") {
        {
            TestType a;
            REQUIRE(a.begin() == a.end());
            REQUIRE_FALSE(a.begin() != a.end());
        }
        {
            TestType a;
            for (int i = 0; i < 31; ++i) {
                a.insert({i, i});
            }
            REQUIRE(a.begin() != a.end());
            REQUIRE_FALSE(a.begin() == a.end());
        }
    }

    SECTION("const_iterator begin()/end() const noexcept") {
        TestType const a;
        REQUIRE(a.begin() == a.end());
        REQUIRE_FALSE(a.begin() != a.end());
    }
}
#pragma clang diagnostic pop
