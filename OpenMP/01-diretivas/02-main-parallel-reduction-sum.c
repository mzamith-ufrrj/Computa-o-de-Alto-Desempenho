#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#define ALING 64

int main (int ac, char **av){
    unsigned long size = strtoul(av[1], NULL, 0);
    double *vet  = NULL;
    //unsigned int nThreads = atoi(av[2]);
    //srand(42);


    size *= 1048576 / sizeof(double);

    printf("\n Redução: \n");
    printf("               Tamanho: %ul em MB \n", (size * sizeof(double)) / 1048576);
    printf("              Posições: %ul\n", size);

    assert(posix_memalign((void**)(&vet), ALING, size * sizeof(double)) == 0);


    #pragma omp parallel for
    for (unsigned long i = 1; i < size; i++){
        vet[i] = (double) i / (double) size;
    }



    double sum = 0.0f;
    #pragma omp parallel for shared(size, vet) reduction(+:sum)
    for (unsigned long i = 0; i < size; i++){
        sum += vet[i];
    }

    printf("Resultado da soma %lf\n", sum);

    free(vet);

    return EXIT_SUCCESS;
}

