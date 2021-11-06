# Pre-processing exercise

## Compile the executable

The code is written in `C++` so you will need to use `g++` compiler.
You can compile with different optimisation levels:
```
> g++ main.cpp
```
```
> g++ -O1 main.cpp
```
```
> g++ -O2 main.cpp
```
```
> g++ -O3 -fopt-info-vec main.cpp
```
```
> g++ -O3 -fopt-info-vec -march=native main.cpp
```
Those commands will produce an `a.out` executable.

## Run the executable

Run the executable with:
```
> ./a.out
```
This will print out the computational time of the code.
```
Computational time: 11069.4ms
```

`time` command can give you the total execution time:
```
> time ./a.out
Computational time: 2132.48ms

real	0m5.620s
user	0m4.472s
sys	0m1.143s
```

## Work to do

Try to pre-compute things in order to minimize the total number of operations in the code.

>Understand what this code is doing could help you in this task. For example you can write the `D[0]` computation on a piece of paper (of course you can take a small value for `n`).
