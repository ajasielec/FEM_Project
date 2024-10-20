#include "Node.h"

// NODE constructors
Node::Node() : id(-1), x(0), y(0) {}    // id = -1 if no values are gives
Node::Node(const int id, const double x, const double y) : id(id), x(x), y(y) {}
Node::Node(double x, double y) : id(0), x(x), y(y) {}   // default id = 0

// method that displays a single node
void Node::display() const {
    std::cout << id << ": (" << x << ", " << y << ")" << std::endl;
}