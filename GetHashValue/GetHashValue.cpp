#include <stdlib.h>
#include <stdio.h>
#include <openssl/sha.h>
void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char *outputBuffer);

int calc_sha256(char *path, char *output) {

	FILE* file = fopen(path, "rb");
	if (!file) return -1;

	unsigned char hash[SHA256_DIGEST_LENGTH];
	const int bufSize = 32768;
	unsigned char *buffer = (unsigned char *)malloc(bufSize);
	SHA256_CTX sha256;
	int bytesRead = 0;

	if (!buffer) return -2;

	SHA256_Init(&sha256);

	while ((bytesRead = fread(buffer, 1, bufSize, file))) {
		SHA256_Update(&sha256, buffer, bytesRead);
	}

	SHA256_Final(hash, &sha256);
	sha256_hash_string(hash, output);
	fclose(file);

	return 0;
}

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char *outputBuffer)
{
	int i = 0;
	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
}

void main(int argc, char *argv[]) {

	char calc_hash[65];
	char * path = "Project1.exe";
	int result;
	result = calc_sha256(path, calc_hash);
	printf("%s\n", calc_hash);

	return;
}