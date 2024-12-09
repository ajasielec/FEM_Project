//
// Created by Ania on 04.12.2024.
//

#ifndef ELEM2D_4_H
#define ELEM2D_4_H
#include "ElemUniv.h"


struct Elem2d_4 {
    std::vector<std::vector<double>> dNdKsi; // dla x
    std::vector<std::vector<double>>  dNdEta; // dla y

    Elem2d_4(ElemUniv);
};



#endif //ELEM2D_4_H
