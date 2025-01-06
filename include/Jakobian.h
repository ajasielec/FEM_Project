//
// Created by Ania on 20.10.2024.
//

#ifndef JAKOBIAN_H
#define JAKOBIAN_H

#include "includes.h"
#include "ElemUniv.h"
#include "Node.h"

struct Jakobian {
    double J[2][2];
    double J1[2][2];
    double detJ;

    // taking tables and nodes
    Jakobian();
    void displayJakobian();
    void calculateJakobian(ElemUniv e, std::vector<Node> nodes, int row_id);
    void calculateInverse();
};


#endif //JAKOBIAN_H
