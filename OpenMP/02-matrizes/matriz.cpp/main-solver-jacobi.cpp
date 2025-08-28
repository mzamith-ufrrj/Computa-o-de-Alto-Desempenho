#include <iostream>
#include <iomanip>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <cstdio>
#include <omp.h>
#include <chrono>
#include <rural-mat.hpp>
#define EPSILON 1E-30

using namespace std;
void help (void){
    cout << "Programa para gerar matrizes aleatórias e vetores X e B para solução linear" << endl;
    cout << "Gerar matriz quadrada apenas, senão não há solução" << endl;
    cout << "Exemplo: " << endl;

    cout << "\t./generate-mat-cpu.exec 10 " << endl;
    cout << "\t                         |-------------> respectivamente colunas e linhas da matriz"  << endl;
}


void saveLog(string logFile,
             unsigned int mem,
             double totalTime,
             double threadsTime,
             unsigned int nThreads){

    fstream logout;
    FILE *ptr = NULL;
    struct stat buffer;
    int ret = stat(logFile.c_str(), &buffer);
    if (ret == -1){
        logout.open(logFile, fstream::trunc|fstream::out);

        assert(logout.is_open());
        logout << "memoria;threads;tempo_medio_threads;tempo_total" << endl;

    }else{
        logout.open(logFile, fstream::app | fstream::out);
    }
    logout << mem << ";";
    logout << nThreads << ";";
    logout << threadsTime << ";";
    logout << totalTime << endl;

    logout.close();
    cout << "\tLog file[" << logFile << "] saved" << endl;

}


void jacobiSolverMethod(double *elapsedtime, double **X, stMatrix *A, stMatrix *B, const double error, bool show){

   double aux,
         div,
         err = 0.0,
         *x0 = NULL,
         *x1 = NULL;
   unsigned int inter = 0;
   bool flag = false;

   assert(posix_memalign(reinterpret_cast <void**>(&x0), ALING, B->n * sizeof(double)) == 0);
   assert(posix_memalign(reinterpret_cast <void**>(&x1), ALING, B->n * sizeof(double)) == 0);
   bzero(x0, B->n * sizeof(double));
   bzero(x1, B->n * sizeof(double));
    #pragma omp parallel private(aux, div) shared (elapsedtime, x0, x1, A, B, error, err, show, flag)
    {
            double et  = omp_get_wtime();
            do{
                    #pragma omp for schedule (dynamic)
                    for (unsigned int j = 0; j < A->m; j++){ //M equações
                        aux    = 0.0;
                        div    = 0.0;

                        for (unsigned int i = 0; i < A->n; i++){ // n incógnitas
                        if (j != i)
                            aux += (A->vetor[A->n * j + i] * x0[i]);
                        else
                            div =  A->vetor[A->n * j + i];
                        }
                        x1[j] = (B->vetor[j] - aux) / div ;
                    }
                    #pragma omp barrier


                    #pragma omp for schedule (dynamic) reduction(max:err)
                    for (unsigned int j = 0; j < A->n; j++){
                        double b = fabs((x1[j] - x0[j]) / (x1[j] + EPSILON));
                        if (b > err)
                            err = b;
                        //err = max(err, b);
                    }

                    #pragma omp master
                    {
                        double *swap = x0;
                        x0 = x1;
                        x1 = swap;
                        inter++;
                        if (show)
                            cout << "Error: " << setprecision(20) << err << " \t Passos: " << inter << endl;

                        flag = (err > error);
                        err = 0.0f;
                    }
                    #pragma omp barrier

            }while (flag);

            elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
    }//seção paralela
   *X = x0;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Error: " << err << " \t Passos: " << inter << endl;
    cout << "----------------------------------------------------------------" << endl;
    free(x1);

}
int main (int ac, char **av){

    stMatrix A, B, X, X1;
       /*
    if (ac != 5){
        help();
        return EXIT_SUCCESS;
    }
    */

    double error = stof(av[1]);
    bool   showErr = stoi(av[2]);

    string filename_A = av[3],
           filename_B = av[4],
           filename_X = av[5],
           logFile    = av[6];



    unsigned int nThreads = stoi(av[7]);
    unsigned long mem = 0;
    double *elapsedtime = NULL;

    auto start_time = std::chrono::high_resolution_clock::now();

    loadBinary(&A, filename_A);
    loadBinary(&B, filename_B);
    assert(posix_memalign(reinterpret_cast <void**>(&elapsedtime), ALING, nThreads * sizeof(double)) == 0);

    //loadBinary(&X, filename_X);

    cout << "Método de Jacobi para solução de sistema linear" << endl;
    cout << "\t     Error: " << scientific << error << endl;
    cout << "\t  Matriz A: " << filename_A << endl;
    cout << "\t   Vetor B: " << filename_B << endl;
   // cout << "\t Solução X: " << filename_B << endl;
    cout << endl;

    /*
    for (int j = 0; j < A.n; j++){
        cout  <<  " ";

        for (int i = 0; i < A.m; i++){
            int k = j * A.m +  i;
            cout << setw(10)  <<  setprecision(3) << fixed << A.vetor[k] << " ";
        }//end-for (int j = 0; j < mLattice->height; j++){
        cout << "|" << setw(10) <<  setprecision(3) << fixed << B.vetor[j];
        cout << endl;
    }//end-void InitRandness(tpLattice *mLattice, float p){

    cout << endl;
    for (int i = 0; i < X.m; i++){
        cout << setw(10) <<  setprecision(3) << fixed << X.vetor[i];
    }
    cout << endl << endl;
    */

    X1.m = B.m;
    X1.n = B.n;
    mem = (A.m * A.n + B.m * B.n + X1.m * X1.n) * sizeof(double);
    omp_set_num_threads(nThreads);
    jacobiSolverMethod(elapsedtime, &X1.vetor, &A, &B, error, showErr);
    print2Binary(&X1, filename_X);
    /*
    cout << endl;
    for (unsigned int i = 0; i < nThreads; i++){
        cout << "\tTempo gasto pela thread " << i << " foi de " << setw(10) <<  setprecision(4) << fixed << elapsedtime[i] << " em segundos" << endl;
    }
    */
    /*
    cout << "Solução aproximada: " << endl;
    int width = 10;
    int precision = 5;
    for (int i = 0; i < X1.n; i++){
        cout << setw(width) << fixed << (i+1) << "|" << setw(width) <<  setprecision(precision) << fixed << X1.vetor[i] << "\t"  << setw(width) <<  setprecision(precision) << fixed << X.vetor[i] << "|" << endl;;
    }
    */

    double avElapsedTime = 0.0f;
    for (unsigned int i = 0; i < nThreads; i++) avElapsedTime += elapsedtime[i];
    avElapsedTime /= static_cast <double> (nThreads);
    free(A.vetor);
    free(B.vetor);
    free(X1.vetor);
    free(elapsedtime);
    auto stop_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = stop_time - start_time;
    cout << "----------------------------------------------------------------" << endl;
    cout << "\tTempo total de execução: " << total_time.count() << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << endl;

    saveLog(logFile, mem, total_time.count(), avElapsedTime, nThreads);
    return EXIT_SUCCESS;
}
