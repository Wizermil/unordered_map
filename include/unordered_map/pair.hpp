#pragma once

namespace wiz {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
    template <typename T1, typename T2>
    struct pair {
        T1 first;
        T2 second;
    };
#pragma clang diagnostic pop
} // namespace wiz
