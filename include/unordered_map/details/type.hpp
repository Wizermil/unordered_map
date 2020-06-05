#pragma once

using u8 = unsigned char;
using s8 = signed char;
using u16 = unsigned short;
using s16 = signed short;
using u32 = unsigned int;
using s32 = signed int;
using u64 = unsigned long long;
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
