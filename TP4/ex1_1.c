#include <stdio.h>
int main () {
    # pragma omp parallel num_threads(3)
    printf (" Hello \n") ;
    printf (" World \n") ;
    return 0;
}