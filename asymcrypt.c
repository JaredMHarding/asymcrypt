#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "asymcrypt.h"
#include "miller-rabin.h"

#define ACCURACY 8

void keygen(uint64_t seed) {
    srand(seed);
    // choose 2 as the generator and pick a prime from that
    uint64_t g = 2;
    uint64_t p = 0;
    do {
        uint64_t q = randBetween(2,UINT64_MAX>>1);
        if ((!isPrime(q,ACCURACY)) || (q % 12 != 5)) {
            continue;
        }
        p = (q<<1) + 1;
    } while (!isPrime(p,ACCURACY));
    // p is prime with 2 as a primative root
    uint64_t d = randBetween(1,p-1);
    uint64_t e2 = expBySquaring(g,d,p);
    // output the keys to files
    writeKeys("pubkey.txt",p,g,e2,0644);
    writeKeys("prikey.txt",p,g,d,0600);
    // output to screen for convenience
    printf("p = %" PRIu64 "\n",p);
    printf("g = %" PRIu64 "\n",g);
    printf("e2 = %" PRIu64 "\n",e2);
    printf("d = %" PRIu64 "\n",d);
}

void writeKeys(const char* pathname,uint64_t first,uint64_t second,uint64_t third,mode_t mode) {
    int keyfd = open(pathname,O_WRONLY|O_CREAT|O_TRUNC,mode);
    if (keyfd == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    if (dprintf(keyfd,"%" PRIu64 " %" PRIu64 " %" PRIu64 "",first,second,third) < 0) {
        fprintf(stderr,"Error: Could not print to file \"%s\"\n",pathname);
        exit(EXIT_FAILURE);
    }
    close(keyfd);
}