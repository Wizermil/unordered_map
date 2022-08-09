#pragma once

#include "hash.hpp"
#include <unordered_map/robin_hood.hpp>

namespace wiz::robin_hood::test {

    template <typename K, typename T, typename Hash = wiz::nohash<K>, typename KeyEqual = wiz::equal_to<K>>
    using node_hash_map = wiz::robin_hood::raw_flat_hash_map<K, T, Hash, KeyEqual, details::node_policy<K, T, Hash, KeyEqual>>;

    template <typename K, typename T, typename Hash = wiz::nohash<K>, typename KeyEqual = wiz::equal_to<K>>
    using flat_hash_map = wiz::robin_hood::raw_flat_hash_map<K, T, Hash, KeyEqual, details::policy<K, T, Hash, KeyEqual>>;

} // namespace wiz::robin_hood::test
