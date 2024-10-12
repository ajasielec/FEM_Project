#include "grid.h"
#include <iostream>
#include <fstream>

void Node::display() const {
    std::cout << id << ": (" << x << ", " << y << ")" << std::endl;
}

void Element::display() const {
        std::cout << id << ":\t\t(";
        for (int i = 0; i < 3; i++) {
            std::cout << node_id[i] << ", ";
        }
        std::cout << node_id[3] << ")" << std::endl;
}

void Grid::displayGlobalData() const {
    std::cout << "Global Data:" << std::endl;
    for (const auto&[fst, snd] : globalData) {
        std::cout << fst << ": " << snd << std::endl;
    }
}

void Grid::displayNodes() const {
    std::cout << "\nNodes (" << nodes_number <<"): " << std::endl;
    std::cout << "id:\t x:\t\ty:" << std::endl;
    for (const auto& node : nodes) {
        node.display();
    }
}

void Grid::displayElements() const {
    std::cout << "\nElements (" << elements_number <<"): " << std::endl;
    std::cout << "elem_id:\t nodes_ids:" << std::endl;
    for (const auto& element : elements) {
        element.display();
    }
}