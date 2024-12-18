//
// Created by Ania on 20.11.2024.
//

#include "GlobalSystemOfEquation.h"
#include "includes.h"
#define SMALL_NUMBER 0.0001

void GlobalSystemOfEquation::solve() {
    // metoda gaussa
    vectorT = gaussElimination(globalMatrixH, globalVectorP);
}

void GlobalSystemOfEquation::displayMatrixH() {
    for (int i = 0; i < globalMatrixH.size(); i++) {
        for (int j = 0; j < globalMatrixH[i].size(); j++) {
            std::cout << globalMatrixH[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void GlobalSystemOfEquation::displayMatrixC() {
    for (int i = 0; i < globalMatrixC.size(); i++) {
        for (int j = 0; j < globalMatrixC[i].size(); j++) {
            std::cout << globalMatrixC[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void GlobalSystemOfEquation::displayVectorP() {
    for (int i = 0; i < globalVectorP.size(); i++) {
        std::cout << globalVectorP[i] << " ";
    }
    std::cout << std::endl;
}

void GlobalSystemOfEquation::displayVectorT() {
    for (int i = 0; i < vectorT.size(); i++) {
        std::cout << vectorT[i] << " ";
    }
    std::cout << std::endl;
}

Vector<double> gaussElimination(Matrix<double> H, Vector<double> P) {
    int n = H.size();
    Vector t(n, 0.0);

    for (int i = 0; i < n; i++) {
        // Szukanie maksymalnego elementu w kolumnie
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(H[k][i]) > abs(H[maxRow][i]))
                maxRow = k;
        }

        // Zamiana wierszy
        std::swap(H[i], H[maxRow]);
        std::swap(P[i], P[maxRow]);

        // Normalizacja wiersza
        for (int k = i + 1; k < n; k++) {
            double c = -H[k][i] / H[i][i];
            for (int j = i; j < n; j++) {
                if (i == j) {
                    H[k][j] = 0;
                } else {
                    H[k][j] += c * H[i][j];
                }
            }
            P[k] += c * P[i];
        }
    }

    // Rozwiązanie układu poprzez podstawienie wsteczne
    for (int i = n - 1; i >= 0; i--) {
        t[i] = P[i] / H[i][i];
        for (int k = i - 1; k >= 0; k--) {
            P[k] -= H[k][i] * t[i];
        }
    }

    return t;
}


// void aggregateMatrixH(const Grid& grid, GlobalSystemOfEquation& globalSystemOfEquation) {
//     int size = grid.nodes_number;
//     Matrix<double> globalMatrixH (size, Vector<double>(size, 0));    //matrix size x size
//
//     for (int i = 0; i < grid.elements.size(); i++) {
//         Matrix<double> localMatrixH = grid.elements[i].H;
//
//         int node_ids[4];
//         for (int j = 0; j < 4; ++j) {
//             node_ids[j] = grid.elements[i].node_id[j];
//         }
//
//         for (int j = 0; j < 4; ++j) {
//             for (int k = 0; k < 4; ++k) {
//                 globalMatrixH[node_ids[j]-1][node_ids[k]-1] += localMatrixH[j][k];
//             }
//         }
//     }
//
//     globalSystemOfEquation.globalMatrixH = globalMatrixH;
// }
//
// void aggregateVectorP(const Grid &grid, GlobalSystemOfEquation &globalSystemOfEquation) {
//     int size = grid.nodes_number;
//     Vector<double> globalVectorP(size, 0.0);
//
//     for (auto& element : grid.elements) {
//         Vector<double> local_P = element.P;
//         int node_ids[4];
//
//         for (int j = 0; j < 4; ++j) {
//             node_ids[j] = element.node_id[j];
//         }
//
//         for (int j = 0; j < 4; ++j) {
//                 globalVectorP[node_ids[j] - 1] += local_P[j];
//         }
//     }
//     globalSystemOfEquation.globalVectorP = globalVectorP;
// }

void aggregate(const Grid &grid, GlobalSystemOfEquation &globalSystemOfEquation) {

    int size = grid.nodes_number;
    Matrix<double> globalMatrixH (size, Vector<double>(size, 0));    //matrix size x size
    Matrix<double> globalMatrixC (size, Vector<double>(size, 0));    //matrix size x size
    Vector<double> globalVectorP(size, 0.0);

    for (int i = 0; i < grid.elements.size(); i++) {
        Matrix<double> localMatrixH = grid.elements[i].H;
        Matrix<double> localMatrixC = grid.elements[i].C;
        Vector<double> local_P = grid.elements[i].P;

        int node_ids[4];
        for (int j = 0; j < 4; ++j) {
            node_ids[j] = grid.elements[i].node_id[j];
        }

        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                globalMatrixH[node_ids[j]-1][node_ids[k]-1] += localMatrixH[j][k];
                globalMatrixC[node_ids[j]-1][node_ids[k]-1] += localMatrixC[j][k];
            }
            globalVectorP[node_ids[j] - 1] += local_P[j];
        }
    }

    globalSystemOfEquation.globalMatrixH = globalMatrixH;
    globalSystemOfEquation.globalMatrixC = globalMatrixC;
    globalSystemOfEquation.globalVectorP = globalVectorP;
}
