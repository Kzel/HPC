#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main () {
    int val;
    srand(0);

    #pragma omp parallel num_threads(4) private (val)
    {
      val = rand();
      sleep(1);
      printf ("My val : %d\n", val);
    }

    return 0;
}
