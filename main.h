#pragma once

// all includes
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>


// structures
struct Node {
    int id;
    double x, y;

    void display() const {
        std::cout << id << ": (" << x << ", " << y << ")" << std::endl;
    }
};

struct Element {
    int id;
    int node_id [4];

    void display() const {
        std::cout << id << ":\t\t(";
        for (int i = 0; i < 3; i++) {
            std::cout << node_id[i] << ", ";
        }
        std::cout << node_id[3] << ")" << std::endl;
    }
};

struct Grid {
    int nodes_number;
    int elements_number;
    std::vector<Node> nodes;
    std::vector<Element> elements;
    std::map<std::string, int> globalData;

    void displayGlobalData() const {
        std::cout << "Global Data:" << std::endl;
        for (const auto&[fst, snd] : globalData) {
            std::cout << fst << ": " << snd << std::endl;
        }
    }
    void displayNodes() const {
        std::cout << "\nNodes (" << nodes_number <<"): " << std::endl;
        std::cout << "id:\t x:\t\ty:" << std::endl;
        for (const auto& node : nodes) {
            node.display();
        }
    }
    void displayElements() const {
        std::cout << "\nElements (" << elements_number <<"): " << std::endl;
        std::cout << "elem_id:\t nodes_ids:" << std::endl;
        for (const auto& element : elements) {
            element.display();
        }
    }
};




// functions
Grid readFromFile(const std::string& path);


