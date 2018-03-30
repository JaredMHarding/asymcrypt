#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "miller-rabin.h"
#include "asymcrypt.h"

#define MAXDIGITS 21

uint64_t promptSeed() {
    printf("Please provide a seed value (1,(2^64)-1): ");
    char seedStr[MAXDIGITS] = {0};
    fgets(seedStr,MAXDIGITS,stdin);
    return (uint64_t) strtoull(seedStr,NULL,10);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr,"Usage:\n");
        fprintf(stderr,"\"%s keygen <seed>\" OR\n",argv[0]);
        fprintf(stderr,"\"%s encrypt <seed>\" OR\n",argv[0]);
        fprintf(stderr,"\"%s decrypt\"\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    uint64_t seed = 0;
    if (argc < 3) {
        seed = promptSeed();
    } else {
        seed = (uint64_t) strtoull(argv[2],NULL,10);
    }
    // key generation mode
    if (strncmp(argv[1],"keygen",7) == 0) {
        keygen(seed);
    }
    // encryption mode
    else if (strncmp(argv[1],"encrypt",8) == 0) {
        asymEncrypt(seed);
    }
    // decryption mode
    else if (strncmp(argv[1],"decrypt",8) == 0) {
        
    }
}
