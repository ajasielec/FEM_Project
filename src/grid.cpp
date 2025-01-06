#include "grid.h"

// GRID METHODS
// displaying global data map
void Grid::displayGlobalData() const {
    std::cout << "Global Data:" << std::endl;
    for (const auto&[fst, snd] : globalData) {
        std::cout << fst << ": " << snd << std::endl;
    }
}
// displaying nodes of the grid
void Grid::displayNodes() const {
    std::cout << "\nNodes (" << nodes_number <<"): " << std::endl;
    std::cout << "id:\t x:\t\ty:" << std::endl;
    for (const auto& node : nodes) {
        node.display();
    }
}
// displaying elements of the grid
void Grid::displayElements() const {
    std::cout << "\nElements (" << elements_number <<"): " << std::endl;
    std::cout << "elem_id:\t nodes_ids:" << std::endl;
    for (const auto& element : elements) {
        element.display();
    }
}
// displaying H matrix of each element
void Grid::displayAllMatrixH() const {
    for (const auto& element : elements) {
        element.display_H();
    }
}

// returning an element with given id
Element Grid::findElementById(int id) {
    for (const auto& element : elements) {
        if (element.id == id) {
            return element;
        }
    }
    std::cout <<"Element with the given id does not exist";
    return {};
}
// returning a node with given id
Node Grid::findNodeById(int id) {
    for (const auto& node : nodes) {
        if (node.id == id) {
            return node;
        }
    }
    std::cout <<"Node with the given id does not exist";
    return {};
}

// calculating jakobians of each npc of each element
void Grid::calculateAllJakobians() {
    for (auto& element : elements) {
        element.calculateJakobians(*this);
    }
}
// calculating shape derivatives of each element
void Grid::calculateAllShapeDerivatives() {
    for (auto& element : elements) {
        element.calculateShapeDerivatives(npc);
    }
}
//calculating H matrix for each element
void Grid::calculateAllMatrixH() {
    for (auto& element : elements) {
        element.calculateMatrixH(*this);
    }
}











