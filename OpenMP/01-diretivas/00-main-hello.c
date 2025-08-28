#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int ac, char **v){
    printf("\nHello World do OpenMP\n");
    
#pragma omp parallel
{
    int total_t = omp_get_num_threads();
    int id_t = omp_get_thread_num();
    printf("\t -> Eu sou a thread %d / %d \n", id_t, total_t);
}    
    return EXIT_SUCCESS;
}
