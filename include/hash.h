#ifndef _HASH_H
#define _HASH_H

#include "leaf.h"

#include <openssl/sha.h>

#define SHA256_OUT_LENGTH (65)

void sha256_string(char *string, char * outputBuffer);

#endif /* _HASH_H */
