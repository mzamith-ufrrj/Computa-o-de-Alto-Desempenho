#include <assert.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ALING 64
double getPI(double epsilon){
    double pi0  = 0.0f,
           pi1  = 0.0f,
           a    = 0.0f;
    int  flag  = 1;

    do{
        pi1 += (4.0f / (2.0 * a + 1.0f)) * pow(-1.0f, a);
        flag = fabs(pi1 - pi0) > epsilon;
        pi0 = pi1;
        a++;
    }while (flag);
    return pi1;

}

double getGoldenRation(double epsilon){
    unsigned long a  = 1.0f,
                  b  = 1.0f,
                  c  = 0.0f;

    double phi0 = 0.0f,
           phi1 = 0.0f;
    int  flag  = 1;

    do{
        c = b + a;
        a = b;
        b = c;
        phi1 = (double)(b) / (double)(a);
        flag = fabs(phi1 - phi0) > epsilon;
        phi0 = phi1;
    }while (flag);
    return phi1;

}

int main (int ac, char **av){
    double epsilon = 1E-8,
           *elapsedtime = NULL,
           pi, phi,
           et;
    unsigned int nThreads = 2;
    printf("\n\tExemplo: %s de uso de diretiva do OpenMP\n", __FILE__);
    printf("\tErro absoluto menor que: %e\n", epsilon);
    omp_set_num_threads(nThreads);
    posix_memalign((void**)(&elapsedtime), ALING, nThreads * sizeof(double));
#pragma omp parallel shared(elapsedtime, pi, phi, epsilon) private(et)
{
    et  = omp_get_wtime();
    #pragma omp sections nowait
    {
        #pragma omp section
        {
            pi = getPI(epsilon);
            printf("\t\t            Pi encontrado  %.20lf\n", pi);
            fflush(stdout);
        }

        #pragma omp section
        {
            phi = getGoldenRation(epsilon);
            
            printf("\t\tNúmero de ouro encontrado   %.20lf\n", phi);
            fflush(stdout);
        }
    }
    elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
}


    for (unsigned int i = 0; i < nThreads; i++){
        printf("\tTempo gasto pela thread %u foi de %.10lf em segundos\n", i, elapsedtime[i]);
    }
    
    return EXIT_SUCCESS;
}
