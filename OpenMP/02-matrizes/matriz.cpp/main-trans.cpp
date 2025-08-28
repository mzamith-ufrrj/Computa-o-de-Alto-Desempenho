#include <iostream>
#include <iomanip>
#include <cassert>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <omp.h>
#include <rural-mat.hpp>
#include <chrono>

using namespace std;
void matrix_trans(stMatrix *B, stMatrix *A, unsigned int threads, double **ptr){
    double et = 0.0f;
    double *elapsedtime = NULL;

    assert(posix_memalign(reinterpret_cast <void**>(&elapsedtime), ALING, threads * sizeof(double)) == 0);

    omp_set_num_threads(threads);
    #pragma omp parallel private(et) shared(A, B, elapsedtime)
    {
         et  = omp_get_wtime();
        //dinamic, static guided, chunck <- passos do for opcional
        #pragma omp for nowait schedule (dynamic)
        for (int j = 0; j < A->m; j++){
            for (int i = 0; i < A->n; i++){

                int p = j * A->n + i;
                int q = i * B->n + j;
                B->vetor[q] = A->vetor[p];
            }//end-for (int j = 0; j < mLattice->height; j++){
        }//end-void InitRandness(tpLattice *mLattice, float p){
        elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
    }

    if (ptr == NULL)
        free(elapsedtime);
    else
        *ptr = elapsedtime;
}


void help (void);
int main (int ac, char **av){
    unsigned long mem = 0;
    double *elapsedtime = NULL;
    stMatrix A,
             B;
    if (ac != 5){
        help();
        return EXIT_SUCCESS;
    }
    string filename = av[1];
    string filename_transp = av[2];
    bool show = stoi(av[3]);
    unsigned int nThreads = stoi(av[4]);
    auto start_time = std::chrono::high_resolution_clock::now();
    cout << endl << "Matriz transposta" << endl;
    loadBinary(&A, filename);

    B.m = A.n;
    B.n = A.m;
    assert(posix_memalign(reinterpret_cast <void**>(&B.vetor), ALING, B.m *  B.n * sizeof(double)) == 0);
    bzero(B.vetor, B.m * B.n * sizeof(double));
    mem =  (A.m *  A.n + B.m *  B.n) * sizeof(double);
    cout << "Total de memória usada: " << (mem / 1048576) << " Mbytes" << endl;


    matrix_trans(&B, &A, nThreads, &elapsedtime);
    print2Binary(&B, filename_transp);

    cout << endl;
    if (show){
        for (int j = 0; j < B.n; j++){

            for (int i = 0; i < B.m; i++){
                int k = j * B.m + i;
                cout << setw(10) << fixed << setprecision(6) << B.vetor[k] << " ";
                //cout << setprecision(4) << A.MAT[k] << " ";
            }
            cout << endl;
        }
    }
    for (unsigned int i = 0; i < nThreads; i++){
        cout << "\tTempo gasto pela thread " << i << " foi de " << elapsedtime[i] << " em segundos" << endl;
    }


    free(B.vetor);
    free(A.vetor);
    free(elapsedtime);
    auto stop_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = stop_time - start_time;
    cout << "\tTempo total de execução: " << total_time.count() << endl;

    return EXIT_SUCCESS;
}
void help (void){
    cout << "Programa exibir uma matriz" << endl;
    cout << "Exemplo: " << endl;

    cout << "\t./trans-mat.exec r4x5.bin saida.bin 0 1" << endl;
    cout << "\t                 |          |       | |--> Threads" << endl;
    cout << "\t                 |          |       |----> 0/1 exibe ou não a matriz de saída" << endl;
    cout << "\t                 |          |------------> Arquivo de saídas com a matriz transposta " << endl;
    cout << "\t                 |-----------------------> Arquivo com a matriz origianl " << endl;

}
