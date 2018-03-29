#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "miller-rabin.h"

#define ACCURACY 8

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("All primes less than 100:\n");
    for (uint32_t i = 1;i < 100;i++) {
        if (isPrime(i,ACCURACY)) {
            printf("%" PRIu32 " ",i);
        }
    }
}