//
// Created by Ania on 20.10.2024.
//

#include "Element.h"

// ELEMENT constructors
Element::Element(): id(-1) {    // id and node_id = -1 if no values are given
    for (int & i : node_id)
        i = -1;
}
Element::Element(const int id, const int node_id[4]) : id(id) {
    std::copy(node_id, node_id + 4, this->node_id);
}

// method that displays a single element
void Element::display() const {
    std::cout << id << ":\t\t(";
    for (int i = 0; i < 3; i++) {
        std::cout << node_id[i] << ", ";
    }
    std::cout << node_id[3] << ")" << std::endl;
}