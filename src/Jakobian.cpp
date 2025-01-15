#include "Jakobian.h"

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
    std::cout << "det[J] = " << detJ << std::endl;
    std::cout << "J^-1 =\n";
    for (int i = 0; i < 2; i++) {
        std::cout << "[" << J1[i][0] << ", " << J1[i][1] << "]" << std::endl;
    }
}
