#pragma once

#include "hash.hpp"
#include <unordered_map/linear_probing.hpp>

namespace wiz::linear_probing::test {

    template <typename K, typename T, typename Hash = wiz::nohash<K>, typename KeyEqual = wiz::equal_to<K>>
    using node_hash_map = raw_flat_hash_map<K, T, Hash, KeyEqual, details::node_policy<K, T, Hash, KeyEqual>>;

    template <typename K, typename T, typename Hash = wiz::nohash<K>, typename KeyEqual = wiz::equal_to<K>>
    using flat_hash_map = raw_flat_hash_map<K, T, Hash, KeyEqual, details::policy<K, T, Hash, KeyEqual>>;

} // namespace wiz::linear_probing::test
