#include <stdio.h>
#include <omp.h>
#define SIZE 100
#define CHUNK 10

int main () {
    
    double a[SIZE], b[SIZE] , c[SIZE], d[SIZE];
    
    for (int i = 0; i < SIZE ; i ++) {
            a[i] = i;
            b[i] = i;
    }
        
    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();
     
        #pragma omp for schedule (guided) nowait
        for(int i = 0; i < SIZE ; i += 1){
            c[i] = a[i] + b[i];
            printf(" Thread %d: c[%2d] est %g\n", tid, i, c[i]); 
        }

        #pragma omp for schedule (guided)
        for(int i = 0; i < SIZE ; i ++){
            d[i] = a[i] + c[i];
            printf(" Thread %d: d[%2d] est %g\n", tid, i, d[i]); 
        }
    }

    for (int i = 0; i < SIZE ; i += 1) {
        printf ("%g ", d[i]) ;
    }

    printf ("\n") ;
    return 0;
}