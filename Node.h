#ifndef NODE_H
#define NODE_H

#include "includes.h"

struct Node {
    int id;
    double x, y;

    Node();
    Node(double x, double y);
    Node(int id, double x, double y);
    void display() const;
};



#endif //NODE_H
