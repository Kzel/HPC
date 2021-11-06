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
	const int n = 10000;
	float *A, *B, *C;
	A = new float[n*n]; // in
	B = new float[n*n]; // in
	C = new float[n*n]; // out
	randomInit(A, n*n);
	randomInit(B, n*n);
	struct timeval t1, t2;

	// solver or computational part
	gettimeofday(&t1, NULL);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			C[i + j*n] = A[i + j*n] + B[i + j*n];
		}
	}
	gettimeofday(&t2, NULL);

	// post-processing part
	/* This could help you to verify the results ;-)
	std::cout << "Results:" << std::endl;
	for(int i = 0; i < n*n; i++) {
		std::cout << C[i] << " ";
	}
	std::cout << std::endl;
	*/
	delete[] A;
	delete[] B;
	delete[] C;

	float timeMs = ((t2.tv_sec * 1000000 + t2.tv_usec) -
	                (t1.tv_sec * 1000000 + t1.tv_usec)) / 1000.f;

	std::cout << "Computational time: " << timeMs << "ms" << std::endl;

	return 0;
}
