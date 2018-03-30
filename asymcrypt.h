#ifndef _ASYMCRYPT_H_
#define _ASYMCRYPT_H_

#include <stdint.h>
#include <sys/stat.h>

void keygen(uint64_t seed);

void writeKeys(const char* pathname,uint64_t first,uint64_t second,uint64_t third,mode_t mode);

#endif