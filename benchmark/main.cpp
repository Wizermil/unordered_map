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
#include <unordered_map/robin_hood_dense.hpp>
#include <unordered_map>

#include "constructor.hpp"
#include "iterators.hpp"
#include "lookup.hpp"
#include "modifiers.hpp"
#include "rehash.hpp"
#include "mix.hpp"

using wiz_flat_rob = wiz::robin_hood::flat_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
using wiz_flat_dense_rob = wiz::robin_hood::flat_dense_hash_map<int, int, wiz::hash<int>, wiz::equal_to<int>>;
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

//    BENCHMARK_TEMPLATE(constructor_default_test, wiz_flat_rob);
//    BENCHMARK_TEMPLATE(constructor_default_test, absl_map);
//    BENCHMARK_TEMPLATE(constructor_default_test, std_map);
//    BENCHMARK_TEMPLATE(constructor_default_test, elixir_map);
//    BENCHMARK_TEMPLATE(constructor_default_test, crnlib_hash_map);
//    BENCHMARK_TEMPLATE(constructor_default_test, ska_flat);
//    BENCHMARK_TEMPLATE(constructor_default_test, ska_bytell);
//    BENCHMARK_TEMPLATE(constructor_default_test, wiz_flat_lin);
//    BENCHMARK_TEMPLATE(constructor_default_test, wiz_flat_dense_rob);

//    BENCHMARK_TEMPLATE(find_test, wiz_flat_rob)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, absl_map)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, std_map)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, elixir_map)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, crnlib_hash_map)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, ska_flat)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, ska_bytell)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, wiz_flat_lin)->Args({2048, 32768});
//    BENCHMARK_TEMPLATE(find_test, wiz_flat_dense_rob)->Args({2048, 32768});

//    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_rob)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, absl_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, std_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, elixir_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, crnlib_hash_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_flat)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, ska_bytell)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_lin)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_insert_test, wiz_flat_dense_rob)->Arg(50000);

    BENCHMARK_TEMPLATE(modifiers_erase_test, wiz_flat_rob)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, absl_map)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, std_map)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, elixir_map)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, crnlib_hash_map)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, ska_flat)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, ska_bytell)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, wiz_flat_lin)->Args({2048, 32768});
    BENCHMARK_TEMPLATE(modifiers_erase_test, wiz_flat_dense_rob)->Args({2048, 32768});

//    BENCHMARK_TEMPLATE(modifiers_clear_test, wiz_flat_rob)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, absl_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, std_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, elixir_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, crnlib_hash_map)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, ska_flat)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, ska_bytell)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, wiz_flat_lin)->Arg(50000);
//    BENCHMARK_TEMPLATE(modifiers_clear_test, wiz_flat_dense_rob)->Arg(50000);

//    BENCHMARK_TEMPLATE(rehash_test, wiz_flat_rob);
//    BENCHMARK_TEMPLATE(rehash_test, absl_map);
//    BENCHMARK_TEMPLATE(rehash_test, std_map);
//    // no rehash for elixir_map
//    // no rehash for crnlib_hash_map
//    BENCHMARK_TEMPLATE(rehash_test, ska_flat);
//    BENCHMARK_TEMPLATE(rehash_test, ska_bytell);
//    BENCHMARK_TEMPLATE(rehash_test, wiz_flat_lin);
//    BENCHMARK_TEMPLATE(rehash_test, wiz_flat_dense_rob);

//    BENCHMARK_TEMPLATE(iterators_forloop_test, wiz_flat_rob)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, absl_map)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, std_map)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, elixir_map)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, crnlib_hash_map)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, ska_flat)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, ska_bytell)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, wiz_flat_lin)->Arg(107);
//    BENCHMARK_TEMPLATE(iterators_forloop_test, wiz_flat_dense_rob)->Arg(107);

#pragma clang diagnostic pop

} // namespace wiz


int main(int argc, char** argv) {
    using A = wiz::robin_hood::flat_hash_map<std::size_t, std::size_t, wiz::hash<std::size_t>, wiz::equal_to<std::size_t>>;
    using B = absl::flat_hash_map<std::size_t, std::size_t, wiz::hash<std::size_t>, wiz::equal_to<std::size_t>>;

//    bench_1m_ins<A, B, true>();
//    bench_1m_ins<A, B, false>();
//
//    bench_mix_ins_erase<A, B, true, 6000000>();
//    bench_mix_ins_erase<A, B, false, 6000000>();

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
