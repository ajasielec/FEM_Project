//
// Created by Ania on 20.11.2024.
//

#include "GlobalSystemOfEquation.h"
#include "includes.h"
#define SMALL_NUMBER 0.0001

void GlobalSystemOfEquation::solve() {
    // metoda gaussa
    Matrix<double> matrix;
    matrix = scalMacierz(globalMatrixH, globalVectorP);

    if(zeroOnDiagonal(globalMatrixH)) {
        std::cout << "Obliczenia niemozliwe - zera na przekatnej!" << std::endl;
    } else {
        vectorT = GaussElimination(matrix);
    }
}

void GlobalSystemOfEquation::displayMatrixH() {
    for (int i = 0; i < globalMatrixH.size(); i++) {
        for (int j = 0; j < globalMatrixH[i].size(); j++) {
            std::cout << globalMatrixH[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void GlobalSystemOfEquation::displayVectorP() {
    for (int i = 0; i < globalVectorP.size(); i++) {
        std::cout << globalVectorP[i] << std::endl;
    }
}

// function to gauss elimination
bool zeroOnDiagonal(Matrix<double> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        if (matrix[i][i] == 0) return true;
    }
    return false;
}

Vector <double> GaussElimination(Matrix<double> matrix) {
    // 1 etap
    int n = matrix.size();
    double m = 0;
    for (int z = 0; z < n; z++) {
        for (int i = z + 1; i < n; i++) {
            m = matrix[i][z] / matrix[z][z];
            for (int j = 0; j < n + 1; j++) {
                matrix[i][j] -= (matrix[z][j] * m);
                if (abs(matrix[i][j]) < SMALL_NUMBER) {
                    matrix[i][j] = 0;
                }
            }
        }
    }

    // 2 etap
    Vector<double> results(n);
    for (int i = n - 1; i >= 0; --i) {
        results[i] = matrix[i][n] / matrix[i][i];
        for (int j = i - 1; j >= 0; --j) {
            matrix[j][n] -= matrix[j][i] * results[i];
        }
    }

    return results;
}

Matrix<double> scalMacierz(Matrix<double> a, Vector<double> b) {
    Matrix<double> matrix;
    for (int i = 0; i < a.size(); i++) {
        matrix.push_back(a[i]);
        matrix[i].push_back(b[i]);
    }
    return matrix;
}


void aggregateMatrixH(const Grid& grid, GlobalSystemOfEquation& globalSystemOfEquation) {
    int size = grid.nodes_number;
    Matrix<double> globalMatrixH (size, Vector<double>(size, 0));    //matrix size x size

    for (int i = 0; i < grid.elements.size(); i++) {
        Matrix<double> localMatrixH = grid.elements[i].H;

        int node_ids[4];
        for (int j = 0; j < 4; ++j) {
            node_ids[j] = grid.elements[i].node_id[j];
        }

        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                globalMatrixH[node_ids[j]-1][node_ids[k]-1] += localMatrixH[j][k];
            }
        }
    }

    globalSystemOfEquation.globalMatrixH = globalMatrixH;
}

void aggregateVectorP(const Grid &grid, GlobalSystemOfEquation &globalSystemOfEquation) {
    int size = grid.nodes_number;
    Vector<double> globalVectorP(size, 0.0);

    for (auto& element : grid.elements) {
        Vector<double> local_P = element.P;
        int node_ids[4];

        for (int j = 0; j < 4; ++j) {
            node_ids[j] = element.node_id[j];
        }

        for (int j = 0; j < 4; ++j) {
                globalVectorP[node_ids[j] - 1] += local_P[j];
        }
    }
    globalSystemOfEquation.globalVectorP = globalVectorP;
}
