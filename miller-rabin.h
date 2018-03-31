#ifndef _MILLER_RABIN_H_
#define _MILLER_RABIN_H_

#include <stdbool.h>
#include <stdint.h>

#define DEFAULTK 16

bool isPrime(uint64_t n, int k);

uint64_t expBySquaring(uint64_t base, uint64_t exp, uint64_t modulus);

uint64_t randBetween(uint64_t min, uint64_t max);

#endif
