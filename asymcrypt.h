#ifndef _ASYMCRYPT_H_
#define _ASYMCRYPT_H_

#include <stdint.h>
#include <sys/stat.h>

#define BUFSIZE 64
#define BLOCKBYTES 8

void keygen(uint64_t seed);

void asymEncrypt(uint64_t seed);

void asymDecrypt();

void putKeys(const char* pathname,uint64_t first,uint64_t second,uint64_t third,mode_t mode);

void getKeys(const char* pathname,uint64_t* first,uint64_t* second,uint64_t* third);

#endif
