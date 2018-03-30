#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "miller-rabin.h"

bool isPrime(uint64_t n, int k) {
    // Corner cases
    if ( n == 2 || n == 3 ) return true;
    if ( n <= 1 || !(n & 0x1) ) return false;
    // n is odd and greater than 3
    // r is the power of 2 that divides n-1
    int r = 0;
    // d will the odd number left over after dividing out the 2s
    uint64_t d = n-1;
    while ((d & 0x1) == 0) {
        d >>= 1;
        r++;
    }
    // witness loop, repeat k times
    for (int i = 0;i<k;i++) {
        uint64_t a = randBetween(2,n-2);
        uint64_t x = expBySquaring(a,d,n);
        if ((x == 1) || (x == n-1)) continue;
        // repeat r-1 times
        for (int j = 1;j <= (r-1);j++) {
            x = expBySquaring(x,2,n);
            if (x == 1) return false;
            if (x == n-1) goto KLOOP;
        }
        return false;
KLOOP:
        continue;
    }
    return true;
}

uint64_t expBySquaring(uint64_t base, uint64_t exp, uint64_t modulus) {
    uint64_t result = 1;
    while (exp) {
        if ((exp & 0x1) == 1) {
            result = (__uint128_t) base*result % modulus;
        }
        exp >>= 1;
        base = (__uint128_t) base*base % modulus;
    }
    return result;
}

uint64_t randBetween(uint64_t min, uint64_t max) {
    // assuming rand() is 32-bits
    uint64_t r = (((uint64_t) rand()) << 32) | (((uint64_t) rand()) & UINT32_MAX);
    uint64_t result = min + (uint64_t) ((double)(max-min+1)*r/(UINT64_MAX+1.0));
    return result;
}