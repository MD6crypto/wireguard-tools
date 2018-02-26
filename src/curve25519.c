/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2018 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 */

#include "curve25519.h"

#include <stdint.h>
#include <string.h>

#ifdef __linux__
#include <linux/types.h>
typedef __u64 u64;
typedef __u32 u32;
typedef __u8 u8;
typedef __s64 s64;
#else
typedef uint64_t u64, __le64;
typedef uint32_t u32, __le32;
typedef uint8_t u8;
typedef int64_t s64;
#endif
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define le64_to_cpup(a) __builtin_bswap64(*(a))
#define le32_to_cpup(a) __builtin_bswap32(*(a))
#define cpu_to_le64(a) __builtin_bswap64(a)
#else
#define le64_to_cpup(a) (*(a))
#define le32_to_cpup(a) (*(a))
#define cpu_to_le64(a) (a)
#endif
#ifndef __always_inline
#define __always_inline __inline __attribute__((__always_inline__))
#endif
#ifndef noinline
#define noinline __attribute__((noinline))
#endif
#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif
#ifndef __force
#define __force
#endif
#define normalize_secret(a) curve25519_normalize_secret(a)

static noinline void memzero_explicit(void *s, size_t count)
{
	memset(s, 0, count);
	asm volatile("": :"r"(s) :"memory");
}

#ifdef __SIZEOF_INT128__
#include "../crypto/curve25519-hacl64.h"
#else
#include "../crypto/curve25519-fiat32.h"
#endif

void curve25519_generate_public(uint8_t pub[static CURVE25519_POINT_SIZE], const uint8_t secret[static CURVE25519_POINT_SIZE])
{
	static const uint8_t basepoint[CURVE25519_POINT_SIZE] = { 9 };

	curve25519(pub, secret, basepoint);
}

void curve25519(uint8_t mypublic[static CURVE25519_POINT_SIZE], const uint8_t secret[static CURVE25519_POINT_SIZE], const uint8_t basepoint[static CURVE25519_POINT_SIZE])
{
	curve25519_generic(mypublic, secret, basepoint);
}
