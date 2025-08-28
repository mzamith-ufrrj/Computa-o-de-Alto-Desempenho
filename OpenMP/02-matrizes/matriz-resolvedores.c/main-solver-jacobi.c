#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
/**
 * \brief Dispara cronometro.
 */
#define START_STOPWATCH( prm ) {                                        \
   gettimeofday( &prm.mStartTime, 0);                                   \
}

/**
  * \brief Para cronometro.
  *        retorna em segundos
  *        para retornar em ms basta remover a linha 42 :)
  */
#define STOP_STOPWATCH( prm ) {                                                        \
  gettimeofday( &prm.mEndTime, 0);                                                     \
  prm.mElapsedTime = (1000.0f * ( prm.mEndTime.tv_sec - prm.mStartTime.tv_sec) + (0.001f * (prm.mEndTime.tv_usec - prm.mStartTime.tv_usec)) );  \
  prm.mElapsedTime /= 1000.0f;                                                         \
}
#define EPSILON 1E-30
#define ALING 64
/**
 * \brief Estrutura para tratar cronometro.
 */
typedef struct
{
  struct timeval mStartTime;
  struct timeval mEndTime;
  double mElapsedTime;
} Stopwatch;

typedef struct {
    int n, m;
    double *vetor;
}Matrix;

/**
 *
 * Função para gravar vetores e matrizes no formato binário.
 * @param *A estrutura de matriz para ser gravada em disco.
 * @param *filename nome do arquivo binário.
 * @return Quantidade de bytes lidos;
 *
 * */
size_t saveBinary(Matrix *A, char * filename);

/**
 *
 * Função para recuperar vetores e matrizes no formato binário.
 * @param *A estrutura de matriz usada para armazenar a matriz ou vetor.
 * @param *filename nome do arquivo binário.
 * @return Quantidade de bytes lidos;
 *
 * */
size_t loadBinary(Matrix *A, char *filename);

/**
 * Função para exibir os parâmetros e a sua ordem.
 *  */
void help(void);

/**
 *
 * Multiplica duas matrizes;.
 * @param *elapsedtime Vetor para armazenar o tempo de execução de cada thread. Apenas para fins didáticos.
 * @param *X Vetor com a solução do sistema linear.
 * @param *A Matriz A.
 * @param *B Vetor B.
 * @param error Error relativo permitido. Define a quantidade de iterações.
 * @param nThreads quantidade de threadas. Apenas para fins didáticos.
 * @param show exibe o erro máximo obtido em cada iteração. Apenas para fins didáticos.
 *
 * */
