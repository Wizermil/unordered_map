#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void rehash_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            state.PauseTiming();
            T data;
            state.ResumeTiming();
            data.rehash(3);
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
