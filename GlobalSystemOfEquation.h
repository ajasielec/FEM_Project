//
// Created by Ania on 20.11.2024.
//

#ifndef GLOBALSYSTEMOFEQUATION_H
#define GLOBALSYSTEMOFEQUATION_H
#include "grid.h"


class GlobalSystemOfEquation {
public:
    Matrix<double> globalMatrixH;
    Vector<double> globalVectorP;
    Vector<double> vectorT;

    void solve();

    void displayMatrixH();
    void displayVectorP();
};

bool zeroOnDiagonal(Matrix<double> matrix);
Vector <double> GaussElimination(Matrix<double> matrix);
Matrix<double> scalMacierz(Matrix<double> a, Vector<double> b);

void aggregateMatrixH(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);
void aggregateVectorP(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);



#endif //GLOBALSYSTEMOFEQUATION_H
