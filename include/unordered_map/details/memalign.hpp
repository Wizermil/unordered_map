#pragma once

#include <assert.h>
#include <stdlib.h>

#include "unordered_map/details/config.hpp"
#include "unordered_map/details/type.hpp"

namespace wiz::details {

    WIZ_HIDE_FROM_ABI inline void* memalign(usize count, usize align) noexcept {
        void* buf;
        s32 const errc = ::posix_memalign(&buf, align, count);
        assert(errc == 0);
        if (WIZ_UNLIKELY(errc != 0)) {
            return nullptr;
        }
        return buf;
    }

} // namespace wiz::details
