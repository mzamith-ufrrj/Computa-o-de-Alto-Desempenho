#include <assert.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <omp.h>
#define ALING 64
/*
 * Neste exemplo são passados o tamanho do vetor em MB e a quantidade de threads.
 * Observa-se o uso do private(x). A sua falta torna x shared e há uma perda de
 * performance.
 */
int main (int ac, char **av){
    unsigned int  nThreads = 4,
                  size = 21,
                  *vet = NULL;
    
    double *a = NULL, *b = NULL, *c = NULL;

    printf("\nExemplo de OpenMP parallel for\n");
    a = (double *) malloc(sizeof(double) * size);
    b = (double *) malloc(sizeof(double) * size);
    c = (double *) malloc(sizeof(double) * size);
    vet = (unsigned int *) malloc(sizeof(unsigned int) * size);
    for (unsigned int i = 0; i < size; i++){
        a[i] = (double) (i+1);
        b[i] = (double) (i+1) / 100.0f;
        c[i] = 0.0f;
    }
    omp_set_num_threads(nThreads);
    #pragma omp parallel
    {
            unsigned int id_t = omp_get_thread_num();
            printf("Thread [%u] created \n", id_t);
            #pragma omp for
            for (unsigned int i = 0; i < size; i++){
                vet[i] = id_t;
                c[i] = a[i] + b[i];
            }
                
    }


    for (unsigned int i = 0; i < size; i++){
        printf("%lf = %lf + %lf computador pela thread %u\n", c[i], a[i], b[i], vet[i]);
    }

    free(a);free(b);free(c);
    return EXIT_SUCCESS;
}
