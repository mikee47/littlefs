/*
 * lfs utility functions
 *
 * Copyright (c) 2022, The littlefs authors.
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef LFS_UTIL_H
#define LFS_UTIL_H

#include <debug_progmem.h>
#include <string.h>
#include <inttypes.h>

#ifdef SMING_RELEASE
#define LFS_NO_ASSERT
#else
#include <assert.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __INT16
#define __INT16
#endif

#ifndef __INT32
#define __INT32
#endif

// Macros, may be replaced by system specific wrappers. Arguments to these
// macros must not have side-effects as the macros can be removed for a smaller
// code footprint

// Logging functions
#ifndef LFS_TRACE
#ifdef LFS_YES_TRACE
#define LFS_TRACE_(fmt, ...) \
    printf("%s:%d:trace: " fmt "%s\n", __FILE__, __LINE__, __VA_ARGS__)
#define LFS_TRACE(...) LFS_TRACE_(__VA_ARGS__, "")
#else
#define LFS_TRACE(...)
#endif
#endif

#ifndef LFS_DEBUG
#ifndef LFS_NO_DEBUG
#define LFS_DEBUG_(fmt, ...) debug_d(fmt,  __VA_ARGS__)
#define LFS_DEBUG(...) LFS_DEBUG_(__VA_ARGS__, "")
#else
#define LFS_DEBUG(...)
#endif
#endif

#ifndef LFS_WARN
#ifndef LFS_NO_WARN
#define LFS_WARN_(fmt, ...) debug_w(fmt,  __VA_ARGS__)
#define LFS_WARN(...) LFS_WARN_(__VA_ARGS__, "")
#else
#define LFS_WARN(...)
#endif
#endif

#ifndef LFS_ERROR
#ifndef LFS_NO_ERROR
#define LFS_ERROR_(fmt, ...) debug_e(fmt,  __VA_ARGS__)
#define LFS_ERROR(...) LFS_ERROR_(__VA_ARGS__, "")
#else
#define LFS_ERROR(...)
#endif
#endif

// Runtime assertions
#ifndef LFS_ASSERT
#ifndef LFS_NO_ASSERT
#define LFS_ASSERT(test) assert(test)
#else
#define LFS_ASSERT(test)
#endif
#endif


// Builtin functions, these may be replaced by more efficient
// toolchain-specific implementations. LFS_NO_INTRINSICS falls back to a more
// expensive basic C implementation for debugging purposes

// Min/max functions for unsigned 32-bit numbers
static inline uint32_t lfs_max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

static inline uint32_t lfs_min(uint32_t a, uint32_t b) {
    return (a < b) ? a : b;
}

// Align to nearest multiple of a size
static inline uint32_t lfs_aligndown(uint32_t a, uint32_t alignment) {
    return a - (a % alignment);
}

static inline uint32_t lfs_alignup(uint32_t a, uint32_t alignment) {
    return lfs_aligndown(a + alignment-1, alignment);
}

// Find the smallest power of 2 greater than or equal to a
static inline uint32_t lfs_npw2(uint32_t a) {
    return 32 - __builtin_clz(a-1);
}

// Count the number of trailing binary zeros in a
// lfs_ctz(0) may be undefined
static inline uint32_t lfs_ctz(uint32_t a) {
    return __builtin_ctz(a);
}

// Count the number of binary ones in a
static inline uint32_t lfs_popc(uint32_t a) {
    return __builtin_popcount(a);
}

// Find the sequence comparison of a and b, this is the distance
// between a and b ignoring overflow
static inline int lfs_scmp(uint32_t a, uint32_t b) {
    return (int)(unsigned)(a - b);
}

// Convert between 32-bit little-endian and native order
static inline uint32_t lfs_fromle32(uint32_t a) {
    return a;
}

static inline uint32_t lfs_tole32(uint32_t a) {
    return lfs_fromle32(a);
}

// Convert between 32-bit big-endian and native order
static inline uint32_t lfs_frombe32(uint32_t a) {
    return (a >> 24) | ((a >> 8) & 0xff00) | ((a << 8) & 0xff0000) | (a << 24);
}

static inline uint32_t lfs_tobe32(uint32_t a) {
    return lfs_frombe32(a);
}

// Calculate CRC-32 with polynomial = 0x04c11db7
#ifdef LFS_CRC
uint32_t lfs_crc(uint32_t crc, const void *buffer, size_t size) {
    return LFS_CRC(crc, buffer, size)
}
#else
uint32_t lfs_crc(uint32_t crc, const void *buffer, size_t size);
#endif

// Allocate memory, only used if buffers are not provided to littlefs
//
// littlefs current has no alignment requirements, as it only allocates
// byte-level buffers.
static inline void *lfs_malloc(size_t size) {
    (void)size;
    return NULL;
}

// Deallocate memory, only used if buffers are not provided to littlefs
static inline void lfs_free(void *p) {
    (void)p;
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
