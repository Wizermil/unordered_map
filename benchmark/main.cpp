#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>

#include <absl/container/flat_hash_map.h>
#include <skarupke/bytell_hash_map.hpp>
#include <skarupke/flat_hash_map.hpp>
#include <crn_core.h>
#include <crn_hash_map.h>
#pragma clang diagnostic pop

#include <elixir/UnorderedMap.hpp>
#include <unordered_map/linear_probing.hpp>
#include <unordered_map/robin_hood.hpp>
#include <unordered_map>

#include "constructor.hpp"
#include "iterators.hpp"
#include "lookup.hpp"
#include "modifiers.hpp"
#include "rehash.hpp"

using wiz_flat_rob = wiz::robin_hood::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using wiz_flat_lin = wiz::linear_probing::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using elixir_map = elixir::UnorderedMap<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using absl_map = absl::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using ska_flat = ska::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using std_map = std::unordered_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using ska_bytell = ska::bytell_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using crnlib_hash_map = crnlib::hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;

namespace wiz {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

    //    BENCHMARK_TEMPLATE(constructor_default_test, wiz::unordered_map<int, int, wiz::hash<int>, wiz::equal_to<int>>);
    //    BENCHMARK_TEMPLATE(constructor_default_test, elixir::UnorderedMap<int, int>);
    //    BENCHMARK_TEMPLATE(constructor_default_test, absl::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>);
    //    BENCHMARK_TEMPLATE(constructor_default_test, ska::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>);
    //    BENCHMARK_TEMPLATE(constructor_default_test, std::unordered_map<int, int, wiz::hash<int>, wiz::equal_to<int>>);
    //    BENCHMARK_TEMPLATE(constructor_default_test, ska::bytell_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>);

//    BENCHMARK_TEMPLATE(find_test, wiz_flat_rob)->Args({31, 2048});
//    BENCHMARK_TEMPLATE(find_test, wiz_flat_lin)->Args({31, 2048});
//    BENCHMARK_TEMPLATE(find_test, elixir_map)->Args({31, 2048});
//    BENCHMARK_TEMPLATE(find_test, crnlib_hash_map)->Args({31, 2048});
    
    //    BENCHMARK_TEMPLATE(find_test, absl_map  )->Args({31, 107});
    //    BENCHMARK_TEMPLATE(find_test, ska_flat  )->Args({31, 107});
    //    BENCHMARK_TEMPLATE(find_test, std_map   )->Args({31, 107});
    //    BENCHMARK_TEMPLATE(find_test, ska_bytell)->Args({31, 107});

    //    BENCHMARK_TEMPLATE(find_test, wiz_flat  )->Args({31, 32768});
    //    BENCHMARK_TEMPLATE(find_test, elixir_map)->Args({31, 32768});
    //    BENCHMARK_TEMPLATE(find_test, absl_map  )->Args({31, 32768});
    //    BENCHMARK_TEMPLATE(find_test, ska_flat  )->Args({31, 32768});
    //    BENCHMARK_TEMPLATE(find_test, std_map   )->Args({31, 32768});
    //    BENCHMARK_TEMPLATE(find_test, ska_bytell)->Args({31, 32768});

    //    BENCHMARK_TEMPLATE(rehash_test, wiz_flat);
    //    BENCHMARK_TEMPLATE(rehash_test, absl_map);
    //    BENCHMARK_TEMPLATE(rehash_test, ska_flat);
    //    BENCHMARK_TEMPLATE(rehash_test, std_map);
    //    BENCHMARK_TEMPLATE(rehash_test, ska_bytell);

    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_rob)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_lin)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, crnlib_hash_map)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, absl_map)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_flat)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, std_map)->Arg(50000);
    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_bytell)->Arg(50000);

//    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_lin)->Arg(107);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, elixir_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_insert_test, absl_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_flat)->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_insert_test, std_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_bytell)->Arg(107);

//    BENCHMARK_TEMPLATE(iterators_forloop_test, wiz_flat_rob)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, wiz_flat_lin)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, elixir_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(iterators_forloop_test, absl_map  )->Arg(107);
    //    BENCHMARK_TEMPLATE(iterators_forloop_test, ska_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(iterators_forloop_test, std_map   )->Arg(107);
    //    BENCHMARK_TEMPLATE(iterators_forloop_test, ska_bytell)->Arg(107);

    //    BENCHMARK_TEMPLATE(modifiers_clear_test, wiz_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_clear_test, elixir_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_clear_test, absl_map  )->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_clear_test, ska_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_clear_test, std_map   )->Arg(107);
    //    BENCHMARK_TEMPLATE(modifiers_clear_test, ska_bytell)->Arg(107);

//    BENCHMARK_TEMPLATE(constructor_cpy_test, wiz_flat_rob)->Arg(107);
//    BENCHMARK_TEMPLATE(constructor_cpy_test, wiz_flat_lin)->Arg(107);
//    BENCHMARK_TEMPLATE(constructor_cpy_test, elixir_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_cpy_test, absl_map  )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_cpy_test, ska_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_cpy_test, std_map   )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_cpy_test, ska_bytell)->Arg(107);

    //    BENCHMARK_TEMPLATE(constructor_move_test, wiz_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_move_test, elixir_map)->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_move_test, absl_map  )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_move_test, ska_flat  )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_move_test, std_map   )->Arg(107);
    //    BENCHMARK_TEMPLATE(constructor_move_test, ska_bytell)->Arg(107);

    //    BENCHMARK(metadata_match_test);

#pragma clang diagnostic pop

} // namespace wiz

BENCHMARK_MAIN();
