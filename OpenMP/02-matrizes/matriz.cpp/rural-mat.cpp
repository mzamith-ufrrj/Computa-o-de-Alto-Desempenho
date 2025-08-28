#include <iostream>
#include <cassert>
#include <cstring>
#include <ctime>
#include <random>
#include <chrono>
#include <rural-mat.hpp>
#include <cassert>
#include <fstream>
#include <omp.h>
using namespace std;
// m equações
// n incógnitas

/*
 * Show the last CA state to console.
 */
void print2Console(stMatrix *mat){
    cout << endl;
    cout << "Matriz: " << endl;

    for (int j = 0; j < mat->m; j++){
        cout << "\t";
        for (int i = 0; i < mat->n; i++){
            int k = j * mat->n +  i;
            cout << mat->vetor[k] << " ";
        }//end-for (int j = 0; j < mLattice->height; j++){
        cout << endl;
    }//end-void InitRandness(tpLattice *mLattice, float p){


}

void print2Binary(stMatrix *A, std::string filename){
    fstream output;
    output.open(filename, fstream::out | fstream::binary);
    assert(output.is_open());
    output.write(reinterpret_cast<char*>(&A->m), sizeof(A->m));
    output.write(reinterpret_cast<char*>(&A->n), sizeof(A->n));
    output.write(reinterpret_cast<char*>(A->vetor), sizeof(double) * A->m * A->n);

    output.close();
}

void loadBinary(stMatrix *A, std::string filename){
    fstream input;
    input.open(filename, fstream::in | fstream::binary);
    assert(input.is_open());
    double *a = NULL;
    input.read(reinterpret_cast<char*>(&A->m), sizeof(A->m));
    input.read(reinterpret_cast<char*>(&A->n), sizeof(A->n));
    posix_memalign(reinterpret_cast <void**>(&a), ALING, A->m *  A->n * sizeof(double));

    input.read(reinterpret_cast<char*>(a), sizeof(double) * A->m * A->n);

    A->vetor = a;
    input.close();
    //print2Console(A);
}



void create_mat_identity(stMatrix *A){
    for (int j = 0; j < A->m; j++){
        for (int i = 0; i < A->n; i++){
            double c = 0.0f;
            int k = j * A->n + i;
            A->vetor[k] = 0.0;
            if (j == i)
                A->vetor[k] = 1.0;
        }//for (int jA = j; jA < A->m; jA++){

    }//end-for (int j = 0; j < mLattice->height; j++){


}

void create_mat_dense(stMatrix *A){

    std::mt19937_64  generator (time(nullptr)); //64 bits
    uniform_real_distribution<double> unif(0, 1); //uniform distribuition
    for (int j = 0; j < A->m; j++){
        for (int i = 0; i < A->n; i++){
            double c = 0.0f;
            int k = j * A->n + i;
            A->vetor[k] = unif(generator) ;//* 100.0f;
            if (unif(generator) < 0.12)
                A->vetor[k] *= -1.0f;
        }//for (int jA = j; jA < A->m; jA++){
    }//end-for (int j = 0; j < mLattice->height; j++){



}
//https://en.wikipedia.org/wiki/Diagonally_dominant_matrix#Examples
void create_mat_DDM(stMatrix *A,  stMatrix *B, stMatrix *X){
    create_mat_dense(A);
    std::mt19937_64  generator (time(nullptr)); //64 bits
    uniform_real_distribution<double> unif(0, 1); //uniform distribuition
    for (int j = 0; j < A->m; j++){
        double acc = 0.0f,
               aii = abs(A->vetor[j * A->n + j]);

        A->vetor[j * A->n + j] = 0.0f;
        for (int i = 0; i < A->n; i++){
            acc += abs(A->vetor[j * A->n + i]);
        }//for (int jA = j; jA < A->m; jA++){

        if (acc >= aii){
            double aux = acc / aii;
            aii *= aux;
        }


        A->vetor[j * A->n + j] = aii + 1.0;
    }//end-for (int j = 0; j < mLattice->height; j++){

    for (int i = 0; i < A->n; i++){
         X->vetor[i] = unif(generator) * 100.0f;
        if (unif(generator) < 0.12)  X->vetor[i] *= -1.0f;

    }


    for (int j = 0; j < A->m; j++){
        double acc = 0.0f;
        B->vetor[j] = 0.0f;
        for (int i = 0; i < A->n; i++){
            B->vetor[j] += A->vetor[j * A->n + i] * X->vetor[i];
        }//for (int jA = j; jA < A->m; jA++){
    }//end-for (int j = 0; j < mLattice->height; j++){

}
/*
void matrix_multi(stMatrix *C, stMatrix *A, stMatrix *B){
   for (int j = 0; j < C->n; j++){

        for (int i = 0; i < C->m; i++){
            int ck = j * C->m +  i;
            double c = 0.0f;
            for (int jA = i; jA < A->m; jA++){
                int ak = j * A->m + jA;
                int bk = jA * B->m + i;
                c += A->vetor[ak] * B->vetor[bk];
            }//for (int jA = j; jA < A->m; jA++){
            C->vetor[ck] = c;
        }//end-for (int j = 0; j < mLattice->height; j++){

    }//end-void InitRandness(tpLattice *mLattice, float p){

}
*/
