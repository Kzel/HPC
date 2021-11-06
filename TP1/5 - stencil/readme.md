# Stencil code

## Compile the executable

The code is written in `C++11` so you will need to use `g++` compiler with `-std=c++11` option.
There is a `Makefile` to help you in this task. The compiling is very simple: 
```
> make
```
This will generate the `bin/stencil` executable file.
If you want to re-compile after a code modification just re-do:
```
> make
```

If you want to re-compile the code from scratch:
```
> make clean; make
```

You can modify the `Makefile` options:
```
# compiler choice
CC = g++
# compiling options
CFLAGS = -O3 -march=native -Wall -std=c++11
# compiling options (debug mode)
DCFLAGS = -g -O0
# compiling flags
LDFLAGS = -fopenmp
# library link
LIBS = -lm
```

If you modify something in the `Makefile`, think to launch a from scratch compiling:
```
> make clean; make
```

## Run the executable

The generated executable `bin/stencil` requires some arguments:
```
> ./bin/stencil cols rows nIte verbose
```
Here is an example with a 100 columns and 100 rows grid and  with 10 iterations (verbose mode off):
```
> ./bin/stencil 100 100 10 0
0.0762939 MB allocated

Simulation configuration:
 - nIte     = 10
 - cols     = 100
 - rows     = 100
 - nThreads = 1
 - dataType = float
Stencil time:   0.562 ms
Stencil Gflops: 0
Stencil GB/s:   0

Deallocations...
```

Verbose mode displays the initial and the final grid. Try to use it with small grids:
```
> ./bin/stencil 8 8 3 1
0.000488281 MB allocated

Initial grid: 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 2.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 

Simulation configuration:
 - nIte     = 3
 - cols     = 8
 - rows     = 8
 - nThreads = 4
 - dataType = float
Stencil time:    0.003 ms
Stencil Gflop/s: 0
Stencil GB/s:    0

Final grid: 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 
1.000 1.000 1.024 1.024 1.024 1.000 1.000 1.000 
1.000 1.024 1.048 1.096 1.048 1.024 1.000 1.000 
1.000 1.024 1.096 1.104 1.096 1.024 1.008 1.000 
1.000 1.024 1.048 1.096 1.048 1.024 1.000 1.000 
1.000 1.000 1.024 1.024 1.024 1.000 1.000 1.000 
1.000 1.000 1.000 1.008 1.000 1.000 1.000 1.000 
1.000 1.000 1.000 1.000 1.000 1.000 1.000 1.000 

Deallocations...
```

As you can see, the basic code provides the computational time but Gflop/s and GB/s are not enabled.

## Work to do

The source code is available in the `src` folder (`src/stencil.cpp`).
In this code we use a generic `floatType` type instead of declaring directly `float` or `double` types. This have been made to easily modify the precision of floating-point numbers.

### Code analysis

1. Count the number of floating-point operations (flops) in the `computeStencil` routine (lines 27 to 64) and report this value in the `flops` variable line 156.
2. Estimate the number of memory accesses (memops) in the `computeStencil` routine (lines 27 to 64) and report this value in the `memops` variable line 158. Now you will have dynamic Gflop/s (code performance) and GB/s (memory bandwidth utilization).
3. Launch code with different `floatType` precisions (line 17: `float` for simple precision numbers, `double` for double precision numbers) but keep the same problem size. What did you notice? Can you explain it?
4. Calculate the peak performance of your processor (in Gflop/s).
5. Measure the memory bandwidth of your computer (in GB/s with STREAM).
6. Calculate the attainable performance of this stencil code with the Roofline model.
7. Is this a memory-bound code or a compute-bound code?
8. Try to launch the code with different problem sizes and keep the results into a file. What can you say about the CPU performances evolution?

Example of strategy (variable number `cols` and fixed number of `rows`):

```
> ./bin/stencil 128 256 10 0
```

```
> ./bin/stencil 256 256 10 0
```

```
> ./bin/stencil 512 256 10 0
```

```
> ./bin/stencil 1024 256 10 0
```

```
> ./bin/stencil 2048 256 10 0
```

### Code modifications

1. Implement **cache blocking** technique with an abitrary size of blocks.
2. Determine the optimal size of blocks (for your computer).
3. Try to see when cache blocking brings performance improvements (keep the results into a file).
4. Make a multi-threaded code with OpenMP.
5. Calculate speed up for small problem sizes (`cols = 512`, `rows = 256`) and for big problem sizes (`cols = 32768`, `row = 512`).
6. What can you say about those speeds up?
7. If you could make improvements on your computer architecture, what will you do in order to improve the stencil code performances?
