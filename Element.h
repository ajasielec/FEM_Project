#ifndef ELEMENT_H
#define ELEMENT_H

#include "includes.h"
#include "Jakobian.h"

struct Element {
    int id;
    int node_id [4]{};
    Jakobian jakobian;
    std::vector<Jakobian> jakobians;
    // tabele dN/dx i dN/dy
    std::vector<std::vector<double>> dN_dx; // dla x
    std::vector<std::vector<double>> dN_dy; // dla y
    std::vector<std::vector<double>> H;

    Element();
    Element (int id, const int node_id[4]);
    void display() const;
    void display_dN_dx() const;
    void display_dN_dy() const;
};



#endif //ELEMENT_H
