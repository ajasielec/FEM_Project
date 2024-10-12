#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <map>

// structures
struct Node {
    int id;
    double x, y;
    void display() const;
};

struct Element {
    int id;
    int node_id [4];
    void display() const;
};

struct Grid {
    int nodes_number;
    int elements_number;
    std::vector<Node> nodes;
    std::vector<Element> elements;
    std::map<std::string, int> globalData;

    void displayGlobalData() const;
    void displayNodes() const;
    void displayElements() const;
};

#endif //GRID_H