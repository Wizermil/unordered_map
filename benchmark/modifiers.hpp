#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

namespace wiz {

    template <typename T>
    void modifiers_insert_test(benchmark::State& state) noexcept {
        T* data = nullptr;
        for (auto _ : state) {
            state.PauseTiming();
            if (data != nullptr) {
                delete data;
            }
            int const sz{static_cast<int>(state.range(0))};
            data = new T{};
            data->reserve(static_cast<size_t>(sz));
            state.ResumeTiming();
            for (int i = 0; i < sz; ++i) {
                auto it = data->insert({i, i});
                benchmark::DoNotOptimize(it);
                benchmark::ClobberMemory();
            }
        }
        if (data != nullptr) {
            delete data;
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void modifiers_erase_test(benchmark::State& state) noexcept {
        for (auto _ : state) {
            state.PauseTiming();
            T data;
            for (int i = 0; i < state.range(0); ++i) {
                data.insert({i, i});
            }
            auto const erase{data.find(static_cast<int>(state.range(1)))};
            state.ResumeTiming();
            data.erase(erase);
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

    template <typename T>
    void modifiers_clear_test(benchmark::State& state) noexcept {
        T data;
        for (auto _ : state) {
            state.PauseTiming();
            for (int i = 0; i < state.range(0); ++i) {
                data.insert({i, i});
            }
            state.ResumeTiming();
            data.clear();
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
