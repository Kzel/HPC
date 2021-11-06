#include <cstdlib>    // rand()
#include <iostream>   // cout
#include <sys/time.h> // gettimeofday()

void randomInit(float* array, int n)
{
	for(int i = 0; i < n; i++) {
		array[i] = rand() % 1000;
	}
}

void kernel(const float *A, float *B, int i)
{
	B[i] = ((A[i] - 1.1111f) + (B[i] - 0.1111f)) * 0.3333f;
}

int main()
{
	// pre-processing part
	const int n = 30000000;
	float *A, *B;
	A = new float[n]; // in
	B = new float[n]; // out
	randomInit(A, n);
	randomInit(B, n);
	struct timeval t1, t2;

	// solver or computational part
	gettimeofday(&t1, NULL);
	for(int i = 0; i < n; i++) {
		kernel(A, B, i);
	}
	gettimeofday(&t2, NULL);

	// post-processing part
	/* This could help you to verify the results ;-)
	std::cout << "Results:" << std::endl;
	for(int i = 0; i < n; i++) {
		std::cout << B[i] << " ";
	}
	std::cout << std::endl;
	*/
	delete[] A;
	delete[] B;

	float timeMs = ((t2.tv_sec * 1000000 + t2.tv_usec) -
	                (t1.tv_sec * 1000000 + t1.tv_usec)) / 1000.f;

	std::cout << "Computational time: " << timeMs << "ms" << std::endl;

	return 0;
}
