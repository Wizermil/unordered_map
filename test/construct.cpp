#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <catch2/catch.hpp>
#pragma clang diagnostic pop

#include <unordered_map/linear_probing.hpp>
#include <unordered_map/robin_hood.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
TEMPLATE_PRODUCT_TEST_CASE("flat_hash_map constructor", "[flat_hash_map]",
                           (wiz::linear_probing::node_hash_map, wiz::linear_probing::flat_hash_map, wiz::robin_hood::node_hash_map, wiz::robin_hood::flat_hash_map),
                           ((int, int))) {
    SECTION("flat_hash_map() noexcept") {
        TestType a;
        REQUIRE(a.size() == 0);
        REQUIRE(a.bucket_count() == 0);
    }

    SECTION("explicit flat_hash_map(size_type bucket_count)") {
        {
            TestType a{1};
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 1);
        }
        {
            TestType a{3};
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 4);
        }
    }

    SECTION("flat_hash_map(InputIt first, InputIt last, size_type bucket_count = 0)") {
        {
            using P = wiz::pair<int, int>;
            P data[]{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}, {10, 10}, {11, 11}, {12, 12}};
            TestType a{data, data + sizeof(data) / sizeof(P)};
            REQUIRE(a.size() == 13);
            REQUIRE(a.bucket_count() == 1);
        }
        {
            using P = wiz::pair<int, int>;
            P data[]{{0, 0}, {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},  {8, 8},
                     {9, 9}, {10, 10}, {11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15}, {16, 16}};
            TestType a{data, data + sizeof(data) / sizeof(P)};
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 2);
        }
        {
            using P = wiz::pair<int, int>;
            P data[]{{0, 0}, {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},  {8, 8},
                     {9, 9}, {10, 10}, {11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15}, {16, 16}};
            TestType a{data, data + sizeof(data) / sizeof(P), 3};
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 4);
        }
    }

    SECTION("flat_hash_map(std::initializer_list<value_type> il, size_type bucket_count = 0)") {
        {
            TestType a{{0, 0}, {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},  {8, 8},
                       {9, 9}, {10, 10}, {11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15}, {16, 16}};
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 2);
        }
        {
            TestType a{
                {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}, {10, 10}, {11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15}, {16, 16}},
                3};
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 4);
        }
    }

    SECTION("flat_hash_map(flat_hash_map const& other)") {
        {
            TestType a{{0, 0}, {1, 1},   {2, 2},   {3, 3},   {4, 4},   {5, 5},   {6, 6},   {7, 7},  {8, 8},
                       {9, 9}, {10, 10}, {11, 11}, {12, 12}, {13, 13}, {14, 14}, {15, 15}, {16, 16}};
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 2);

            TestType b{a};
            REQUIRE(b.size() == 17);
            REQUIRE(b.bucket_count() == 2);

            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 2);

            for (auto& [k, v] : b) {
                REQUIRE(v == a[k]);
            }
        }
        {
            TestType a;
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);

            TestType b{a};
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 0);

            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
        }
    }

    SECTION("flat_hash_map(flat_hash_map&& other) noexcept") {
        {
            TestType a{{0, 0}, {2, 2}, {4, 4}, {6, 6}, {8, 8}};
            REQUIRE(a.size() == 5);
            REQUIRE(a.bucket_count() == 1);

            TestType b{wiz::move(a)};
            REQUIRE(b.size() == 5);
            REQUIRE(b.bucket_count() == 1);

            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
        }
        {
            TestType a;
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);

            TestType b{wiz::move(a)};
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 0);

            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
        }
    }

    SECTION("flat_hash_map& operator=(flat_hash_map const& other)") {
        {
            TestType a;
            TestType b;
            b = a;
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 0);
            REQUIRE(a == b);
        }
        {
            TestType a;
            TestType b{{1, 1}};
            b = a;
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 1);
            REQUIRE(a == b);
        }
        {
            TestType a{{0, 0}, {2, 2}, {4, 4}, {6, 6}, {8, 8}};
            TestType b{{1, 1}};
            b = a;
            REQUIRE(a.size() == 5);
            REQUIRE(a.bucket_count() == 1);
            REQUIRE(b.size() == 5);
            REQUIRE(b.bucket_count() == 1);
            REQUIRE(a == b);
        }
        {
            TestType a{{0, 0},   {2, 2},   {4, 4},   {6, 6},   {8, 8},   {10, 10}, {12, 12}, {14, 14}, {16, 16},
                       {18, 18}, {20, 20}, {22, 22}, {24, 24}, {26, 26}, {28, 28}, {30, 30}, {32, 32}};
            TestType b{{1, 1},   {3, 3},   {5, 5},   {7, 7},   {9, 9},   {11, 11}, {13, 13}, {15, 15}, {17, 17}, {19, 19}, {21, 21},
                       {23, 23}, {25, 25}, {27, 27}, {29, 29}, {31, 31}, {33, 33}, {35, 35}, {37, 37}, {39, 39}, {41, 41}, {43, 43},
                       {45, 45}, {47, 47}, {49, 49}, {51, 51}, {53, 53}, {57, 57}, {59, 59}, {61, 61}, {63, 63}, {65, 65}, {67, 67}};
            b = a;
            REQUIRE(a.size() == 17);
            REQUIRE(a.bucket_count() == 2);
            REQUIRE(b.size() == 17);
            REQUIRE(b.bucket_count() == 4);
            REQUIRE(a == b);
        }
    }

    SECTION("flat_hash_map& operator=(flat_hash_map&& other) noexcept") {
        {
            TestType a;
            TestType b;
            b = wiz::move(a);
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 0);
        }
        {
            TestType a;
            TestType b{{1, 1}};
            b = wiz::move(a);
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 0);
            REQUIRE(b.bucket_count() == 0);
        }
        {
            TestType a{{0, 0}, {2, 2}, {4, 4}, {6, 6}, {8, 8}};
            TestType b{{1, 1}};
            b = wiz::move(a);
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 5);
            REQUIRE(b.bucket_count() == 1);
        }
        {
            TestType a{{0, 0},   {2, 2},   {4, 4},   {6, 6},   {8, 8},   {10, 10}, {12, 12}, {14, 14}, {16, 16},
                       {18, 18}, {20, 20}, {22, 22}, {24, 24}, {26, 26}, {28, 28}, {30, 30}, {32, 32}};
            TestType b{{1, 1},   {3, 3},   {5, 5},   {7, 7},   {9, 9},   {11, 11}, {13, 13}, {15, 15}, {17, 17}, {19, 19}, {21, 21},
                       {23, 23}, {25, 25}, {27, 27}, {29, 29}, {31, 31}, {33, 33}, {35, 35}, {37, 37}, {39, 39}, {41, 41}, {43, 43},
                       {45, 45}, {47, 47}, {49, 49}, {51, 51}, {53, 53}, {57, 57}, {59, 59}, {61, 61}, {63, 63}, {65, 65}, {67, 67}};
            b = wiz::move(a);
            REQUIRE(a.size() == 0);
            REQUIRE(a.bucket_count() == 0);
            REQUIRE(b.size() == 17);
            REQUIRE(b.bucket_count() == 2);
        }
    }
}
#pragma clang diagnostic pop
