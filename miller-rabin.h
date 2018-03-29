#ifndef _MILLER_RABIN_H_
#define _MILLER_RABIN_H_

#include <stdbool.h>
#include <stdint.h>

bool isPrime(uint32_t n, uint32_t k);

uint32_t factorOut2s(uint32_t n, uint32_t* rPtr);

uint32_t expBySquaring(uint32_t base, uint32_t exp);

#endif