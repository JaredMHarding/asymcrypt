#ifndef _MILLER_RABIN_H_
#define _MILLER_RABIN_H_

#include <stdbool.h>
#include <stdint.h>

#define DEFAULTK 16

bool isPrime(uint64_t n, int k);

uint64_t modExp(__uint128_t base, __uint128_t exp, uint64_t mod);

uint64_t modMul(__uint128_t a, __uint128_t b, uint64_t mod);

uint64_t randBetween(uint64_t min, uint64_t max);

#endif
