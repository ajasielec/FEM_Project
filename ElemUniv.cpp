//
// Created by Ania on 20.10.2024.
//

#include "ElemUniv.h"


// UNIVERSAL ELEMENT constructor, takes number of points, creates ksi and eta tables
ElemUniv::ElemUniv(int npc) {

    std::vector<Node> current_pc;
    std::vector<Node> current_wages;

    if (npc == 1) {
        current_pc = pc[0];
        current_wages = wages[0];
    }
    else if (npc == 4) {
        current_pc = pc[1];
        current_wages = wages[1];
    }
    else if (npc == 9) {
        current_pc = pc[2];
        current_wages = wages[2];
    }

    dN_dE.reserve(npc);
    dN_dn.reserve(npc);

    for (int i = 0; i < npc; i++) {
        std::vector<double> x_row = {
            -0.25 * (1 - current_pc[i].y),
            0.25 * (1 - current_pc[i].y),
            0.25 * (1 + current_pc[i].y),
            -0.25 * (1 + current_pc[i].y)
        };

        std::vector<double> y_row = {
            -0.25 * (1 - current_pc[i].x),
            -0.25 * (1 + current_pc[i].x),
            0.25 * (1 + current_pc[i].x),
            0.25 * (1 - current_pc[i].x)
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
