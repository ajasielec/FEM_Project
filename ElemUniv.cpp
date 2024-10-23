//
// Created by Ania on 20.10.2024.
//

#include "ElemUniv.h"
#include "Node.h"

// values of nodes, used for creating a ksi and eta tables
static const std::vector<Node> pc {
    Node(-1/sqrt(3), -1/sqrt(3)),   // pc1
    Node(1/sqrt(3), -1/sqrt(3)),   // pc2
    Node(-1/sqrt(3), 1/sqrt(3)),   // pc3
    Node(1/sqrt(3), 1/sqrt(3))   // pc4
};

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
