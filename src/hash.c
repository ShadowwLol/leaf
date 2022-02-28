#include "../include/hash.h"

void sha256_string(char *string, char * outputBuffer){
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, MEL_strlen(string));
	SHA256_Final(hash, &sha256);

	for(size_t i = 0; i < SHA256_DIGEST_LENGTH; i++){
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}

	outputBuffer[SHA256_OUT_LENGTH-1] = 0;
}
