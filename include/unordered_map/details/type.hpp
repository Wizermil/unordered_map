#pragma once

using u8 = unsigned char;
using s8 = signed char;
using u16 = unsigned short;
using s16 = signed short;
using u32 = unsigned int;
using s32 = signed int;
using u64 = unsigned long long;
using u128 = __uint128_t;
using s64 = signed long long;
using usize = unsigned long;
using ssize = signed long;
using uptr = unsigned long;
using sptr = signed long;

static_assert(sizeof(u8) == 1, "u8 must be 8 bits");
static_assert(sizeof(s8) == 1, "s8 must be 8 bits");
static_assert(sizeof(u16) == 2, "u16 must be 16 bits");
static_assert(sizeof(s16) == 2, "s16 must be 16 bits");
static_assert(sizeof(u32) == 4, "u32 must be 32 bits");
static_assert(sizeof(s32) == 4, "s32 must be 32 bits");
static_assert(sizeof(u64) == 8, "u64 must be 64 bits");
static_assert(sizeof(s64) == 8, "s64 must be 64 bits");
static_assert(sizeof(u128) == 16, "u128 must be 128 bits");
#if defined(__LP64__)
static_assert(sizeof(usize) == 8, "usize must be 64 bits");
static_assert(sizeof(ssize) == 8, "ssize must be 64 bits");
static_assert(sizeof(uptr) == 8, "uptr must be 64 bits");
static_assert(sizeof(sptr) == 8, "uptr must be 64 bits");
#else
static_assert(sizeof(usize) == 4, "usize must be 32 bits");
static_assert(sizeof(ssize) == 4, "ssize must be 32 bits");
static_assert(sizeof(ssize) == 4, "ssize must be 32 bits");
static_assert(sizeof(sptr) == 4, "uptr must be 32 bits");
static_assert(sizeof(sptr) == 4, "uptr must be 32 bits");
#endif

#if !defined(NSIMD)
#    if defined(__arm__) || defined(__aarch64__)

#        if defined(__ARM_NEON__)
#            if defined(__aarch64__)
#                define SIMD_USE_ARM64_NEON 1
#            else
#                define SIMD_USE_ARM32_NEON 1
#            endif

#            define SIMD_USE_ARM 1
#        endif

#        if defined(__ARM_FEATURE_FMA__)
#            define SIMD_USE_ARM_FMA10 1
#        endif

#        if !defined(SIMD_USE_ARM)
#            define NSIMD 1
#        endif

#    elif defined(__i386__) || defined(__x86_64__)
#        if defined(__MMX__)
#            define SIMD_USE_X86_MMX10 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__SSE__)
#            define SIMD_USE_X86_SSE10 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__SSE2__)
#            define SIMD_USE_X86_SSE20 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__SSE3__)
#            define SIMD_USE_X86_SSE30 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__SSE4_1__)
#            define SIMD_USE_X86_SSE41 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__SSE4_2__)
#            define SIMD_USE_X86_SSE42 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__AVX__)
#            define SIMD_USE_X86_AVX10 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__AVX2__)
#            define SIMD_USE_X86_AVX20 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__FMA__)
#            define SIMD_USE_X86_FMA10 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__AVX512F__)
#            define SIMD_USE_X86_AVX512F 1
#            define SIMD_USE_X86 1
#        endif
#        if defined(__AVX512VL__)
#            define SIMD_USE_X86_AVX512VL 1
#            define SIMD_USE_X86 1
#        endif

#        if !defined(SIMD_USE_X86)
#            define NSIMD 1
#        endif
#    else
#        error Architecture not supported
#    endif
#endif

#if defined(SIMD_USE_ARM) || defined(SIMD_USE_X86)
using s8x16 = __attribute__((__vector_size__(16 * sizeof(s8)))) s8;
#else
#    error Architecture not supported
#endif
