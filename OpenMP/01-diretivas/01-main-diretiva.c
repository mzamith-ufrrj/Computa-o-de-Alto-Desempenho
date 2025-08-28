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
    printf("\nDiretivas do OpenMP\n");
    omp_set_nested(1);
#pragma omp parallel
{
    
    #pragma omp parallel
    {
        #pragma omp parallel
        {
            int total_t = omp_get_num_threads();
            int id_t = omp_get_thread_num();
            printf("\t -> Eu sou a thread %d / %d \n", id_t, total_t);
            unsigned int t = 0;
            for (unsigned int i = 0; i < (id_t + 1) * 1000000000; i++){
                t = i + 1;
            }
            printf("\t -> Eu sou a thread %d / %d -> %u \n", id_t, total_t, t);
        }

    }
}    
    

    return EXIT_SUCCESS;

}
