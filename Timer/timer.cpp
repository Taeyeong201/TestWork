#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
typedef long long int64;

LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;
int64 elapsed;

int main() {
	QueryPerformanceFrequency(&Frequency);
	for (int i = 0; i < 5; i++) {
		QueryPerformanceCounter(&BeginTime);
		Sleep(2000);
		QueryPerformanceCounter(&Endtime);
		elapsed = Endtime.QuadPart - BeginTime.QuadPart;
		printf("%f \n", elapsed);
		float duringtime = elapsed / Frequency.QuadPart;
		printf("%f \n", duringtime);
	}
}