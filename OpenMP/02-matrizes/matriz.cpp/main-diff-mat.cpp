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
#define EPSILON 1E-30

using namespace std;
void help (void){
    cout << "Programa para comparar duas matrizes" << endl;
    cout << "- ER entre duas matrizes" << endl;
    //cout << "- RMSE entre duas matrizes" << endl;
    cout << "Exemplo: " << endl;
    cout << "\t./diff-mat.exec 1E-10  [matriz A].bin [matriz B].bin" << endl;
    cout << "\t                 |          |            |-------> matriz a ser comparada " << endl;
    cout << "\t                 |          |--------------------> matriz a ser comparada" << endl;
    cout << "\t                 |-------------------------------> Erro relativo considerado" << endl;

}

double  diffVetores(stMatrix X0, stMatrix X1){
    assert((X0.m == X1.m) && (X0.n == X1.n));
    double err = 0.0f;
    for (unsigned int i = 0; i < X0.m; i++){
        for (unsigned int j = 0; j < X0.n; j++){
            unsigned int k = i * X0.n + j;
            double b = fabs((X1.vetor[k] - X0.vetor[k]) / (X1.vetor[k] + EPSILON));
            if (b > err)
                err = b;
        }
    }
    return err;
}


/*

 * ./diff-mat.exec 1E-10 X-out.bin X1000.bin
 */
int main (int ac, char **av){

    stMatrix X_ana, X_app;

    if (ac != 4){
        help();
        return EXIT_SUCCESS;
    }

    double errorMax = stof(av[1]);

    string filename_X_anali = av[2],
           filename_X_appro = av[3];

    cout << endl << "Comparação entre o valor exato da matriz e o aproximado" << endl;
    cout << endl << "Erro máximo utilizado no Método aproximativo: " << fixed << setprecision(2) << scientific << errorMax << endl;
    cout << "\t      Arquivo X exato: " << filename_X_anali << endl;
    cout << "\t Arquivo X aproximado: " << filename_X_appro << endl;

    loadBinary(&X_ana, filename_X_anali);
    loadBinary(&X_app, filename_X_appro);
    double err = diffVetores(X_app, X_ana);
    bool ok = err < errorMax;
    cout << "\t                 |EA|: "  << fixed << setprecision(2) << scientific << err << " < " << errorMax << " ["  << boolalpha << ok << "]" << endl;



    free(X_ana.vetor);
    free(X_app.vetor);

    return EXIT_SUCCESS;
}
