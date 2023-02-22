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
    void rehash_test(benchmark::State& state) noexcept {
        std::random_device rd;
        std::mt19937 g(rd());

        std::size_t const sz = 10;
        std::vector<int> rnd;
        rnd.reserve(sz);
        for (std::size_t i = 0; i < sz; ++i) {
            rnd.push_back(get_random(-2147483646, 2147483647));
        }

        T* data = nullptr;
        for (auto _ : state) {
            state.PauseTiming();
            state.PauseTiming();
            if (data != nullptr) {
                delete data;
            }
            data = new T{};
            std::shuffle(rnd.begin(), rnd.end(), g);
            for (std::size_t i = 0; i < sz; ++i) {
                data->insert({rnd[i], static_cast<int>(i)});
            }
            state.ResumeTiming();
            data->rehash(32);
        }
        delete data;
        state.counters["itr"] = benchmark::Counter(state.iterations(), benchmark::Counter::kIsRate);
    }

} // namespace wiz
