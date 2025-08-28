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
void add_based_on_reduction(void){
    unsigned int    size     = 10,
                    *vet     = NULL,
                    global_s = 0;
    assert(posix_memalign(reinterpret_cast <void**>(&vet), ALING, size * sizeof(unsigned int)) == 0);
    for (unsigned int i = 0; i < size; i++)
        vet[i] = i + 1;

    omp_set_num_threads(3); //Fixo o número de threads

    #pragma omp parallel for shared(size, vet) reduction(+:global_s)
    for (unsigned int i = 0; i < size; i++)
        global_s += vet[i];


    free(vet);
    cout << "\tValor somado: " << global_s  << endl;
}

void add_based_on_lock(void){
    unsigned int    size     = 10,
                    *vet     = NULL,
                    local_s  = 0,
                    global_s = 0;

    omp_lock_t lock2W;
    assert(posix_memalign(reinterpret_cast <void**>(&vet), ALING, size * sizeof(unsigned int)) == 0);
    for (unsigned int i = 0; i < size; i++)
        vet[i] = i + 1;


    omp_set_num_threads(6); //Fixo o número de threads
    omp_init_lock(&lock2W);

    #pragma omp parallel private(local_s) shared(size, vet, global_s)
    {
        local_s = 0;
        #pragma omp for
        for (unsigned int i = 0; i < size; i++){
            local_s += vet[i];
        }

        omp_set_lock(&lock2W);
            //cout << omp_get_thread_num() << " -> " << local_s << endl;
            global_s += local_s;
        omp_unset_lock(&lock2W);
    }



    free(vet);
    cout << "\tValor somado: " << global_s  << endl;
}


int main (int ac, char **av){

    cout << "Exemplos de soma" << endl;
    cout << "Exemplo 1: diretiva [reduction]" << endl;
    add_based_on_reduction();
    cout << "Exemplo 2: diretiva [lock]" << endl;
    add_based_on_lock();
    return EXIT_SUCCESS;
}

