#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "miller-rabin.h"

bool isPrime(uint32_t n, uint32_t k) {
    // Corner cases
    if ((n <= 1) || (n == 4)) return false;
    if (n <= 3) return true;
    if ((n & 0x1) == 0) return false;
    // n is odd and greater than 3
    // r is the power of 2 that divides n-1
    uint32_t r;
    uint32_t d = factorOut2s(n-1,&r);
    // witness loop, repeat k times
    while (k-- > 0) {
        // pick a random integer a in the range [2, n - 2] inclusive
        // rand() will be seeded in the main.c function
        uint32_t a = (uint32_t) ((rand() % (n-3)) + 2);
        uint32_t x = expBySquaring(a,d) % n;
        if ((x == 1) || (x == (n-1))) continue;
        bool cond2True = false;
        // repeat r-1 times
        while (r-- > 1) {
            x = (x*x) % n;
            if (x == 1) {
                return false;
            }
            if (x == (n-1)) {
                cond2True = true;
                break;
            }
        }
        if (cond2True == true) continue;
        return false;
    }
    return true;
}

uint32_t factorOut2s(uint32_t n, uint32_t* rPtr) {
    (*rPtr) = 0;
    while ((n & 0x1) == 0) {
        n = n >> 1;
        (*rPtr)++;
    }
    return n;
}

uint32_t expBySquaring(uint32_t base, uint32_t exp) {
    uint32_t result = 1;
    while (exp) {
        if (exp & 0x1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}