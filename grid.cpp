#include "grid.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <ranges>

// values of nodes, used for creating a ksi and eta tables
static const std::vector<Node> pc {
    Node(-1/sqrt(3), -1/sqrt(3)),   // pc1
    Node(1/sqrt(3), -1/sqrt(3)),   // pc2
    Node(1/sqrt(3), 1/sqrt(3)),   // pc3
    Node(-1/sqrt(3), 1/sqrt(3))   // pc4
};

// NODE constructors
Node::Node() : id(-1), x(0), y(0) {}    // id = -1 if no values are gives
Node::Node(const int id, const double x, const double y) : id(id), x(x), y(y) {}
Node::Node(double x, double y) : id(0), x(x), y(y) {}   // default id = 0

// method that displays a single node
void Node::display() const {
    std::cout << id << ": (" << x << ", " << y << ")" << std::endl;
}

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

// UNIVERSAL ELEMENT constructor, takes number of points, creates ksi and eta tables
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
// displaying ksi and eta tables
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

// JAKOBIAN class constructor
Jakobian::Jakobian() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            J[i][j] = 0;
            J1[i][j] = 0;
            detJ = 0;
        }
    }

} // default, all values are 0

// JAKOBIAN methods
// calculating jakobian matrix
void Jakobian::calculateJakobian(ElemUniv e, std::vector<Node> nodes, int row_id) {
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

// calculating inverse matrix of jakobian matrix -> J^-1
void Jakobian::calculateInverse() {
    detJ = J[0][0] * J[1][1] - J[0][1] * J[1][0];
    double inv_detJ = 1 / detJ;
    J1[0][0] = J[1][1] * inv_detJ;
    J1[1][1] = J[0][0] * inv_detJ;
    J1[0][1] = -J[0][1] * inv_detJ;
    J1[1][0] = -J[1][0] * inv_detJ;

    // changing -0 to 0
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (J1[i][j] == -0.0) {
                J1[i][j] = 0.0;
            }
        }
    }
}

// displaying jakobian matrix, its inverse and determinant
void Jakobian::displayJakobian() {
    std::cout << "J =\n";
    for (int i = 0; i < 2; i++) {
        std::cout << "[" << J[i][0] << ", " << J[i][1] << "]" << std::endl;
    }
    std::cout << "J^-1 =\n";
    for (int i = 0; i < 2; i++) {
        std::cout << "[" << J1[i][0] << ", " << J1[i][1] << "]" << std::endl;
    }
    std::cout << "det[J] = " << detJ << std::endl;
}

// FUNCTIONS
// function that can calculate all jakobians in one element
std::vector<Jakobian> calculateJakobiansOfElement(int element_id, Grid grid, ElemUniv e) {

    std::vector<Jakobian> result;

    // taking an element
    const Element element = grid.findElementById(element_id);

    // taking nodes of given element
    std::vector<Node> nodes (4);
    for (int i = 0; i < 4; i++) {
        nodes[i] = grid.findNodeById(element.node_id[i]);
    }

    //calculating jakobian for each npc
    for (int i = 0; i < 4; i++) {
        Jakobian jakobian;
        jakobian.calculateJakobian(e, nodes, i);
        jakobian.calculateInverse();
        result.push_back(jakobian);
       }

    return result;
}







