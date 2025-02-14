//
// Created by Ania on 20.10.2024.
//

#include "ElemUniv.h"

// UNIVERSAL ELEMENT constructor, takes number of points, creates ksi and eta tables, completing surface struct
ElemUniv::ElemUniv(int npc) {
    this->npc = npc;

    std::vector<Node> current_pc;
    std::vector<Node> current_N;

    if (npc == 1) {
        current_pc = pc[0];
    }
    else if (npc == 4) {
        current_pc = pc[1];
    }
    else if (npc == 9) {
        current_pc = pc[2];
    }
    else if (npc == 16) {
        current_pc = pc[3];
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

    // completing surface array
    if (npc == 4) {
        surface[0].boundary_pc = downSide[1];
        surface[1].boundary_pc = rightSide[1];
        surface[2].boundary_pc = upSide[1];
        surface[3].boundary_pc = leftSide[1];
    }
    else if (npc == 9) {
        surface[0].boundary_pc = downSide[2];
        surface[1].boundary_pc = rightSide[2];
        surface[2].boundary_pc = upSide[2];
        surface[3].boundary_pc = leftSide[2];
    }
    else if (npc == 16) {
        surface[0].boundary_pc = downSide[3];
        surface[1].boundary_pc = rightSide[3];
        surface[2].boundary_pc = upSide[3];
        surface[3].boundary_pc = leftSide[3];
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
