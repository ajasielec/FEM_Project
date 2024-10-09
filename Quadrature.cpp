//
// Created by Ania on 09.10.2024.
//

#include "Quadrature.h"

#include <iostream>
#include <ostream>

static const std::vector<std::vector<double>> predefined_points = {
        {0},  // for 1 node
        {-sqrt(3.0) / 3.0, sqrt(3.0) / 3.0},  // for 2 nodes
        {-sqrt(3.0 / 5.0), 0, sqrt(3.0 / 5.0)},  // for 3 nodes
        {-0.861136, -0.339981, 0.339981, 0.861136}  // for 4 nodes
};

static const std::vector<std::vector<double>> predefined_wages = {
        {2},  // for 1 node
        {1, 1},  // for 2 nodes
        {5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0},  // for 3 nodes
        {0.347855, 0.652145, 0.652145, 0.347855}  // for 4 nodes
};

Quadrature::Quadrature(int nodes_number) {
    this->nodes_number = nodes_number;
        if (nodes_number < 1 || nodes_number > 4) {
                std::cerr << "Invalid node number" << std::endl;
                return;
        }
        points = predefined_points[nodes_number - 1];
        wages = predefined_wages[nodes_number - 1];
}


double Quadrature::solveQuadratures(double (*f)(double x)) const {
        double sum = 0.0;
        for (int i = 0; i < nodes_number; i++) {
                sum += wages[i] * f(points[i]);
        }
        return sum;
}

double Quadrature::solveQuadratures(double (*f)(double x, double y)) const {
        double sum = 0.0;
        for (int i = 0; i < nodes_number; i++) {
                for(int j = 0; j < nodes_number; j++) {
                        sum += wages[i] * wages[j] * f(points[i], points[j]);
                }
        }
        return sum;
}