void jacobiSolverMethod(double* elapsedtime,
                        double** __restrict__ X,
                        Matrix* __restrict__ A,
                        Matrix* __restrict__ B,
                        const double error,
                        const unsigned int nThreads,
                        int show){

   double aux,
         div,
         err = 0.0,
         *x0 = NULL,
         *x1 = NULL;
   unsigned int inter = 0;
   int flag = 0;

   x0 = (double *) aligned_alloc(ALING,  B->m * sizeof(double));
   x1 = (double *) aligned_alloc(ALING,  B->m * sizeof(double));

   bzero(x0, B->m * sizeof(double));
   bzero(x1, B->m * sizeof(double));
   omp_set_num_threads(nThreads);
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

                    #pragma omp single
                    {//#master
                        double *swap = x0;
                        x0 = x1;
                        x1 = swap;
                        inter++;
                        if (show){
                            printf("\t     Error: %.3E | passo: %.5u | thread: %d\n", err, inter, omp_get_thread_num());
                            fflush(stdout);
                        }
                        flag = (err > error);
                        err = 0.0f;
                    }
                    #pragma omp barrier

            }while (flag);

            elapsedtime[omp_get_thread_num()] = omp_get_wtime() - et;
    }//seção paralela
    *X = x0;
    printf("----------------------------------------------------------------\n");
    printf("\t     Error: %.3E | passos %u\n", error, inter);
    printf("----------------------------------------------------------------\n");
    free(x1);

}
int main (int ac, char **av){

    Matrix A, B, X, X1;
    Stopwatch stopwatch;
    START_STOPWATCH(stopwatch);
    if (ac != 8){
        help();
        return EXIT_FAILURE;
    }

    double error = atof(av[1]);
    int    showErr = atoi(av[2]);

    char  *filename_A = av[3],
          *filename_B = av[4],
          *filename_X = av[5],
          *fileLog = av[7];

    unsigned int nThreads = atoi(av[6]);
    size_t            mem = 0;
    double *elapsedtime = NULL;


    mem =  loadBinary(&A, filename_A);
    mem += loadBinary(&B, filename_B);
    elapsedtime = (double *) aligned_alloc(ALING, nThreads * sizeof(double));

    printf("\nMétodo de Jacobi para solução de sistema linear\n");
    printf("\t     Error: %.3E \n", error);
    printf("\t  Matriz A: %s \n", filename_A);
    printf("\t   Vetor B: %s \n", filename_B);
    printf("\t   Threads: %u \n", nThreads);

    X1.m = B.n;
    X1.n = B.m;

    jacobiSolverMethod(elapsedtime, &X1.vetor, &A, &B, error, nThreads, showErr);
    saveBinary(&X1, filename_X);



    //cout << endl;
    for (unsigned int i = 0; i < nThreads; i++){
        printf(" Tempo gasto pela thread [%.3u] foi de %15.8lf segundos\n", i, elapsedtime[i]);
    }
    //    cout << "\tTempo gasto pela thread " << i << " foi de " << setw(10) <<  setprecision(4) << fixed << elapsedtime[i] << " em segundos" << endl;
    //}
    /*
    cout << "Solução aproximada: " << endl;
    int width = 10;
    int precision = 5;
    for (int i = 0; i < X1.n; i++){
        cout << setw(width) << fixed << (i+1) << "|" << setw(width) <<  setprecision(precision) << fixed << X1.vetor[i] << "\t"  << setw(width) <<  setprecision(precision) << fixed << X.vetor[i] << "|" << endl;;
    }
    */
    free(A.vetor);
    free(B.vetor);
    free(X1.vetor);


    STOP_STOPWATCH(stopwatch);
    printf("----------------------------------------------------------------\n");
    printf("\tTempo total de execução: %lf\n", stopwatch.mElapsedTime);
    printf("----------------------------------------------------------------\n");
    return EXIT_SUCCESS;
}

size_t saveBinary(Matrix *A, char * filename){
    FILE *ptr = fopen(filename, "wb");
    size_t bytes_written = 0, aux;
    assert(ptr != NULL);
    aux = fwrite(&A->m, sizeof(A->m), 1, ptr); bytes_written += aux * sizeof(A->m);
    aux = fwrite(&A->n, sizeof(A->n), 1, ptr); bytes_written += aux * sizeof(A->n);
    aux = fwrite(A->vetor, sizeof(double), A->m * A->n, ptr); bytes_written += aux * sizeof(double);

    fclose(ptr);

}

size_t loadBinary(Matrix *A, char *filename){
    FILE *ptr = fopen(filename, "rb");
    assert(ptr != NULL);
    double *a = NULL;
    size_t bytes_read = 0, aux;
    //numread = fread( list, sizeof( char ), 25, stream );

    aux = fread(&A->m, sizeof(A->m), 1, ptr); bytes_read += aux * sizeof(A->m);
    aux = fread(&A->n, sizeof(A->n), 1, ptr); bytes_read += aux * sizeof(A->n);

    a = (double *) aligned_alloc(ALING,  A->m *  A->n * sizeof(double));
    assert(a != NULL);

    aux = fread(a, sizeof(double), A->m * A->n, ptr); bytes_read += aux * sizeof(double);

    A->vetor = a;
    fclose(ptr);
    return bytes_read;
    //print2Console(A);
}

void help(void){
    printf("\n\nExemplo de execução do programa:\n");
    printf("./solver-jacobi.exec 1E-10 1 A.bin B.bin Xout.bin 6  log.csv\n");
    printf("                       |   |   |    |     |       |    |-> Arquivo de log \n");
    printf("                       |   |   |    |     |       |------> Qtde threads \n");
    printf("                       |   |   |    |     |--------------> arquivo de saída \n");
    printf("                       |   |   |    |--------------------> arquivo com vetor B \n");
    printf("                       |   |   |-------------------------> arquivo com matriz A \n");
    printf("                       |   |-----------------------------> Exibe erro máximo a cada passo \n");
    printf("                       |---------------------------------> Critério de parada ER \n");
}
