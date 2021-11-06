#include <cstdlib>    // rand()
#include <iostream>   // cout
#include <sys/time.h> // gettimeofday()

void randomInit(float* array, int n)
{
	for(int i = 0; i < n; i++) {
		array[i] = rand() % 1000;
	}
}

int main()
{
	// pre-processing part
	const int n = 30000;
	float *A, *B, *C, *D;
	A = new float[n]; // in
	B = new float[n]; // in
	C = new float[n]; // in
	D = new float[n]; // out
	randomInit(A, n);
	randomInit(B, n);
	randomInit(C, n);
	for(int i = 0; i < n; i++) {
		D[i] = 0;
	}
	struct timeval t1, t2;

	// solver or computational part
	int r = n%3;
	float a0, a1, a2, b0, b1, b2, c0, c1, c2, d0, d1, d2;

	gettimeofday(&t1, NULL);
	for(int j = 0; j < n-r; j++) {
		c0 = C[j]; c1 = C[j+1]; c2 = C[j+2];

		for(int i = 0; i < n-r; i+=3) {
			a0 = A[i]; a1 = A[i+1]; a2 = A[i+2];
			b0 = B[i]; b1 = B[i+1]; b2 = B[i+2];
			d0 = D[i]; d1 = D[i+1]; d2 = D[i+2];

			d0 = d0 + (a0 + b0) * c0;
			d1 = d1 + (a1 + b1) * c1;
			d2 = d2 + (a2 + b2) * c2;

			D[i] = d0; D[i+1] = d1; D[i+2] = d2;
			//D[i] = D[i] + (A[i] + B[i]) * C[j];

		}
		switch (r) {
			case 2: D[n-2] = D[n-2] + (A[n-2] + B[n-2]) * C[n-2];
			case 1: D[n-1] = D[n-1] + (A[n-1] + B[n-1]) * C[n-1];
			case 0: break;
		}
	}

	gettimeofday(&t2, NULL);

	// post-processing part
	// This could help you to verify the results ;-)
  /*
	std::cout << "Results:" << std::endl;
	for(int i = 0; i < n; i++) {
		std::cout << D[i] << " ";
	}
	std::cout << std::endl;
	*/
	delete[] A;#include <cstdlib>    // rand()
#include <iostream>   // cout
#include <sys/time.h> // gettimeofday()

void randomInit(float* array, int n)
{
	for(int i = 0; i < n; i++) {
		array[i] = rand() % 1000;
	}
}

int main()
{
	// pre-processing part
	const int n = 30000;
	float *A, *B, *C, *D;
	A = new float[n]; // in
	B = new float[n]; // in
	C = new float[n]; // in
	D = new float[n]; // out
	randomInit(A, n);
	randomInit(B, n);
	randomInit(C, n);
	for(int i = 0; i < n; i++) {
		D[i] = 0;
	}
	struct timeval t1, t2;

	// solver or computational part
	gettimeofday(&t1, NULL);
	for(int j = 0; j < n; j++) {
		for(int i = 0; i < n; i++) {
			D[i] = D[i] + (A[i] + B[i]) * C[j];
		}
	}
	gettimeofday(&t2, NULL);

	// post-processing part
	/* This could help you to verify the results ;-)
	std::cout << "Results:" << std::endl;
	for(int i = 0; i < n; i++) {
		std::cout << D[i] << " ";
	}
	std::cout << std::endl;
	*/
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;

	float timeMs = ((t2.tv_sec * 1000000 + t2.tv_usec) -
	                (t1.tv_sec * 1000000 + t1.tv_usec)) / 1000.f;

	std::cout << "Computational time: " << timeMs << "ms" << std::endl;

	return 0;
}

	delete[] B;
	delete[] C;
	delete[] D;

	float timeMs = ((t2.tv_sec * 1000000 + t2.tv_usec) -
	                (t1.tv_sec * 1000000 + t1.tv_usec)) / 1000.f;

	std::cout << "Computational time: " << timeMs << "ms" << std::endl;

	return 0;
}
