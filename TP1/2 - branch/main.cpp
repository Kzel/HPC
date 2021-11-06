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
 	int r = (n-1) % 4 ;

	for(int i = 1; i < n-r; i+=4) {
		//if(i >= 1 && i < n -1) {
		B[i] = A[i] * 0.3333f;
		B[i+1] = A[i+1] + 1.3333f;
		B[i+2] = A[i+2] - 0.7555f;
		B[i+3] = A[i+3] * 1.1111f;

		switch(r){
			case 1: B[n-1] = A[n-1] * 1.1111f; break;
			case 2: B[n-2] = A[n-2] - 0.7555f; break;
			case 3: B[n-3] = A[n-3] * 1.3333f; break;
			case 0:  break;
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
