#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
        // range is to ensure p has a high bit of 1
        uint64_t q = randBetween(UINT64_MAX>>2,UINT64_MAX>>1);
        if ((q % 12 != 5) || (!isPrime(q,DEFAULTK))) continue;
        p = (q<<1) + 1;
        printf("Testing p = %" PRIu64 "\n",p);
        // p must be prime and have a high bit of 1
    } while (p <= (UINT64_MAX>>1) || !isPrime(p,DEFAULTK));
    // p is prime with 2 as a primative root
    uint64_t d = randBetween(1,p-1);
    uint64_t e2 = modExp(g,d,p);
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
        perror("open(ptext.txt)");
        exit(EXIT_FAILURE);
    }
    int ctextfd = open("ctext.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if (ctextfd == -1) {
        perror("open(ctext.txt)");
        exit(EXIT_FAILURE);
    }
    uint64_t mblock = 0;
    while (read(ptextfd,&mblock,BLOCKBYTES) > 0) {
        printf("m = %016" PRIx64 "\n",mblock);
        uint64_t k = randBetween(0,p-1);
        uint64_t c1 = modExp(g,k,p);
        printf("c1 = %" PRIu64 "\n",c1);
        uint64_t c2 = modMul(modExp(e2,k,p),mblock,p);
        printf("c2 = %" PRIu64 "\n",c2);
        if (dprintf(ctextfd,"%" PRIu64 " %" PRIu64 "\n",c1,c2) < 0) {
            fprintf(stderr,"Error: Could not print to file \"ctext.txt\"\n");
            exit(EXIT_FAILURE);
        }
    }
    close(ptextfd);
    close(ctextfd);
}

void asymDecrypt() {
    uint64_t p, g, d;
    getKeys("prikey.txt",&p,&g,&d);
    int ctextfd = open("ctext.txt",O_RDONLY);
    if (ctextfd == -1) {
        perror("open(ctext.txt)");
        exit(EXIT_FAILURE);
    }
    int dtextfd = open("dtext.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if (dtextfd == -1) {
        perror("open(dtext.txt)");
        exit(EXIT_FAILURE);
    }
    while (true) {
        // this buffer will hold each line of the file
        char buffer[BUFSIZE] = {0};
        for (int i = 0;i<BUFSIZE;i++) {
            ssize_t readBytes = read(ctextfd,&(buffer[i]),1);
            if (readBytes == -1) {
                perror("read(ctext.txt)");
                exit(EXIT_FAILURE);
            }
            if (readBytes == 0) {
                close(dtextfd);
                close(ctextfd);
                return;
            }
            if (buffer[i] == '\n') break;
        }
        // parse out the 2 ciphertext values from the buffer
        char* secondStart = NULL;
        uint64_t c1 = (uint64_t) strtoull(buffer,&secondStart,10);
        //printf("c1 = %" PRIu64 "\n",c1);
        uint64_t c2 = (uint64_t) strtoull(secondStart,NULL,10);
        //printf("c2 = %" PRIu64 "\n",c2);
        // calculate m from the ciphertext
        uint64_t mblock = modMul(modExp(c1,p-1-d,p),c2,p);
        // now write the message bytes to the plaintext file
        if (write(dtextfd,&mblock,BLOCKBYTES) == -1) {
            perror("write(dtext.txt)");
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
