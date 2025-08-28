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
    cout << "Programa para gerar matrizes aleatórias e identidade" << endl;
    cout << "Exemplo: " << endl;

    cout << "\t./generate-mat-cpu.exec 10 10 1 r10x10.bin" << endl;
    cout << "\t                         |  | |  |-----> Arquivo binário com a matriz " << endl;
    cout << "\t                         |  | |--------> 1 indica matriz randomica e 0 identidade " << endl;
    cout << "\t                         |--|----------> respectivamente colunas e linhas da matriz"  << endl;
}

/*
 *  ./generate-mat-cpu.exec 10 10 1 r10x10.bin -> random
 * ./generate-mat-cpu.exec 10 10 0 i10x10.bin -> matriz identidade
 *
 */
int main (int ac, char **av){

    stMatrix A, B, X;
    if (ac != 5){
        help();
        return EXIT_SUCCESS;
    }

    string filename = av[4], smatType;
    A.m = stoi(av[1]);
    A.n = stoi(av[2]);
    int matType = stoi(av[3]);
    switch (matType){
        case 0:smatType = "IDENTITY";break;
        case 1:smatType = "RANDOM DENSE";break;

    }



    cout << endl << "Gerando matriz" << endl;
    cout << "\tMatriz (" << A.m << "," << A.n << ") \t Type: " << smatType <<  endl;
    cout << "\t File name: " << filename << endl;
    assert(posix_memalign(reinterpret_cast <void**>(&A.vetor), ALING, A.m *  A.n * sizeof(double)) == 0);
    memset(A.vetor, 0x00,  A.m *  A.n * sizeof(double));



    switch (matType){
        case 0:create_mat_identity(&A);break;
        case 1:create_mat_dense(&A);break;

    }



    print2Binary(&A, filename);
    free(A.vetor);
    return EXIT_SUCCESS;
}
