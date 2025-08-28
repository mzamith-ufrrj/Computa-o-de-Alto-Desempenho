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
void matrix_multi(Matrix *  __restrict__ C,
                  Matrix *  __restrict__ A,
                  Matrix *  __restrict__ B,
                  double * __restrict__ elapsedtime,
                  unsigned int nThreads){

     #pragma omp parallel 
     {
	#pragma omp for
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
      }
}
// ./multi-mat-cpu.exec r2000x2000.bin i2000x2000.bin resultado.bin 4
int main (int ac, char **av){

    Matrix A, B, C;
    Stopwatch stopwatch;
    START_STOPWATCH(stopwatch);


    char  *filename_A = av[1],
          *filename_B = av[2],
          *filename_C = av[3];

    unsigned int nThreads = atoi(av[4]);
    double mem = 0.0,
           *elapsedtime = NULL;


    mem  =  (double)loadBinary(&A, filename_A);
    mem  += (double) loadBinary(&B, filename_B);
    C.m = A.n;
    C.n = B.m;

    C.vetor = (double *) aligned_alloc(ALING, C.m *  C.n * sizeof(double));
    mem  += (double) C.m *  C.n * sizeof(double);
    bzero(C.vetor, C.m *  C.n * sizeof(double));


    elapsedtime = (double *) aligned_alloc(ALING, nThreads * sizeof(double));

    printf("\nMultiplicação de matrizes\n");
    printf("\t  Matriz A: %s \n", filename_A);
    printf("\t  Matriz B: %s \n", filename_B);
    printf("\t  Matriz C: %s \n", filename_C);
    printf("\t   Threads: %u \n", nThreads);
    printf("\t   Memória: %lf MBytes \n", (mem / 1048576));

    matrix_multi(&C, &A, &B, elapsedtime, nThreads);
    saveBinary(&C, filename_C);

    printf("----------------------------------------------------------------\n");
    for (unsigned int i = 0; i < nThreads; i++){
        printf(" Tempo gasto pela thread [%.3u] foi de %15.8lf segundos\n", i, elapsedtime[i]);
    }

    free(A.vetor);
    free(B.vetor);
    free(C.vetor);


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
