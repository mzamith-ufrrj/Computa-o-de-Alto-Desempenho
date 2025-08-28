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
#include <rural-mat.hpp>


using namespace std;
void help (void){
    cout << "Programa para gerar matrizes aleatórias e vetores X e B para solução linear" << endl;
    cout << "Gerar matriz quadrada apenas, senão não há solução" << endl;
    cout << "Exemplo: " << endl;
    cout << "\t./generate-mat-ABX.exec 10 [matriz A].bin [vetor B].bin [vetor X].bin" << endl;
    cout << "\t                         |     |              |            |----------> vetor solução"  << endl;
    cout << "\t                         |     |              |-----------------------> vetor B"  << endl;
    cout << "\t                         |     |--------------------------------------> Matriz A com m equações 4 n incógnitas"  << endl;
    cout << "\t                         |--------------------------------------------> ordem da matriz"  << endl;
}

/*
 *  ./generate-mat-cpu.exec 10 10 1 r10x10.bin -> random
 * ./generate-mat-cpu.exec 10 10 0 i10x10.bin -> matriz identidade
 *
 */
int main (int ac, char **av){

    stMatrix A, B, X, X1;
    if (ac != 5){
        help();
        return EXIT_SUCCESS;
    }


    string filename_A = av[2],
           filename_B = av[3],
           filename_X = av[4];
    A.m = stoi(av[1]);
    A.n = A.m;

    B.m = A.m;
    B.n = 1;

    X.n = B.m;
    X.m = B.n;



    cout << endl << "Gerando: B = A*X válido" << endl;
    cout << "\t Matriz (" << A.m << "," << A.n << ")" <<  endl;
    cout << "\t - Arquivo matriz A: " << filename_A << endl;
    cout << "\t - Arquivo matriz B: " << filename_B << endl;
    cout << "\t - Arquivo matriz X: " << filename_X << endl;
    assert(posix_memalign(reinterpret_cast <void**>(&A.vetor), ALING, A.m *  A.n * sizeof(double)) == 0);
    assert(posix_memalign(reinterpret_cast <void**>(&B.vetor), ALING, B.m *  B.n * sizeof(double)) == 0);
    assert(posix_memalign(reinterpret_cast <void**>(&X.vetor), ALING, X.m *  X.n * sizeof(double)) == 0);

    memset(A.vetor, 0x00,  A.m *  A.n * sizeof(double));
    memset(B.vetor, 0x00,  B.m *  B.n * sizeof(double));
    memset(X.vetor, 0x00,  X.m *  X.n * sizeof(double));

    create_mat_DDM(&A, &B, &X);
    /*
    for (int j = 0; j < A.m; j++){
        cout  <<  " ";

        for (int i = 0; i < A.n; i++){
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


    print2Binary(&A, filename_A);
    print2Binary(&B, filename_B);
    print2Binary(&X, filename_X);
    free(A.vetor);
    free(B.vetor);
    free(X.vetor);
    return EXIT_SUCCESS;
}
