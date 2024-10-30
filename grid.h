#ifndef GRID_H
#define GRID_H

#include "includes.h"
#include "Element.h"
#include "Node.h"
class Element;

// GRID structure
struct Grid {
    int nodes_number;
    int elements_number;
    std::vector<Node> nodes;
    std::vector<Element> elements;
    std::map<std::string, int> globalData;
    int npc; // wiersze w tabeli

    void displayGlobalData() const;
    void displayNodes() const;
    void displayElements() const;
    void displayAllMatrixH() const;

    Element findElementById(int id);
    Node findNodeById(int id);

    void calculateAllJakobians();
    void calculateAllShapeDerivatives();
    void calculateAllMatrixH();
};


#endif //GRID_H