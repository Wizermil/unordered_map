#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include "helper/get_random.hpp"

namespace wiz {

    template <typename T>
    void modifiers_insert_test(benchmark::State& state) noexcept {
        constexpr bool const has_load_factor = requires(T const& t) {
            t.load_factor();
        };
        double load_factor = 0.0;

        std::size_t const sz{static_cast<std::size_t>(state.range(0))};
        std::vector<int> rnd;
        rnd.reserve(sz);
        for (std::size_t i = 0ul; i < sz; ++i) {
            rnd.push_back(get_random(-2147483646, 2147483647));
        }

        T* data = nullptr;
        for (auto _ : state) {
            state.PauseTiming();
            if (data != nullptr) {
                if constexpr (has_load_factor) {
                    load_factor += static_cast<double>(data->load_factor());
                }
                delete data;
            }
            data = new T{};
            state.ResumeTiming();

            for (std::size_t i = 0; i < sz; ++i) {
                benchmark::DoNotOptimize(data->insert({rnd[i], static_cast<int>(i)}));
            }
        }

        if constexpr (has_load_factor) {
            load_factor += static_cast<double>(data->load_factor());
        }
        delete data;
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
        state.counters["load_factor"] = load_factor / state.iterations();
    }

    template <typename T>
    void modifiers_erase_test(benchmark::State& state) noexcept {
        constexpr bool const has_load_factor = requires(T const& t) {
            t.load_factor();
        };
        double load_factor = 0.0;

        std::size_t const num_to_erase{static_cast<std::size_t>(state.range(0))};
        std::size_t const sz{static_cast<std::size_t>(state.range(1))};
        T defaultdata;
        std::vector<int> rnd;
        rnd.reserve(sz);
        for (std::size_t i = 0ul; i < sz; ++i) {
            rnd.push_back(get_random(-2147483646, 2147483647));
            defaultdata.insert({rnd[i], static_cast<int>(i)});
        }

        T* data = nullptr;
        for (auto _ : state) {
            state.PauseTiming();
            if (data != nullptr) {
                if constexpr (has_load_factor) {
                    load_factor += static_cast<double>(data->load_factor());
                }
                delete data;
            }
            data = new T{defaultdata};
            state.ResumeTiming();

            for (std::size_t i = 0; i < num_to_erase; ++i) {
                benchmark::DoNotOptimize(data->erase(rnd[i]));
            }
        }

        if constexpr (has_load_factor) {
            load_factor += static_cast<double>(data->load_factor());
        }
        delete data;
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
        state.counters["load_factor"] = load_factor / state.iterations();
    }

    template <typename T>
    void modifiers_clear_test(benchmark::State& state) noexcept {
        constexpr bool const has_load_factor = requires(T const& t) {
            t.load_factor();
        };
        double load_factor = 0.0;

        std::size_t const sz{static_cast<std::size_t>(state.range(0))};
        T defaultdata;
        std::vector<int> rnd;
        rnd.reserve(sz);
        for (std::size_t i = 0ul; i < sz; ++i) {
            rnd.push_back(get_random(-2147483646, 2147483647));
            defaultdata.insert({rnd[i], static_cast<int>(i)});
        }

        T* data = nullptr;
        for (auto _ : state) {
            state.PauseTiming();
            if (data != nullptr) {
                if constexpr (has_load_factor) {
                    load_factor += static_cast<double>(data->load_factor());
                }
                delete data;
            }
            data = new T{defaultdata};
            state.ResumeTiming();

            data->clear();
        }

        if constexpr (has_load_factor) {
            load_factor += static_cast<double>(data->load_factor());
        }
        delete data;
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
        state.counters["load_factor"] = load_factor / state.iterations();
    }

} // namespace wiz
