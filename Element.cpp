//
// Created by Ania on 20.10.2024.
//

#include "Element.h"
#include "ElemUniv.h"

// ELEMENT constructors
Element::Element(): id(-1) {    // id and node_id = -1 if no values are given
    for (int & i : node_id)
        i = -1;
}
Element::Element(const int id, const int node_id[4]) : id(id) {
    std::copy(node_id, node_id + 4, this->node_id);
}

// calculate jakobians of each npc
void Element::calculateJakobians(Grid& grid) {

    // taking nodes of an element
    std::vector<Node> nodes (4);
    for (int i = 0; i < 4; i++) {
        nodes[i] = grid.findNodeById(node_id[i]);
    }

    // creating universal element
    ElemUniv elem_univ(grid.npc);

    //calculating jakobian for each npc
    for (int i = 0; i < grid.npc; i++) {
        Jakobian jakobian;
        jakobian.calculateJakobian(elem_univ, nodes, i);
        jakobian.calculateInverse();
        jakobians.push_back(jakobian);
    }
}

// calculating shape derivatives
void Element::calculateShapeDerivatives(int npc){
    // creating elem univ
    ElemUniv elem_univ(npc);

    double dNdx = 0;
    double dNdy = 0;

    for (int i = 0; i < npc; i++) {

        std::vector<double> x_row;
        std::vector<double> y_row;

        // completing the tables
        for (int j = 0; j < 4; j++) {
            dNdx = jakobians[i].J1[0][0] * elem_univ.dN_dE[i][j] + jakobians[i].J1[0][1] * elem_univ.dN_dn[i][j];
            dNdy = jakobians[i].J1[1][0] * elem_univ.dN_dE[i][j] + jakobians[i].J1[1][1] * elem_univ.dN_dn[i][j];
            x_row.push_back(dNdx);
            y_row.push_back(dNdy);
        }
        dN_dx.push_back(x_row);
        dN_dy.push_back(y_row);
    }
}
// calculating H matrix
void Element::calculateMatrixH(int npc, int conductivity) {
    ElemUniv elem_univ(npc);
    std::vector<std::vector<double>> H(npc, std::vector<double>(4, 0.0));
    std::vector<std::vector<double>> Hpc(npc, std::vector<double>(4, 0.0));

    // assigning wages
    std::vector<Node> current_wages;
    if (npc == 1) {
        current_wages = wages[0];
    }
    else if (npc == 4) {
        current_wages = wages[1];
    }
    else if (npc == 9) {
        current_wages = wages[2];
    }

    for (int i = 0; i < npc; i++) {
        std::vector<double> dNdx = dN_dx[i];
        std::vector<double> dNdy = dN_dy[i];

        // calculating H matrix elements for the whole element
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                H[j][k] += jakobians[i].detJ * conductivity * (dNdx[j] * dNdx[k] + dNdy[j] * dNdy[k]) * current_wages[i].x * current_wages[i].y;
                // std::cout << H[j][k] <<"\t";
            }
            // std::cout << std::endl;
        }
    }
    this->H = H;
}

// method that displays a single element
void Element::display() const {
    std::cout << id << ":\t\t(";
    for (int i = 0; i < 3; i++) {
        std::cout << node_id[i] << ", ";
    }
    std::cout << node_id[3] << ")" << std::endl;
}

// displaying dN/dx table
void Element::display_dN_dx() const {
    std::cout << "\ndN/dx table:\n";
    for (const auto& row : dN_dx) {
        for (double value : row) {
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }
}
// displaying dN/dy table
void Element::display_dN_dy() const {
    std::cout << "\ndN/dy table:\n";
    for (const auto& row : dN_dy) {
        for (double value : row) {
            std::cout << value << ", ";
        }
        std::cout << std::endl;
    }
}
// displaying H matrix
void Element::display_H() const {
    std::cout << "\nElement " << id << ":\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << H[i][j] << " ";
        }
        std::cout << "\n";
    }
}


