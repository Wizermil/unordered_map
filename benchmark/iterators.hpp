#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void iterators_forloop_test(benchmark::State& state) noexcept {
        T data;
        for (int i = 0; i < state.range(0); ++i) {
            data.insert({i, i});
        }
        for (auto _ : state) {
            for (auto [key, value] : data) {
                benchmark::DoNotOptimize(key);
                benchmark::DoNotOptimize(value);
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
