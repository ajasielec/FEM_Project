//
// Created by Ania on 20.10.2024.
//

#ifndef ELEMUNIV_H
#define ELEMUNIV_H

#include "includes.h"
#include "Node.h"

// values of nodes, used for creating a ksi and eta tables
static const std::vector<std::vector<Node>> pc {
            {Node(0.0, 0.0)},   // npc = 1
            {Node(-1/sqrt(3), -1/sqrt(3)),  Node(1/sqrt(3), -1/sqrt(3)),
                Node(-1/sqrt(3), 1/sqrt(3)), Node(1/sqrt(3), 1/sqrt(3))}, // npc = 4
            {Node(-sqrt((3.0/5.0)), -sqrt((3.0/5.0))), Node(-sqrt((3.0/5.0)), 0.0), Node(-sqrt((3.0/5.0)), sqrt((3.0/5.0))),
                Node(0.0, -sqrt((3.0/5.0))), Node(0.0, 0.0), Node(0.0, sqrt((3.0/5.0))),
                Node(sqrt((3.0/5.0)), -sqrt((3.0/5.0))), Node(sqrt((3.0/5.0)), 0.0), Node(sqrt((3.0/5.0)), sqrt((3.0/5.0)))}    // npc = 9
};
const std::vector<std::vector<Node>> wages {
                {Node(4.0, 4.0)},   // 1 npc wages
                {Node(1.0, 1.0),  Node(1.0, 1.0), Node(1.0, 1.0),  Node(1.0, 1.0)},     // 4 npc wages
                {Node(5.0/9.0, 5.0/9.0), Node(5.0/9.0, 8.0/9.0), Node(5.0/9.0, 5.0/9.0),
                    Node(8.0/9.0, 5.0/9.0),  Node(8.0/9.0, 8.0/9.0),  Node(8.0/9.0, 5.0/9.0),
                    Node(5.0/9.0, 5.0/9.0),  Node(5.0/9.0, 8.0/9.0),  Node(5.0/9.0, 5.0/9.0)}   // 9 npc wages
};

struct ElemUniv {
    std::vector<std::vector<double>> dN_dE; // dla x
    std::vector<std::vector<double>>  dN_dn; // dla y

    explicit ElemUniv (int npc);
    void display() const;
};



#endif //ELEMUNIV_H
