//
// Created by Ania on 20.11.2024.
//

#include "GlobalSystemOfEquation.h"
#include "includes.h"


// std::vector<std::vector<double>> globalMatrixH;

std::vector<std::vector<double>> aggregateMatrixH(const Grid& grid, GlobalSystemOfEquation& globalSystemOfEquation) {
    int size = grid.nodes_number;
    std::vector<std::vector<double>> globalMatrixH (size, std::vector<double>(size, 0));    //matrix size x size

    for (int i = 0; i < grid.elements.size(); i++) {
        std::vector<std::vector<double>> localMatrixH = grid.elements[i].H;

        // node id danego elementu
        int node_ids[4];
        for (int j = 0; j < 4; ++j) {
            node_ids[j] = grid.elements[i].node_id[j];
        }

        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                globalMatrixH[node_ids[j]-1][node_ids[k]-1] = localMatrixH[j][k];
            }
        }
    }

    globalSystemOfEquation.globalMatrixH = globalMatrixH;

    return globalMatrixH;
}