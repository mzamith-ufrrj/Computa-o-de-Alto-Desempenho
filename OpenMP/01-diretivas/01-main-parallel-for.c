#include <assert.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>
#include <unistd.h>
#define ALING 64
/*
 * Neste exemplo são passados o tamanho do vetor em MB e a quantidade de threads.
 * Observa-se o uso do private(x). A sua falta torna x shared e há uma perda de
 * performance.
 */
int main (int ac, char **av){
    unsigned long size = strtoul(av[1], NULL, 0);
    double        dSize  = 0.0f;
    unsigned int nThreads = stoi(av[2]);


    size *= 1048576 / sizeof(unsigned long);
    dSize = static_cast<double> (size);

    //1024 * 1024 = 1048576 = 1 MB
    cout << "\n\tExemplo: " << __FILE__ << " de uso de diretiva do OpenMP" << endl;
    cout << "\tMemória alocada: " << size / (1048576) << " MBytes alocados" << endl;
    cout << "\tThreads: " << nThreads << endl;
    double dx = 0.0,
           x  = -1.0f,
           *y = NULL,
           *elapsedtime = NULL,
           et;

    posix_memalign(reinterpret_cast <void**>(&y), ALING, size * sizeof(unsigned long));
    omp_set_num_threads(nThreads);

    posix_memalign(reinterpret_cast <void**>(&elapsedtime), ALING, nThreads * sizeof(double));

#pragma omp parallel private(x, et) firstprivate(dx) shared(size, dSize, elapsedtime)
{
    et  = omp_get_wtime();
    #pragma omp master
    {
        cout << "\tIniciando os trabalhos..." << endl;
        cout.flush();
        //usleep(10000000); //Coloca em espera oculpada
    }
    dx = 1.0f / dSize;

    #pragma omp for nowait
    for (unsigned long i = 0; i < size; i++){
        x = static_cast<double>(i) * dx;
        y[i] = exp(x) * cos(x) * sqrt(6*x+5);
    }

    elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
}
    cout << "\t\t  Y(0) = " << setprecision(20) << fixed << y[0] << endl;
    cout << "\t\tY(n-1) = " << y[size-1] << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << endl << "\tTempos: " << setprecision(4) << fixed << endl << endl;
    for (unsigned int i = 0; i < nThreads; i++){
        cout << "\tTempo gasto pela thread " << i << " foi de " << elapsedtime[i] << " em segundos" << endl;
    }
    free(y);
    free(elapsedtime);
    return EXIT_SUCCESS;
}
