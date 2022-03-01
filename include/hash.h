#ifndef _HASH_H
#define _HASH_H

#include "leaf.h"

#include <openssl/sha.h>

#define SHA512_OUT_LENGTH (129)
#define SHA256_OUT_LENGTH (65)

void sha512_string(char *string, char * outputBuffer);
void sha256_string(char *string, char * outputBuffer);

#endif /* _HASH_H */
