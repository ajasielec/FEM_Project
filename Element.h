#ifndef ELEMENT_H
#define ELEMENT_H

#include "includes.h"
#include "Jakobian.h"

struct Element {
    int id;
    int node_id [4]{};
    Jakobian jakobian;
    std::vector<Jakobian> jakobians;

    Element();
    Element (int id, const int node_id[4]);
    void display() const;
};



#endif //ELEMENT_H
