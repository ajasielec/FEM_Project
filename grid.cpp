#include "grid.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <ranges>

static const std::vector<Node> pc {
    Node(-1/sqrt(3), -1/sqrt(3)),   // pc1
    Node(1/sqrt(3), -1/sqrt(3)),   // pc2
    Node(1/sqrt(3), 1/sqrt(3)),   // pc3
    Node(-1/sqrt(3), 1/sqrt(3))   // pc4
};

// node
Node::Node() {}

Node::Node(const int id, const double x, const double y) : id(id), x(x), y(y) {}

Node::Node(double x, double y) : Node(0, x, y) {}

void Node::display() const {
    std::cout << id << ": (" << x << ", " << y << ")" << std::endl;
}

// element
Element::Element() {}

Element::Element(const int id, const int node_id[4]) : id(id) {
    std::copy(node_id, node_id + 4, this->node_id);
}

void Element::display() const {
        std::cout << id << ":\t\t(";
        for (int i = 0; i < 3; i++) {
            std::cout << node_id[i] << ", ";
        }
        std::cout << node_id[3] << ")" << std::endl;
}

// grid
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

Element Grid::findElementById(int id) {
    for (const auto& element : elements) {
        if (element.id == id) {
            return element;
        }
    }
    std::cout <<"Element with the given id does not exist";
}

Node Grid::findNodeById(int id) {
    for (const auto& node : nodes) {
        if (node.id == id) {
            return node;
        }
    }
    std::cout <<"Node with the given id does not exist";
}

// universal element
ElemUniv::ElemUniv(int npc) {

    dN_dE.reserve(npc);
    dN_dn.reserve(npc);

    for (int i = 0; i < npc; i++) {
        std::vector<double> x_row = {
            -0.25 * (1 - pc[i].y),
            0.25 * (1 - pc[i].y),
            0.25 * (1 + pc[i].y),
            -0.25 * (1 + pc[i].y)
        };

        std::vector<double> y_row = {
            -0.25 * (1 - pc[i].x),
            -0.25 * (1 + pc[i].x),
            0.25 * (1 + pc[i].x),
            0.25 * (1 - pc[i].x)
        };

        dN_dE.push_back(x_row);
        dN_dn.push_back(y_row);
    }
}

void ElemUniv::display() const {
    std::cout << "\nTabela ksi:\n";
    for (const auto& row : dN_dE) {
        for (double value : row) {
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nTabela eta:\n";
    for (const auto& row : dN_dn) {
        for (double value : row) {
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }
}

// Jakobian JEST DLA JEDNEGO WIERSZA!
Jakobian::Jakobian(ElemUniv e, Grid grid, int element_id, int row_id) {
    // taking an element
    const Element element = grid.findElementById(element_id);

    // taking nodes of given element
    std::vector<Node> nodes (4);
    for (int i = 0; i < 4; i++) {
        nodes[i] = grid.findNodeById(element.node_id[i]);
        nodes[i].display();
    }

    // row of the tables
    std::vector<double> const pc_E = e.dN_dE[row_id];
    std::vector<double> const pc_n = e.dN_dn[row_id];

    // elements of jakobian matrix
    double dx_dE = 0;
    double dy_dE = 0;
    double dx_dn = 0;
    double dy_dn = 0;

    // calculating jakobian
    for (int i = 0; i < 4; i++) {
        dx_dE += pc_E[i] * nodes[i].x;
        dy_dE += pc_E[i] * nodes[i].y;
        dx_dn += pc_n[i] * nodes[i].x;
        dy_dn += pc_n[i] * nodes[i].y;
    }

    J[0][0] = dx_dE;
    J[0][1] = dy_dE;
    J[1][0] = dx_dn;
    J[1][1] = dy_dn;
}

void Jakobian::displayJakobian() {
    std::cout << "Jakobian:" << std::endl;
    for (int i = 0; i < 2; i++) {
        std::cout << "[" << J[i][0] << ", " << J[i][1] << "]" << std::endl;
    }
}

std::vector<Jakobian> calculateJakobiansOfElement(int element_id, Grid grid, ElemUniv e) {

    std::vector<Jakobian> result;

    // taking an element
    const Element element = grid.findElementById(element_id);

    // taking nodes of given element
    std::vector<Node> nodes (4);
    for (int i = 0; i < 4; i++) {
        nodes[i] = grid.findNodeById(element.node_id[i]);
        nodes[i].display();
    }

    //calculating jakobian
    for (int i = 0; i < 4; i++) {
       // Jakobian jakobian(e)
      //  result[i] = jakobian;
    }
}





