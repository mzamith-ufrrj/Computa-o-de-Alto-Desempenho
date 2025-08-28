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
 * @param nThreads quantidade de threadas. Apenas para fins didáticos.
 *
 * */
void luSolverMethod (double* elapsedtime,
                     double** __restrict__ X,
                     Matrix* __restrict__ A,
                     Matrix* __restrict__ B,
                     const unsigned int nThreads){

    double *AB = (double *) aligned_alloc(ALING,  A->m * (A->n + 1) * sizeof(double)),
           *x  = (double *) aligned_alloc(ALING,  B->m * sizeof(double)),
           aij = 0.0f;
    int r = A->m,
        c = A->n + 1;
    assert((AB != NULL) && (X != NULL));

    //Matriz ampliada
    for (int j = 0; j < A->m; j++){
        for (int i = 0; i < A->n; i++){
            AB[j * c + i] = A->vetor[j * A->n + i];
        }
        AB[j  * c + A->n] = B->vetor[j];
    }
    //----------------------------------------------------------------------------
    for (int j = 0; j < r; j++){
        aij = AB[j * c + j];

        for (int i = j; i < c; i++){
            AB[j * c + i] = AB[j * c + i] / aij;
        }//end-for (int i = 0; i < c; i++){

        int l = j;

        for (int k = j+1; k < r; k++){
            aij = AB[k * c + l];

            for (int i = j; i < c; i++){
                AB[k * c + i] = AB[k * c + i] - aij * AB[l * c + i];
            }


        }//for (int k = j+1; k < r; k++){
    }//end-for (int j = 0; j < r; j++){
    //----------------------------------------------------------------------------
    for (int i = r-1; i >= 0; i--){
        double acc = 0.0;
        for (int k = i+1; k <  (c - 1); k++){
            acc += AB[i * c + k] * x[k];
        }
        x[i] = (AB[i * c + i] * AB[i * c + (c - 1)]) - acc;
    }
    free(AB);
    *X = x;
}
//./solver-gauss.exec A1000.bin  B1000.bin x-gauss.bin 6 log.csv
int main (int ac, char **av){

    Matrix A, B, X, X1;
    Stopwatch stopwatch;
    START_STOPWATCH(stopwatch);

    if (ac != 6){
        help();
        return EXIT_FAILURE;
    }



    char  *filename_A = av[1],
          *filename_B = av[2],
          *filename_X = av[3],
          *fileLog = av[4];

    unsigned int nThreads = atoi(av[5]);
    size_t            mem = 0;
    double *elapsedtime = NULL;


    mem =  loadBinary(&A, filename_A);
    mem += loadBinary(&B, filename_B);
    elapsedtime = (double *) aligned_alloc(ALING, nThreads * sizeof(double));

    printf("\nMétodo de Jacobi para solução de sistema linear\n");
    printf("\t  Matriz A: %s \n", filename_A);
    printf("\t   Vetor B: %s \n", filename_B);
    printf("\t   Threads: %u \n", nThreads);

    X1.m = B.n;
    X1.n = B.m;

    luSolverMethod(elapsedtime, &X1.vetor, &A, &B, nThreads);
    saveBinary(&X1, filename_X);



    //cout << endl;
    for (unsigned int i = 0; i < nThreads; i++){
        printf(" Tempo gasto pela thread [%.3u] foi de %15.8lf segundos\n", i, elapsedtime[i]);
    }

    /*
    printf("\n\n");
    for (int i = 0; i < X1.m; i++){
        for (int j = 0; j < X1.n; j++){
            printf("%lf ", X1.vetor[i * X1.n + j]);
        }
        printf("\n");

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

    printf("./solver-gauss.exec A5000.bin  B5000.bin x-gauss.bin 6 log.csv\n");
    printf("                       |          |         |        |    |-> Arquivo de log \n");
    printf("                       |          |         |        |------> Qtde threads \n");
    printf("                       |          |         |---------------> arquivo de saída \n");
    printf("                       |          |-------------------------> arquivo com vetor B \n");
    printf("                       |------------------------------------> arquivo com matriz A \n");
}
