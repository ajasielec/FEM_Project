//
// Created by Ania on 20.11.2024.
//

#ifndef GLOBALSYSTEMOFEQUATION_H
#define GLOBALSYSTEMOFEQUATION_H
#include "grid.h"


class GlobalSystemOfEquation {
public:
    std::vector<std::vector<double>> globalMatrixH;
};

std::vector<std::vector<double>> aggregateMatrixH(const Grid&  grid, GlobalSystemOfEquation& globalSystemOfEquation);



#endif //GLOBALSYSTEMOFEQUATION_H
