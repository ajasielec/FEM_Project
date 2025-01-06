//
// Created by Ania on 20.11.2024.
//

#ifndef GLOBALSYSTEMOFEQUATION_H
#define GLOBALSYSTEMOFEQUATION_H
#include "grid.h"


class GlobalSystemOfEquation {
public:
    Matrix<double> globalMatrixH;
    Matrix<double> globalMatrixC;
    Vector<double> globalVectorP;
    Vector<double> vectorT;

    void solve(Grid grid);

    void displayMatrixH();
    void displayMatrixC();
    void displayVectorP();
    void displayVectorT();
};

bool zeroOnDiagonal(Matrix<double> matrix);
Vector <double> gaussElimination(Matrix<double> H, Vector<double> P);
Matrix<double> scalMacierz(Matrix<double> a, Vector<double> b);

// void aggregateMatrixH(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);
// void aggregateVectorP(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);
void aggregate(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);



#endif //GLOBALSYSTEMOFEQUATION_H
