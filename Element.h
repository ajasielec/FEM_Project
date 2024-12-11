#ifndef ELEMENT_H
#define ELEMENT_H


#include "includes.h"
#include "Jakobian.h"
#include "grid.h"
class Grid;

struct Element {
    int id;
    int node_id [4];
    Vector<Jakobian> jakobians;
    Matrix<double> dN_dx;   // table dN/dx
    Matrix<double> dN_dy;   // table dN/dy
    Matrix<double> H;       // H matrix
    Matrix<double> Hbc;     // Hbc matrix
    Vector<double> P;       // vector P

    Element();
    Element (int id, const int node_id[4]);

    void calculateJakobians(Grid& grid);
    void calculateShapeDerivatives(int npc);
    void calculateMatrixH(Grid& grid);
    void calculateVectorP(int npc, Grid& grid);

    void display() const;
    void display_dN_dx() const;
    void display_dN_dy() const;
    void display_H() const;
    void display_P() const;

};



#endif //ELEMENT_H
