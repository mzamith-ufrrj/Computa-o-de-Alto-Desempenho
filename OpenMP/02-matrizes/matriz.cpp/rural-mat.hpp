#ifndef __RURAL_MAT_HPP__
#define __RURAL_MAT_HPP__
#include <string>
struct stMatrix{
    int n, m;
    double *vetor;
};
void print2Console(stMatrix *);
void print2Binary(stMatrix *, std::string);
void loadBinary(stMatrix *, std::string);
void create_mat_identity(stMatrix *);
void create_mat_dense(stMatrix *);
void create_mat_DDM(stMatrix*, stMatrix*, stMatrix*);
#endif

