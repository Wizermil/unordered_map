#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include <cstddef>
#include <vector>
#include <algorithm>

#include "helper/get_random.hpp"

namespace wiz {

    template <typename T>
    void find_test(benchmark::State& state) noexcept {
        std::size_t const min_size = static_cast<std::size_t>(state.range(1));
        std::size_t const num_find = static_cast<std::size_t>(state.range(0));

        T data;
        std::vector<int> rnd;
        rnd.reserve(min_size);
        for (std::size_t i = 0; i < min_size; ++i) {
            int const num = get_random(-2147483646, 2147483647);
            rnd.push_back(num);
            data.insert({num, static_cast<int>(i)});
        }

        for (auto _ : state) {
            for (std::size_t i = 0; i < num_find; ++i) {
                benchmark::DoNotOptimize(data.find(rnd[i]));
            }
        }
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
        state.counters["load_factor"] = static_cast<double>(data.load_factor());
    }

} // namespace wiz
