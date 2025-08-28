#include <iostream>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <omp.h>
#include <rural-mat.hpp>
using namespace std;
/**
 *
 * Multiplica duas matrizes;.
 * @param *C Matriz com o resultado A * B.
 * @param *A Matriz A.
 * @param *B Matriz B.
 * @param *elapsedtime Vetor para armazenar o tempo de execução de cada thread. Apenas para fins didáticos.
 * @param nThreads quantidade de threadas. Apenas para fins didáticos.
 * @return Quantidade de bytes lidos;
 *
 * */
void matrix_multi(stMatrix *  __restrict__ C,
                  stMatrix *  __restrict__ A,
                  stMatrix *  __restrict__ B,
                  double * __restrict__ elapsedtime,
                  unsigned int nThreads){
    double et;

    omp_set_num_threads(nThreads);
    #pragma omp parallel private(et) shared(A, B, C, elapsedtime)
    {
        et  = omp_get_wtime();

        #pragma omp for nowait schedule (dynamic)
        for (int j = 0; j < C->n; j++){
            for (int i = 0; i < C->m; i++){
                double c = 0.0f;
                for (int jA = 0; jA < A->m; jA++){
                    int ak = j * A->m + jA;
                    int bk = jA * B->m + i;
                    c += A->vetor[ak] * B->vetor[bk];
                }//for (int jA = j; jA < A->m; jA++){

                int ck = j * C->m +  i;
                C->vetor[ck] = c;
            }//end-for (int j = 0; j < mLattice->height; j++){

        }//end-void InitRandness(tpLattice *mLattice, float p){
        elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
    }
}

/**
 *
 * Salva log com o resultado da computação.
 * @param logFile nome do arquivo de log. Exemplo: log.csv.
 * @param mem quantidade de memória utilizada em Mbytes.
 * @param totalTime tempo total em segundos.
 * @param threadsTime tempo das threads em segundos.
 * @param nThreads quantidade de threadas.
 *
 * */
void saveLog(string logFile,
             unsigned int mem,
             double totalTime,
             double *threadsTime,
             unsigned int nThreads);
void help (void);
int main (int ac, char **av){
    unsigned long mem = 0;
    stMatrix      A,
                  B,
                  C;

    if (ac != 7){
        help();
        return EXIT_SUCCESS;
    }
    string filename_matrix_A = av[1],
           filename_matrix_B = av[2],
           filename_matrix_C = av[3],
           filename_log      = av[4];
    bool show             = stoi(av[6]);
    unsigned int nThreads = stoi(av[5]);
    double *elapsedtime = NULL;

    auto start_time = std::chrono::high_resolution_clock::now();
    cout << endl << "Multiplicação de matrizes" << endl;
    cout << "Matrizes:" << endl;
    cout << "\t A - arquivo: " << filename_matrix_A << endl;
    cout << "\t B - arquivo: " << filename_matrix_B << endl;
    cout << "\t C - arquivo: " << filename_matrix_C << endl;
    cout << "Threads: " << nThreads << endl;

    loadBinary(&A, filename_matrix_A);
    loadBinary(&B, filename_matrix_B);

    C.m = A.n;
    C.n = B.m;

    assert(posix_memalign(reinterpret_cast <void**>(&C.vetor), ALING, C.m *  C.n * sizeof(double)) == 0);

    assert(posix_memalign(reinterpret_cast <void**>(&elapsedtime), ALING, nThreads * sizeof(double)) == 0);

    memset(C.vetor, 0x00,  C.m *  C.n * sizeof(double));
    mem =  (A.m *  A.n + B.m *  B.n + C.m *  C.n) * sizeof(double);
    cout << "Total de memória usada: " << (mem / 1048576) << " Mbytes" << endl;

    matrix_multi(&C, &A, &B, elapsedtime, nThreads);
    if (show){
        cout << "Matrizes" << endl;
        cout << "A:" << endl;
        print2Console(&A);
        cout << "B:" << endl;
        print2Console(&B);
        cout << "C:" << endl;
        print2Console(&C);

    }


    for (unsigned int i = 0; i < nThreads; i++){
        cout << "\tTempo gasto pela thread " << i << " foi de " << elapsedtime[i] << " em segundos" << endl;
    }

    print2Binary(&C, filename_matrix_C);
    free(A.vetor);
    free(B.vetor);
    free(C.vetor);


    auto stop_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = stop_time - start_time;
    cout << "\tTempo total de execução: " << total_time.count() << endl;

    saveLog(filename_log, mem, total_time.count(), elapsedtime, nThreads);
    free(elapsedtime);
    //chrono::hours, chrono::minutes, chrono::seconds, chrono::milliseconds, or chrono::microseconds.
    //auto elapsed = chrono::duration_cast<chrono::microseconds>(stop - start);
    //cout << elapsed.count() << endl;
    //saveLog(filename, mLattice.x, mLattice.y, mLattice.t, elapsed.count(), processorElapsedTime);
    return EXIT_SUCCESS;
}

/**
 *
 * Salva log com o resultado da computação.
 * @param logFile nome do arquivo de log. Exemplo: log.csv.
 * @param mem quantidade de memória utilizada em Mbytes.
 * @param totalTime tempo total em segundos.
 * @param threadsTime tempo das threads em segundos.
 * @param nThreads quantidade de threadas.
 *
 * */
void saveLog(string logFile,
             unsigned int mem,
             double totalTime,
             double *threadsTime,
             unsigned int nThreads){

    fstream logout;
    FILE *ptr = NULL;
    struct stat buffer;
    int ret = stat(logFile.c_str(), &buffer);
    if (ret == -1){
        logout.open(logFile, fstream::trunc|fstream::out);
        //ptr = fopen(logFile.c_str(), "w+");
        assert(logout.is_open());
        logout << "memoria;";
        logout << "tempo_threads_media;" ;
        logout << "threads;";
        logout << "tempo_total" << endl;
    }else{
        logout.open(logFile, fstream::app | fstream::out);
    }
    double tave = 0.0f;
    for (unsigned int i = 0; i < nThreads; i++){
        tave += threadsTime[i];
    }
    tave /= static_cast<double>(nThreads);
    logout << mem << ";";
    logout << tave << ";";
    logout << nThreads << ";";
    logout << totalTime << endl;

    logout.close();
    cout << "\tLog file[" << logFile << "] saved" << endl;

}

void help (void){
    cout << "Programa para multiplicar 2 matrizes" << endl;
    cout << "Exemplo: " << endl;

    cout << "\t./multi-mat-cpu.exec r3x5.bin r5x3.bin C.bin log.csv 1 0" << endl;
    cout << "\t                        |       |        |      |    | |-----> 0/1 exibe ou não a matriz " << endl;
    cout << "\t                        |       |        |      |    |-------> qtde de threads alocadas" << endl;
    cout << "\t                        |       |        |      |------------> arquivo com o log do tempo de execução" << endl;
    cout << "\t                        |-------|--------|-------------------> arquivo binário com as matrizes A, B e C" << endl;
    cout << "\t                        |-------|--------|-------------------> C = A x B" << endl;
}
