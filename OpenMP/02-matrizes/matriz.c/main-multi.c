#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
//#include <omp.h>
#include <assert.h>
#define STRING_SIZE 256
#define ALIGN       64
typedef struct {
    unsigned int n, m;
    double *v;
} tpMatrix;



void loadBinary(tpMatrix *A, char * filename);
void saveBinary(tpMatrix *A, char * filename);
void matrix_multi(tpMatrix *  C,
                  tpMatrix *  A,
                  tpMatrix *  B);

int main (int ac, char **av){
    unsigned long mem = 0;

    tpMatrix     A,
                 B,
                 C;

    char filename_matrix_A[256],
         filename_matrix_B[256],
         filename_matrix_C[256];

    strcpy(filename_matrix_A, av[1]);
    strcpy(filename_matrix_B, av[2]);
    strcpy(filename_matrix_C, av[3]);


    printf("Multiplicação de matrizes\n\n");
    printf(" - Matriz A: [%s]\n", filename_matrix_A);
    printf(" - Matriz B: [%s]\n", filename_matrix_B);
    printf(" - Matriz C: [%s]\n", filename_matrix_C);

    loadBinary(&A, filename_matrix_A);
    loadBinary(&B, filename_matrix_B);
    C.m = A.n;
    C.n = B.m;
    C.v = (double*) malloc(sizeof(double) * C.m * C.n);
    memset(C.v, 0x00,  C.m *  C.n * sizeof(double));
    matrix_multi(&C, &A, &B);
    saveBinary(&C, filename_matrix_C);
    free(A.v);
    free(B.v);
    free(C.v);

    return EXIT_SUCCESS;
}

void loadBinary(tpMatrix *A, char * filename){
    FILE *input = fopen(filename, "rb");
    size_t bytesRead = 0;
    double *v = NULL;
    assert(input != NULL);

    bytesRead = fread(&A->m, sizeof(unsigned int), 1, input);
    bytesRead += fread(&A->n, sizeof(unsigned int), 1, input);
    bytesRead *= sizeof(unsigned int);

    posix_memalign((void**)&v, ALIGN, sizeof(double) * A->m * A->n);
    //A->v = (double*) malloc(sizeof(double) * A->m * A->n);
    bytesRead += fread(v, sizeof(double), A->m * A->n, input) * sizeof(double);
    printf("\t loadBinary - bytes lidos [%u]\n", bytesRead);
    fclose(input);
    A->v = v;

}

void saveBinary(tpMatrix *A, char * filename){
    FILE *output = fopen(filename, "wb+");
    double *v  = A->v;
    size_t bytesWrite = 0;
    assert(output != NULL);

    bytesWrite = fwrite(&A->m, sizeof(unsigned int), 1, output);
    bytesWrite += fwrite(&A->n, sizeof(unsigned int), 1, output);
    bytesWrite *= sizeof(unsigned int);
    bytesWrite += fwrite(v, sizeof(double), A->m * A->n, output) * sizeof(double);
    printf("\t saveBinary - bytes escritos [%u]\n", bytesWrite);
    fclose(output);

}

void matrix_multi(tpMatrix *  C,
                  tpMatrix *  A,
                  tpMatrix *  B){

        //#pragma omp parallel for
        for (unsigned int j = 0; j < C->n; j++){
            for (unsigned int i = 0; i < C->m; i++){
                double c = 0.0f;
                for (unsigned int jA = 0; jA < A->m; jA++){
                    unsigned int ak = j * A->m + jA;
                    unsigned int bk = jA * B->m + i;
                    c += A->v[ak] * B->v[bk];
                }//for (int jA = j; jA < A->m; jA++){

                unsigned int ck = j * C->m +  i;
                C->v[ck] = c;
            }//end-for (int j = 0; j < mLattice->height; j++){

        }//end-void InitRandness(tpLattice *mLattice, float p){

}
