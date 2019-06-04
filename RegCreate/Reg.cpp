#include <Windows.h>
#include "winreg.h"
#include "stdio.h"
#include "conio.h"
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

void createReg() {
	LONG error = 0;
	HKEY hKey;
	DWORD dwDisp, dwData;
	char calc_hash[65];
	char * path = "MSLM_TX.exe";
	int result;
	result = calc_sha256(path, calc_hash);
	printf("%s\n", calc_hash);

	// 먼저 만들려는 키가 이미 존재하는 것인지 살펴본다.
	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Mslm", 0, KEY_ALL_ACCESS, &hKey);

	if (error != ERROR_SUCCESS) // 없다면 새로 생성한다. 
	{
		// 키를 생성한다.
		error = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Mslm", 0, "REG_BINARY", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDisp);
		// 위의 키 밑에 Type이란 DWORD 타입의 값을 만들고 1로 초기화 
		// 위의 키 밑에 Group이란 문자열 타입의 값을 만들고 lpData의 값으로 초기화 
		error = RegSetValueEx(hKey, "MSLMTX", 0, REG_SZ, (BYTE*)calc_hash, strlen(calc_hash));
		// 키를 닫는다. 
		RegCloseKey(hKey);
	}
	else {
		error = RegSetValueEx(hKey, "MSLMTX", 0, REG_SZ, (BYTE*)calc_hash, strlen(calc_hash));
		// 키를 닫는다. 
		RegCloseKey(hKey);
	}
}
void deleteReg() {
	LONG lResult;
	HKEY hKey;
	DWORD dwBytes = 100;
	char* str = "MSLMTX";

	// open Regstry Key
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"Software\\Mslm",
		0, KEY_ALL_ACCESS, &hKey);
	if (lResult != ERROR_SUCCESS)
	{
		printf("reg open error!\n\n");
	}
	

	lResult = RegDeleteKey(hKey, str);  // 삭제

	if (lResult == ERROR_SUCCESS)
		printf("delete success \n\n");
	else
		printf("delete fail \n\n");

	RegCloseKey(hKey);
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		createReg();
	}
	else {
		deleteReg();
	}

	return 0;
}