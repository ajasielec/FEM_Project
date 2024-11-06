#ifndef ELEMENT_H
#define ELEMENT_H


#include "includes.h"
#include "Jakobian.h"
#include "grid.h"
class Grid;

struct Element {
    int id;
    int node_id [4];
    std::vector<Jakobian> jakobians;
    std::vector<std::vector<double>> dN_dx; // table dN/dx
    std::vector<std::vector<double>> dN_dy; // table dN/dy
    std::vector<std::vector<double>> H;     // H matrix

    Element();
    Element (int id, const int node_id[4]);

    void calculateJakobians(Grid& grid);
    void calculateShapeDerivatives(int npc);
    void calculateMatrixH(int npc, int k);

    void display() const;
    void display_dN_dx() const;
    void display_dN_dy() const;
    void display_H() const;

};



#endif //ELEMENT_H
