#pragma once

#define WIZ_HIDDEN [[gnu::visibility("hidden")]]
#define WIZ_INTERNAL_LINKAGE [[clang::internal_linkage]]

#define WIZ_HIDE_FROM_ABI WIZ_HIDDEN WIZ_INTERNAL_LINKAGE

#define WIZ_NOINLINE __attribute__((noinline))

#define WIZ_LIKELY(exp) (__builtin_expect(false || (exp), true))
#define WIZ_UNLIKELY(exp) (__builtin_expect(exp, false))
