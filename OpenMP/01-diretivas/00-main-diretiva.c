#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
    #include <omp.h>
    #pragma message "OpenMP definido"
#else
    #define omp_get_num_threads() 0
    #define omp_get_thread_num() 0
    #pragma message "OpenMP NÃO definido"
#endif
int main (int ac, char **v){
    int total_t = omp_get_num_threads();
    int id_t = omp_get_thread_num();
    printf("\nDiretivas do OpenMP\n");
    printf("\t -> Eu sou a thread %d / %d \n", id_t, total_t);

    return EXIT_SUCCESS;

}
