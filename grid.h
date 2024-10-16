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
};

struct ElemUniv {
    std::vector<std::vector<double>> dN_dE; // dla x
    std::vector<std::vector<double>>  dN_dn; // dla y

    explicit ElemUniv (int npc);
    void display() const;
};

struct Jakobian {
    double J[4][4];
    double J1[4][4];
    double detJ;
};

#endif //GRID_H