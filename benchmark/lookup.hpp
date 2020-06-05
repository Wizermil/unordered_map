#pragma once

#include <stdint.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void find_test(benchmark::State& state) noexcept {
        T data;
        for (int i = 0; i < state.range(1); ++i) {
            data.insert({i, i});
        }
        int8_t const match{static_cast<int8_t>(state.range(0))};
        for (auto _ : state) {
            auto it = data.find(match);
            benchmark::DoNotOptimize(it);
            benchmark::ClobberMemory();
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
