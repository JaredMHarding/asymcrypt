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

void keygen(uint64_t seed) {
    srand(seed);
    // choose 2 as the generator and pick a prime from that
    uint64_t g = 2;
    uint64_t p = 0;
    do {
        uint64_t q = randBetween(2,UINT64_MAX>>1);
        if ((!isPrime(q,DEFAULTK)) || (q % 12 != 5)) {
            continue;
        }
        p = (q<<1) + 1;
        // p must be prime and greater than any message block
    } while (!isPrime(p,DEFAULTK) || (p <= UINT32_MAX));
    // p is prime with 2 as a primative root
    uint64_t d = randBetween(1,p-1);
    uint64_t e2 = expBySquaring(g,d,p);
    // output the keys to files
    putKeys("pubkey.txt",p,g,e2,0644);
    putKeys("prikey.txt",p,g,d,0600);
    // output to screen for convenience
    printf("p = %" PRIu64 "\n",p);
    printf("g = %" PRIu64 "\n",g);
    printf("e2 = %" PRIu64 "\n",e2);
    printf("d = %" PRIu64 "\n",d);
}

void asymEncrypt(uint64_t seed) {
    srand(seed);
    // open necessary files
    uint64_t p, g, e2;
    getKeys("pubkey.txt",&p,&g,&e2);
    int ptextfd = open("ptext.txt",O_RDONLY);
    if (ptextfd == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    int ctextfd = open("ctext.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if (ctextfd == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    uint8_t mblock[BLOCKBYTES] = {0};
    uint32_t* mblockptr = (uint32_t*) mblock;
    ssize_t readBytes = 0;
    while ((readBytes = read(ptextfd,mblockptr,BLOCKBYTES)) > 0) {
        // pad a partial block using ANSI X.923 byte padding
        if (readBytes < BLOCKBYTES) {
            uint8_t padding = BLOCKBYTES - readBytes;
            mblock[BLOCKBYTES-1] = padding;
            for (int i = BLOCKBYTES-2;i>(readBytes-1);i--) {
                mblock[i] = 0x00;
            }
        }
        uint64_t k = randBetween(0,p-1);
        uint64_t c1 = expBySquaring(g,k,p);
        uint64_t c2 = (expBySquaring(e2,k,p)*(*mblockptr)) % p;
        if (dprintf(ctextfd,"%" PRIu64 " %" PRIu64 "\n",c1,c2) < 0) {
            fprintf(stderr,"Error: Could not print to file \"ctext.txt\"\n");
            exit(EXIT_FAILURE);
        }
    }
}

void putKeys(const char* pathname,uint64_t first,uint64_t second,uint64_t third,mode_t mode) {
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

void getKeys(const char* pathname,uint64_t* first,uint64_t* second,uint64_t* third) {
    int keyfd = open(pathname,O_RDONLY);
    if (keyfd == -1) {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    char keyBuf[BUFSIZE] = {0};
    // there should only be a need for one read for key files
    if (read(keyfd,keyBuf,BUFSIZE) == -1) {
        perror("read()");
        exit(EXIT_FAILURE);
    }
    char* secondStart = NULL;
    char* thirdStart = NULL;
    (*first) = (uint64_t) strtoull(keyBuf,&secondStart,10);
    (*second) = (uint64_t) strtoull(secondStart,&thirdStart,10);
    (*third) = (uint64_t) strtoull(thirdStart,NULL,10);
    close(keyfd);
}
