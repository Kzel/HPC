#include <cstdlib>    // rand()
#include <iostream>   // cout
#include <sys/time.h> // gettimeofday()

void randomInit(float* array, int n)
{
	for(int i = 0; i < n; i++)
		array[i] = rand() % 1000;
}

int main()
{
	// pre-processing part
	const int n = 40000000;
	float *A, *B, *C, *D;
	A = new float[n];
	B = new float[n];
	C = new float[n];
	D = new float[n];
	randomInit(A, n);
	randomInit(B, n);
	for(int i = 0; i < n; i++)
		D[i] = 0;
	float alpha = 0.333333;
	struct timeval t1, t2;

	// solver or computational part
	gettimeofday(&t1, NULL);
	for(int i = 0; i < n; i++) {
		C[i] = A[i] + B[i];
		D[i] = C[i] * alpha;
		A[i] = D[i] + C[i];
		B[i] = A[i] + 2.0f;
	}
	gettimeofday(&t2, NULL);

	// post-processing part
	/* This could help you to verify the results ;-)
	std::cout << "Results:" << std::endl;
	for(int i = 0; i < n; i++)
		std::cout << B[i] << " ";
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
