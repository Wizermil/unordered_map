#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void constructor_default_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            T const data;
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void constructor_cpy_test(benchmark::State& state) noexcept {
        T c;
        for (int i = 0; i < state.range(0); ++i) {
            c.insert({i, i});
        }
        for (auto _ : state) {
            T const data{c};
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void constructor_move_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            state.PauseTiming();
            T m;
            for (int i = 0; i < state.range(0); ++i) {
                m.insert({i, i});
            }
            state.ResumeTiming();
            T const data{static_cast<T&&>(m)};
            benchmark::DoNotOptimize(data);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
