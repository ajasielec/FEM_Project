#ifndef NODE_H
#define NODE_H

#include "includes.h"

struct Node {
    int id;
    double x, y;
    bool BC;

    Node();
    Node(double x, double y);
    Node(int id, double x, double y, bool BC);
    void display() const;
};



#endif //NODE_H
