#pragma once

#include <assert.h>

#include "unordered_map/details/algorithm/lower_bound.hpp"
#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details {

    WIZ_HIDE_FROM_ABI static constexpr usize const small_primes[] = {
        0,
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
        101,
        103,
        107,
        109,
        113,
        127,
        131,
        137,
        139,
        149,
        151,
        157,
        163,
        167,
        173,
        179,
        181,
        191,
        193,
        197,
        199,
        211
    };

    // potential primes = 210*k + indices[i], k >= 1 these numbers are not divisible by 2, 3, 5 or 7 (or any integer 2 <= j <= 10 for that matter).
    WIZ_HIDE_FROM_ABI static constexpr usize const indices[] = {
        1,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
        101,
        103,
        107,
        109,
        113,
        121,
        127,
        131,
        137,
        139,
        143,
        149,
        151,
        157,
        163,
        167,
        169,
        173,
        179,
        181,
        187,
        191,
        193,
        197,
        199,
        209
    };

    WIZ_HIDE_FROM_ABI static constexpr usize const PRIME_L = 210ul;
    WIZ_HIDE_FROM_ABI static constexpr usize const PRIME_N = sizeof(small_primes) / sizeof(small_primes[0]);
    WIZ_HIDE_FROM_ABI static constexpr usize const PRIME_M = sizeof(indices) / sizeof(indices[0]);

    WIZ_HIDE_FROM_ABI usize next_prime(usize n);
    WIZ_HIDE_FROM_ABI usize next_prime(usize n) {
        // If n is small enough, search in small_primes
        if (n <= small_primes[PRIME_N - 1ul]) {
            return *lower_bound(small_primes, small_primes + PRIME_N, n);
        }
        // Else n > largest small_primes
        // Check for overflow
        if constexpr (sizeof(usize) == 8ul) {
            assert(n <= 0xFFFFFFFFFFFFFFC5ull);
        } else if constexpr (sizeof(usize) == 4ul) {
            assert(n <= 0xFFFFFFFBul);
        }

        // Start searching list of potential primes: L * k0 + indices[in]

        // Select first potential prime >= n
        //   Known a-priori n >= L
        usize k0 = n / PRIME_L;
        usize in = static_cast<usize>(lower_bound(indices, indices + PRIME_M, n - k0 * PRIME_L) - indices);
        n = PRIME_L * k0 + indices[in];
        while (true) {
            // Divide n by all primes or potential primes (i) until:
            //    1.  The division is even, so try next potential prime.
            //    2.  The i > sqrt(n), in which case n is prime.
            // It is known a-priori that n is not divisible by 2, 3, 5 or 7,
            //    so don't test those (j == 5 ->  divide by 11 first).  And the
            //    potential primes start with 211, so don't test against the last
            //    small prime.
            for (usize j = 5ul; j < PRIME_N - 1ul; ++j) {
                usize const p = small_primes[j];
                usize const q = n / p;
                if (q < p) {
                    return n;
                }
                if (n == q * p) {
                    goto next;
                }
            }
            // n wasn't divisible by small primes, try potential primes
            {
                usize i = 211ul;
                while (true) {
                    usize q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 10ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 8ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 8ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 6ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 4ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 2ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    i += 10ul;
                    q = n / i;
                    if (q < i)
                        return n;
                    if (n == q * i)
                        break;

                    // This will loop i to the next "plane" of potential primes
                    i += 2ul;
                }
            }
        next:
            // n is not prime.  Increment n to next potential prime.
            if (++in == PRIME_M) {
                ++k0;
                in = 0ul;
            }
            n = PRIME_L * k0 + indices[in];
        }
    }

}
