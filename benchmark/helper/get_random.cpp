#include "get_random.hpp"

static constexpr std::uint64_t const DEFAULT_STATE_64 = 0x853c49e7248fea9b;
static constexpr std::uint64_t const DEFAULT_INC_64 = 0xda3e32cb94b95bdc;

namespace wiz {

    static RNG global_rng = { DEFAULT_STATE_64, DEFAULT_INC_64 };

    std::uint32_t getUnsignedInt32() {
        // Based on https://www.pcg-random.org/download.html
        std::uint64_t oldstate = global_rng.state;
        global_rng.state = oldstate * 6364136223846793005ull + (global_rng.inc|1);
        std::uint32_t xorshifted = static_cast<std::uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        std::uint32_t rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    std::uint32_t getBoundedUnsignedInt32(std::uint32_t bound) {
        // Based on https://www.pcg-random.org/posts/bounded-rands.html
        ++bound;
        std::uint64_t random32bit = getUnsignedInt32();
        std::uint64_t multiresult = random32bit * static_cast<std::uint64_t>(bound);
        std::uint32_t leftover = static_cast<std::uint32_t>(multiresult);
        if (leftover < bound) {
            std::uint32_t threshold = -bound;
            if (threshold >= bound) {
                threshold -= bound;
                if (threshold >= bound) {
                    threshold %= bound;
                }
            }
            while (leftover < threshold) {
                random32bit = getUnsignedInt32();
                multiresult = random32bit * static_cast<std::uint64_t>(bound);
                leftover = static_cast<std::uint32_t>(multiresult);
            }
        }
        return multiresult >> 32;
    }

    std::uint32_t getUnsignedInt32InRange(std::uint32_t min, std::uint32_t max) {
        assert(min <= max); // max should be greater than min
        return min + getBoundedUnsignedInt32(max - min);
    }

    bool getBool() {
        return getBoundedUnsignedInt32(1) == 1 ? true : false;
    }

}
