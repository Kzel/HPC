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
	const int n = 40000002;
	float *A, *B;
	A = new float[n]; // in
	B = new float[n]; // out
	randomInit(A, n);
	A[0] = 0.0f; A[n -1] = 0.0f;
	struct timeval t1, t2;

	// solver or computational part
	gettimeofday(&t1, NULL);

	for(int i = 1; i < n; i++) {
		if(i >= 1 && i < n -1) {
			switch(i % 4) {
				case 0: B[i] = A[i] * 0.3333f; break;
				case 1: B[i] = A[i] + 1.3333f; break;
				case 2: B[i] = A[i] - 0.7555f; break;
				case 3: B[i] = A[i] * 1.1111f; break;
				default: break;
			}
		}
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
