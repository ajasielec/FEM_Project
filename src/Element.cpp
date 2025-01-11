//
// Created by Ania on 20.10.2024.
//

#include "Element.h"

#include <thread>

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
void Element::calculateMatrixH(Grid& grid) {
    double alpha = grid.globalData["Alfa"];
    double conductivity = grid.globalData["Conductivity"];
    double c = grid.globalData["SpecificHeat"];
    double ro = grid.globalData["Density"];

    int npc = grid.npc;
    double ambient_temp = grid.globalData["Tot"];

    ElemUniv elem_univ(npc);
    Matrix<double> H(npc, Vector<double>(4, 0.0));
    Matrix<double> C(npc, Vector<double>(4, 0.0));
    Matrix<double> Hpc(npc, Vector<double>(4, 0.0));
    Vector<double> P(4, 0.0);

    // assigning wages
    std::vector<Node> current_wages;
    Vector<double> BC_wages;
    if (npc == 1) {
        current_wages = wages[0];
    }
    else if (npc == 4) {
        current_wages = wages[1];
        BC_wages.push_back(1.0);
        BC_wages.push_back(1.0);
    }
    else if (npc == 9) {
        current_wages = wages[2];
        BC_wages.push_back(5.0/9.0);
        BC_wages.push_back(8.0/9.0);
        BC_wages.push_back(5.0/9.0);
    }
    else if (npc == 16) {
        current_wages = wages[3];
        BC_wages.push_back(0.347855);
        BC_wages.push_back(0.652145);
        BC_wages.push_back(0.652145);
        BC_wages.push_back(0.347855);
    }

    for (int i = 0; i < npc; i++) {
        std::vector<double> dNdx = dN_dx[i];
        std::vector<double> dNdy = dN_dy[i];

        // calculating H matrix elements for the whole element
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                H[j][k] += jakobians[i].detJ * conductivity * (dNdx[j] * dNdx[k] + dNdy[j] * dNdy[k]) * current_wages[i].x * current_wages[i].y;
            }
        }
    }

    // calculating HBC matrix
    // completing N tables
    for (int j = 0 ; j < 4; j++) {
        for (int i = 0; i < sqrt(npc); i++) {
            double ksi = elem_univ.surface[j].boundary_pc[i].x;
            double eta = elem_univ.surface[j].boundary_pc[i].y;
            Vector<double> row;
            row.push_back(0.25 * (1 - ksi) * (1 - eta));    // N1
            row.push_back(0.25 * (1 + ksi) * (1 - eta));    // N2
            row.push_back(0.25 * (1 + ksi) * (1 + eta));    // N3
            row.push_back(0.25 * (1 - ksi) * (1 + eta));    // N4
            elem_univ.surface[j].N.push_back(row);
        }
    }

    // down
    Node node1 = grid.findNodeById(this->node_id[0]);
    Node node2 = grid.findNodeById(this->node_id[1]);
    if (node1.BC && node2.BC) {
        double det_up = (node1.x - node2.x) / 2;
        for (int i = 0; i < sqrt(npc); i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    H[j][k] += det_up * alpha * (elem_univ.surface[0].N[i][j] * elem_univ.surface[0].N[i][k]) * BC_wages[i];
                }
                P[j] +=  det_up * alpha * ambient_temp * elem_univ.surface[0].N[i][j] * BC_wages[i];
            }
        }
    }

    // right
    node1 = grid.findNodeById(this->node_id[1]);
    node2 = grid.findNodeById(this->node_id[2]);
    if (node1.BC && node2.BC) {
        double det_left = (node1.y - node2.y) / 2;
        for (int i = 0; i < sqrt(npc); i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    H[j][k] += det_left * alpha * (elem_univ.surface[1].N[i][j] * elem_univ.surface[1].N[i][k]) * BC_wages[i];
                }
                P[j] += det_left * alpha * ambient_temp * elem_univ.surface[1].N[i][j] * BC_wages[i];
            }
        }
    }

    // up
    node1 = grid.findNodeById(this->node_id[2]);
    node2 = grid.findNodeById(this->node_id[3]);
    if (node1.BC && node2.BC) {
        double det_down = (node2.x - node1.x) / 2;
        for (int i = 0; i < sqrt(npc); i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    H[j][k] += det_down * alpha * (elem_univ.surface[2].N[i][j] * elem_univ.surface[2].N[i][k]) * BC_wages[i];
                }
                P[j] += det_down * alpha * ambient_temp * elem_univ.surface[2].N[i][j] * BC_wages[i];
            }
        }
    }

    // left
    node1 = grid.findNodeById(this->node_id[3]);
    node2 = grid.findNodeById(this->node_id[0]);
    if (node1.BC && node2.BC) {
        double det_right = (node2.y - node1.y) / 2;
        for (int i = 0; i < sqrt(npc); i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    H[j][k] += det_right * alpha * (elem_univ.surface[3].N[i][j] * elem_univ.surface[3].N[i][k]) * BC_wages[i];
                }
                P[j] += det_right * alpha * ambient_temp * elem_univ.surface[3].N[i][j] * BC_wages[i];
            }
        }
    }

    std::vector<Node> current_pc;
    if (grid.npc == 4) current_pc = pc[1];
    else if (grid.npc == 9) current_pc = pc[2];
    else if (grid.npc == 16) current_pc = pc[3];

    // calculating C

    // N tables
    Matrix<double> N;
    for (int i = 0; i < npc; i++) {
        double ksi = current_pc[i].x;
        double eta = current_pc[i].y;
        double N1 = 0.25 * (1 - ksi) * (1 - eta);
        double N2 = 0.25 * (1 + ksi) * (1 - eta);
        double N3 = 0.25 * (1 + ksi) * (1 + eta);
        double N4 = 0.25 * (1 - ksi) * (1 + eta);
        Vector<double> row;
        row.push_back(N1);
        row.push_back(N2);
        row.push_back(N3);
        row.push_back(N4);
        N.push_back(row);
    }

    for (int i = 0; i < npc; i++) {
        double detJ = jakobians[i].detJ / 2;
        std::vector<double> dNdx = N[i];
        std::vector<double> dNdy = N[i];

        // calculating C matrix elements for the whole element
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                C[j][k] += c * ro * detJ * (dNdx[j] * dNdx[k] + dNdy[j] * dNdy[k]) * current_wages[i].x * current_wages[i].y;
            }
        }
    }

    this->C = C;
    this->H = H;
    this->P = P;
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

void Element::display_C() const {
    std::cout << "\nElement " << id << ":\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << C[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Element::display_P() const {
    std::cout << "\nElement " << id << ":\n";
    for (int i = 0; i < 4; ++i) {
            std::cout << P[i] << " ";
    }
    std::cout << std::endl;
}


