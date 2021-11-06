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
	gettimeofday(&t1, NULL);

	float c = 0;
  float d = 0;

  for(int j = 0; j < n; j++){
      c += C[j];
  }

	for(int i = 0; i < n; i++) {

		float ab = A[i] + B[i];
		d +=  ab * c;
		D[i] = d;

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
