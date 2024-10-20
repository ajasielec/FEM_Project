#ifndef QUADRATURE_H
#define QUADRATURE_H

#include "includes.h"

class Quadrature {
    int nodes_number;
    std::vector<double> points;
    std::vector<double> wages;
public:
    explicit Quadrature(int nodes_number);
    double solveQuadratures(double (*f)(double x)) const;
    double solveQuadratures(double (*f)(double x, double y)) const;
};


#endif //QUADRATURE_H
