// 1_3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#define p 4 // количество дочерних потоков
double pi[p]; 
int n = 1000000;

unsigned int __stdcall ThreadFunction(void* pvParam) {
	int nParam = (int)pvParam;
	int i, start;
	double h, sum, x; h = 1. / n; sum = 0.;
	start = nParam;
	for (i = start; i < n; i += p) {
		x = h * i; sum += 4. / (1. + x * x);
	}
	pi[nParam] = h * sum;
	return 0;

}

int main() {
	HANDLE hThreads[p]; int k; double sum; 
	LARGE_INTEGER liFrequency, liStartTime, liFinishTime;
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liStartTime);

	for (k = 0; k < p; ++k) {
		hThreads[k] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, (void*)k, 0, NULL);
		if (hThreads[k] == NULL) // обработка ошибки 
		{
			printf("Create Thread %d Error=%d\n", k, GetLastError()); return -1;
		}
	} 

	WaitForMultipleObjects(p, hThreads, TRUE, INFINITE);
	for (k = 0; k < p; ++k) {
		CloseHandle(hThreads[k]);
		sum = 0.;
		for (k = 0; k < p; ++k)
			sum += pi[k];
		printf("PI = %.16f\n", sum);
	}

	QueryPerformanceCounter(&liFinishTime);
	double dElapsedTime = 1000.*(liFinishTime.QuadPart - liStartTime.QuadPart) / liFrequency.QuadPart;
	printf("Time = %f\n", dElapsedTime);
	return 0;
}
