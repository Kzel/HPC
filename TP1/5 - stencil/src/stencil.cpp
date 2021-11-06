#include <sys/time.h> // gettimeofday()
#include <iostream>   // cout
#include <iomanip>    // setprecision()
#include <string>     // string(), stoi()
using namespace std;

#ifdef _OPENMP
#include <omp.h>
#else
inline void omp_set_num_threads(int) {           }
inline int  omp_get_num_threads(   ) { return 1; }
inline int  omp_get_max_threads(   ) { return 1; }
inline int  omp_get_thread_num (   ) { return 0; }
#endif

/* define the precision of floating-point numbers (float = simple precision, double = double precision) */
using floatType = float;

/* stencil coefficients */
#define COEF_TOP 0.2
#define COEF_LEF 0.2
#define COEF_CEN 0.2
#define COEF_RIG 0.2
#define COEF_BOT 0.2

template <typename T>
void computeStencil(T* dataEven, T* dataOdd,
                    const unsigned long cols, const unsigned long rows, const unsigned long nIte,
                    const T coefTop, const T coefLeft, const T coefCenter, const T coefRight, const T coefBottom)
{
	T *data[2];
	data[0] = dataEven;
	data[1] = dataOdd;

	T *dataRead, *dataWrite;

	for(unsigned long iIte = 0; iIte < nIte; iIte++)
	{
		dataRead  = data[(iIte +0) % 2]; 
		dataWrite = data[(iIte +1) % 2];
		
		for(unsigned long j = 1; j < rows -1; j++)
		{
			for(unsigned long i = 1; i < cols -1; i++)
			{
				unsigned long pos = i + j*cols;

				T top    = dataRead[pos - cols];
				T left   = dataRead[pos - 1   ];
				T center = dataRead[pos       ];
				T right  = dataRead[pos + 1   ];
				T bottom = dataRead[pos + cols];

				T res = coefTop    * top    +
				        coefLeft   * left   +
				        coefCenter * center +
				        coefRight  * right  +
				        coefBottom * bottom;

				dataWrite[pos] = res;
			}
		}
	}
}

template <typename T>
void diplayGrid(T *grid, unsigned long cols, unsigned long rows, unsigned precision = 3)
{
	for(unsigned long j = 0; j < rows; ++j)
	{
		for(unsigned long i = 0; i < cols; ++i)
		{
			cout << setprecision(precision) << fixed << grid[i + j*cols] << " ";
		}
		cout << endl;
	}
}

int main(int argc, char** argv)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// pre-processing part ////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(argc < 5)
	{
		cout << "usage: ./" << string(argv[0]) << " cols rows nIte verbose" << endl;
		exit(0);
	}

	unsigned long cols = stoi(string(argv[1]));
	unsigned long rows = stoi(string(argv[2]));
	unsigned long nIte = stoi(string(argv[3]));
	bool verbose = stoi(string(argv[4]));

	// data allocation
	floatType *dataEven = new floatType[cols*rows];
	floatType *dataOdd  = new floatType[cols*rows];
	unsigned long nBytes = cols * rows * sizeof(floatType);
	cout << ((2 * nBytes)/(1024.f*1024.f)) << " MB allocated" << endl;

	// data grid initialization
	for(unsigned long j = 0; j < rows; ++j)
		for(unsigned long i = 0; i < cols; ++i)
		{
			unsigned long pos = i + j * cols;

			dataEven[pos] = dataOdd [pos] = 1.0;

			if((i == ((cols / 2) -1)) && (j == ((rows / 2) -1)))
				dataEven[pos] = dataOdd[pos] = 2.0;
		}
	cout << endl;

	if(verbose)
	{
		cout << "Initial grid: " << endl;
		diplayGrid<floatType>(dataEven, cols, rows);
		cout << endl;
	}

	unsigned short nThreads = omp_get_max_threads();

	cout << "Simulation configuration:"  << endl;
	cout << " - nIte     = " << nIte     << endl;
	cout << " - cols     = " << cols     << endl;
	cout << " - rows     = " << rows     << endl;
	cout << " - nThreads = " << nThreads << endl;
	cout << " - dataType = " << ((sizeof(floatType) == 4) ? "float" : "double") << endl;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// solver or computational part ///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct timeval t1, t2;
	gettimeofday(&t1, NULL);

	computeStencil<floatType>(dataEven, dataOdd,
	                          cols, rows, nIte,
	                          (floatType)COEF_TOP, 
	                          (floatType)COEF_LEF, 
	                          (floatType)COEF_CEN, 
	                          (floatType)COEF_RIG, 
	                          (floatType)COEF_BOT);

	gettimeofday(&t2, NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// post-processing part ///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float timeMs = ((t2.tv_sec * 1000000 + t2.tv_usec) -
	                (t1.tv_sec * 1000000 + t1.tv_usec)) / 1000.f;

	// make some statistics on the previous kernel
	float flops  = 9.f * (float)nIte * ((float)cols*(float)rows);
	float gflops = (flops / 1024.f / 1024.f / 1024.f) / (timeMs / 1000.f);
	float memops = (float)nIte * 4.f * (float)rows * (float)cols;
	float gbytes = ((memops * sizeof(floatType)) / 1024.f / 1024.f / 1024.f) / (timeMs / 1000.f);
	cout << "Stencil time:    " << timeMs << " ms" << endl;	
	cout << "Stencil Gflop/s: " << gflops          << endl;
	cout << "Stencil GB/s:    " << gbytes          << endl;

	if(verbose)
	{
		cout << endl << "Final grid: " << endl;
		if(nIte % 2)
			diplayGrid<floatType>(dataOdd,  cols, rows);
		else
			diplayGrid<floatType>(dataEven, cols, rows);
	}

	cout << endl << "Deallocations..." << endl;
	delete[] dataEven;
	delete[] dataOdd;

	return 0;
}
