#include <cassert>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <chrono>
#include <climits>
#include <omp.h>
#include <unistd.h>
#include <random>
using namespace std;
#define ALING 64

int main (int ac, char **av){
    unsigned long size = strtoul(av[1], NULL, 0);
    double *vet  = NULL;
    unsigned int nThreads = stoi(av[2]);
    //std::mt19937_64  generator (time(nullptr)); //64 bits
    std::mt19937_64  generator (42); //64 bits
    uniform_real_distribution<double> unif(0, 1); //uniform distribuition

    cout << endl << "\tRedução" << endl;
    size *= 1048576 / sizeof(double);
    cout << "               Tamanho:" << (size * sizeof(double)) / 1048576 << " em Mbytes " << endl;
    cout << "              Posições:" << size << endl;

    assert(posix_memalign(reinterpret_cast <void**>(&vet), ALING, size * sizeof(double)) == 0);


    #pragma omp parallel for
    for (unsigned long i = 1; i < size; i++){
        vet[i] = unif(generator) * 100.0f;
    }



    double sum = 0.0f;
    #pragma omp parallel for shared(size, vet) reduction(+:sum)
    for (unsigned long i = 0; i < size; i++){
        sum += vet[i];
    }

    cout << "Valor somado: " << sum  << endl;


    free(vet);

    return EXIT_SUCCESS;
}

