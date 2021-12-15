#pragma once

#include <cstddef>
#include <iostream>
#include <chrono>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <benchmark/benchmark.h>
#pragma clang diagnostic pop

#include "helper/get_random.hpp"

template <typename A, typename B, bool random, std::size_t print_per_iter = 10000ul>
void bench_1m_ins() {
    A a;
    B b;

    std::cout << "index" << '\t';
    std::cout << "a::size" << '\t' << "a::load_factor" << '\t' << "a::bucket_count" << '\t' << "a::insert" << '\t' << "a::erase" << '\t' << "a::find" << '\t';
    std::cout << "b::size" << '\t' << "b::load_factor" << '\t' << "b::bucket_count" << '\t' << "b::insert" << '\t' << "b::erase" << '\t' << "b::find" << '\n';

    std::size_t rnd;
    std::size_t next_print = print_per_iter;
    std::size_t unique = 0;
    std::size_t print = 0;
    std::size_t insert = 0;
    std::size_t a_insert_avg = 0;
    std::size_t b_insert_avg = 0;
    std::size_t erase = 0;
    std::size_t a_erase_avg = 0;
    std::size_t b_erase_avg = 0;
    std::size_t find = 0;
    std::size_t a_find_avg = 0;
    std::size_t b_find_avg = 0;

    for (std::size_t j = 0; print < 100; ++j) {
        if constexpr (random) {
            rnd = wiz::get_random(0ul, ~0ul);
        } else {
            rnd = unique;
        }

        auto start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(b.insert({rnd, unique}));
        b_insert_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(a.insert({rnd, unique}));
        a_insert_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        ++insert;
        ++unique;

        auto ait = a.cbegin();
        for (std::size_t l = 0, index_to_find = wiz::get_random(0ul, a.size() - 1ul); l < index_to_find; ++l) {
            ++ait;
        }
        std::size_t const key_to_find = ait->first;

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(b.find(key_to_find));
        b_find_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(a.find(key_to_find));
        a_find_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        ++find;

        if (--next_print == 0) {
            ++print;

            std::cout << j << '\t';
            std::cout << a.size() << '\t' << a.load_factor() << '\t' << a.bucket_count() << '\t' << (a_insert_avg / insert) << '\t' << (a_erase_avg / erase) << '\t' << (a_find_avg / find) << '\t';
            std::cout << b.size() << '\t' << b.load_factor() << '\t' << b.bucket_count() << '\t' << (b_insert_avg / insert) << '\t' << (b_erase_avg / erase) << '\t' << (b_find_avg / find) << '\n';

            next_print = print_per_iter;//static_cast<std::size_t>(wiz::getUnsignedInt32InRange(100ul, 200ul));

            a_insert_avg = 0ul;
            b_insert_avg = 0ul;
            insert = 0ul;
            a_erase_avg = 0ul;
            b_erase_avg = 0ul;
            erase = 0ul;
            a_find_avg = 0ul;
            b_find_avg = 0ul;
            find = 0ul;
        }
    }

    std::cout << std::endl;
}

template <typename A, typename B, bool random, std::size_t print_per_iter = 10000ul, std::size_t min_size = 1000ul>
void bench_mix_ins_erase(float target_load_factor = 0.75f) {
    A a;
    B b;

    std::cout << "index" << '\t';
    std::cout << "a::size" << '\t' << "a::load_factor" << '\t' << "a::bucket_count" << '\t' << "a::insert" << '\t' << "a::erase" << '\t' << "a::find" << '\t';
    std::cout << "b::size" << '\t' << "b::load_factor" << '\t' << "b::bucket_count" << '\t' << "b::insert" << '\t' << "b::erase" << '\t' << "b::find" << '\n';

    std::size_t rnd;
    std::size_t next_print = print_per_iter;
    std::size_t unique = 0;
    std::size_t print = 0;
    std::size_t insert = 0;
    std::size_t a_insert_avg = 0;
    std::size_t b_insert_avg = 0;
    std::size_t erase = 0;
    std::size_t a_erase_avg = 0;
    std::size_t b_erase_avg = 0;
    std::size_t find = 0;
    std::size_t a_find_avg = 0;
    std::size_t b_find_avg = 0;

    float const error = 0.001f;
    float const low_target_load = target_load_factor*(1.f-error);
    float const high_target_load = target_load_factor*(1.f+error);
    bool presetup = true;
    while (presetup) {
        if constexpr (random) {
            rnd = wiz::get_random(0ul, ~0ul);
        } else {
            rnd = unique;
        }
        float const a_load_factor = a.load_factor();
        bool const a_should_insert = a.size() < min_size || (a_load_factor < low_target_load || a_load_factor > high_target_load);
        if (a_should_insert) {
            a.insert({rnd, unique});
        }
        float const b_load_factor = b.load_factor();
        bool const b_should_insert = b.size() < min_size || (b_load_factor < low_target_load || b_load_factor > high_target_load);
        if (b_should_insert) {
            b.insert({rnd, unique});
        }
        ++unique;
        presetup = a_should_insert || b_should_insert;
    }

    for (std::size_t j = 0; print < 100; ++j) {

        if constexpr (random) {
            rnd = wiz::get_random(0ul, ~0ul);
        } else {
            rnd = unique;
        }

        auto start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(b.insert({rnd, unique}));
        b_insert_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(a.insert({rnd, unique}));
        a_insert_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        ++insert;
        ++unique;

        auto ait = a.cbegin();
        for (std::size_t l = 0, index_to_remove = wiz::get_random(0ul, a.size() - 1ul); l < index_to_remove; ++l) {
            ++ait;
        }
        std::size_t const key_to_remove = ait->first;

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(b.erase(key_to_remove));
        b_erase_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(a.erase(key_to_remove));
        a_erase_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        ++erase;

        ait = a.cbegin();
        for (std::size_t l = 0, index_to_find = wiz::get_random(0ul, a.size() - 1ul); l < index_to_find; ++l) {
            ++ait;
        }
        std::size_t const key_to_find = ait->first;

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(b.find(key_to_find));
        b_find_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        start = std::chrono::high_resolution_clock::now();
        benchmark::DoNotOptimize(a.find(key_to_find));
        a_find_avg += static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count());

        ++find;

        if (--next_print == 0) {
            ++print;

            std::cout << j << '\t';
            std::cout << a.size() << '\t' << a.load_factor() << '\t' << a.bucket_count() << '\t' << (a_insert_avg / insert) << '\t' << (a_erase_avg / erase) << '\t' << (a_find_avg / find) << '\t';
            std::cout << b.size() << '\t' << b.load_factor() << '\t' << b.bucket_count() << '\t' << (b_insert_avg / insert) << '\t' << (b_erase_avg / erase) << '\t' << (b_find_avg / find) << '\n';

            next_print = print_per_iter;//static_cast<std::size_t>(wiz::getUnsignedInt32InRange(100ul, 200ul));

            a_insert_avg = 0ul;
            b_insert_avg = 0ul;
            insert = 0ul;
            a_erase_avg = 0ul;
            b_erase_avg = 0ul;
            erase = 0ul;
            a_find_avg = 0ul;
            b_find_avg = 0ul;
            find = 0ul;
        }
    }

    std::cout << std::endl;
}
