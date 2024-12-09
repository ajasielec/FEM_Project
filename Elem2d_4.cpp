//
// Created by Ania on 04.12.2024.
//

#include "Elem2d_4.h"

Elem2d_4::Elem2d_4(ElemUniv elem_univ) {
    std::vector<Node> current_pc;
    std::vector<Node> current_wages;
    int npc = elem_univ.npc;

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

    dNdKsi.reserve(npc);
    dNdEta.reserve(npc);

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

        dNdKsi.push_back(x_row);
        dNdEta.push_back(y_row);
    }
}
