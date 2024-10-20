//
// Created by Ania on 20.10.2024.
//

#ifndef ELEMUNIV_H
#define ELEMUNIV_H

#include "includes.h"

struct ElemUniv {
    std::vector<std::vector<double>> dN_dE; // dla x
    std::vector<std::vector<double>>  dN_dn; // dla y

    explicit ElemUniv (int npc);
    void display() const;
};



#endif //ELEMUNIV_H
