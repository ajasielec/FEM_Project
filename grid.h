#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <map>

// structures
struct Node {
    int id;
    double x, y;
    Node();
    Node(double x, double y);
    Node(int id, double x, double y);
    void display() const;
};

struct Element {
    int id;
    int node_id [4];
    Element();
    Element (int id, const int node_id[4]);
    void display() const;
};

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
    Element findElementById(int id);
    Node findNodeById(int id);
};

struct ElemUniv {
    std::vector<std::vector<double>> dN_dE; // dla x
    std::vector<std::vector<double>>  dN_dn; // dla y

    explicit ElemUniv (int npc);
    void display() const;
};

struct Jakobian {
    double J[2][2];
    double J1[2][2];
    double detJ;

    // taking tables and nodes
    Jakobian(ElemUniv e, Grid grid, int element_id, int row_id);
    void displayJakobian();
};

std::vector<Jakobian> calculateJakobiansOfElement(int element_id, Grid grid, ElemUniv e);


#endif //GRID_H